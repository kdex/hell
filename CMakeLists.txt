cmake_minimum_required(VERSION 3.9.4)
project(hell VERSION 0.0.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type specified; falling back to `Release`")
	set(CMAKE_BUILD_TYPE Release)
endif()
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()
set(COMMON_FLAGS
	-Wall
	-Wextra
	-pedantic
)
set(DEBUG_FLAGS
	-DDEBUG
	-Wrestrict
	-fsanitize=address,undefined
)
set(RELEASE_FLAGS
	-s
	-O3
	-fomit-frame-pointer
	-ffunction-sections
	-fdata-sections
	-fno-unwind-tables
	-fno-asynchronous-unwind-tables
	-fno-math-errno
	-fno-unroll-loops
	-fmerge-all-constants
	-fno-ident
	-ffast-math
)
if (CMAKE_C_COMPILER_ID MATCHES "GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdelete-null-pointer-checks")
endif()
if (CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-newline-eof")
endif()
foreach(FLAG ${COMMON_FLAGS})
	if (CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
	endif()
endforeach()
if(CMAKE_BUILD_TYPE MATCHES Debug)
	foreach(FLAG ${DEBUG_FLAGS})
		if (CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
		endif()
	endforeach()
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
	foreach(FLAG ${RELEASE_FLAGS})
		if (CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
		endif()
	endforeach()
endif()
message(${CMAKE_C_FLAGS})
include_directories(include src)
enable_testing()
add_subdirectory(src)
add_subdirectory(test)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/HellConfigVersion.cmake"
	COMPATIBILITY SameMajorVersion
)
install(
	FILES "HellConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/HellConfigVersion.cmake"
	DESTINATION lib/cmake/${PROJECT_NAME}
)
install(EXPORT HellTargets
	FILE HellTargets.cmake
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION lib/cmake/${PROJECT_NAME}
)