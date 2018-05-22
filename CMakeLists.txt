cmake_minimum_required(VERSION 3.9.4)
project(hell VERSION 0.0.1)
set(CMAKE_C_STANDARD 11)
if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type specified; falling back to `Release`")
	set(CMAKE_BUILD_TYPE Release)
endif()
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
set(CMAKE_LINK_WHAT_YOU_USE TRUE)
set(CLANG_TIDY_OPTIONS
	-checks=-*
	bugprone-*
	llvm-include-order
	misc-*
	-misc-suspicious-string-compare
	modernize-*
	performance-*
	portability-*
	readability-*
)
string(REPLACE ";" "," CLANG_TIDY_OPTIONS_COMMA_SEPARATED "${CLANG_TIDY_OPTIONS}")
set(CMAKE_C_CLANG_TIDY "clang-tidy;${CLANG_TIDY_OPTIONS_COMMA_SEPARATED}")
set(COMMON_FLAGS
	-Wall
	-Wextra
	-pedantic
)
set(DEBUG_FLAGS
	-DDEBUG
	-fsanitize=address,undefined
	--coverage
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
enable_testing()
include_directories(src)
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