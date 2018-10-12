cmake_minimum_required(VERSION 3.9.2)
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
if (NOT APPLE)
	set(CMAKE_LINK_WHAT_YOU_USE TRUE)
endif()
find_program(CLANG_TIDY NAMES clang-tidy)
if(CLANG_TIDY)
	message(STATUS "Found clang-tidy: ${CLANG_TIDY}")
	set(CLANG_TIDY_OPTIONS
		-checks=-*
		-clang-diagnostic-newline-eof
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
	set(CMAKE_C_CLANG_TIDY
		${CLANG_TIDY}
		-system-headers
		-checks=${CLANG_TIDY_OPTIONS_COMMA_SEPARATED}
	)
endif()
set(COMMON_FLAGS
	-Wall
	-Wextra
	-pedantic
	-Wsign-conversion
)
set(DEBUG_FLAGS
	-DDEBUG
	-fsanitize=address,undefined
	--coverage
)
set(RELEASE_FLAGS
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
set(GCC FALSE)
set(CLANG FALSE)
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
	set(GCC TRUE)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(CLANG TRUE)
endif()
set(DEBUG FALSE)
set(RELEASE FALSE)
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(DEBUG TRUE)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	set(RELEASE TRUE)
endif()
function(add_flag FLAG)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}" PARENT_SCOPE)
endfunction()
if(NOT MSVC)
	if(${CLANG})
		add_flag(-Wno-newline-eof)
	endif()
	foreach(FLAG ${COMMON_FLAGS})
		if (${GCC} OR ${CLANG})
			add_flag(${FLAG})
		endif()
	endforeach()
	if(${DEBUG})
		foreach(FLAG ${DEBUG_FLAGS})
			if (${GCC} OR ${CLANG})
				add_flag(${FLAG})
			endif()
		endforeach()
	endif()
	if(${RELEASE})
		foreach(FLAG ${RELEASE_FLAGS})
			if (${GCC} OR ${CLANG})
				add_flag(${FLAG})
			endif()
		endforeach()
	endif()
else()
	add_definitions(
		/DNOMINMAX
		/D_CRT_DECLARE_NONSTDCNAMES=0
	)
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