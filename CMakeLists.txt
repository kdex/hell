cmake_minimum_required(VERSION 3.9.2)
project(hell VERSION 0.0.1)
set(CMAKE_C_STANDARD 11)
if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type specified; falling back to `Release`")
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
if(IPO_SUPPORTED)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
if(NOT APPLE)
	set(CMAKE_LINK_WHAT_YOU_USE TRUE)
endif()
add_custom_target(lint
	COMMAND ./lint
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
set(COMMON_FLAGS
	-pedantic
	-Wall
	-Wextra
	-Wsign-conversion
)
set(DEBUG_FLAGS
	--coverage
	-DDEBUG
	-fsanitize=address,undefined
)
set(RELEASE_FLAGS
	-fdata-sections
	-ffast-math
	-ffunction-sections
	-fmerge-all-constants
	-fno-asynchronous-unwind-tables
	-fno-math-errno
	-fno-unroll-loops
	-fno-unwind-tables
	-fomit-frame-pointer
	-O3
)
set(GCC FALSE)
set(CLANG FALSE)
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
	set(GCC TRUE)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(CLANG TRUE)
endif()
# Use lld if available
execute_process(
	COMMAND ${CMAKE_C_COMPILER} -fuse-ld=lld -Wl,--version
	ERROR_QUIET OUTPUT_VARIABLE LD_VERSION
)
if("${LD_VERSION}" MATCHES "LLD" AND ${CLANG})
	message(STATUS ${LD_VERSION})
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
	unset(LD_VERSION)
else()
	# Use gold if available
	execute_process(
		COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version
		ERROR_QUIET OUTPUT_VARIABLE LD_VERSION
	)
	if("${LD_VERSION}" MATCHES "GNU gold")
		message(STATUS ${LD_VERSION})
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
		unset(LD_VERSION)
	else()
		message(WARNING "Neither lld nor GNU gold linker is available, using the default system linker")
	endif()
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
		if(${GCC} OR ${CLANG})
			add_flag(${FLAG})
		endif()
	endforeach()
	if(${DEBUG})
		foreach(FLAG ${DEBUG_FLAGS})
			if(${GCC} OR ${CLANG})
				add_flag(${FLAG})
			endif()
		endforeach()
	endif()
	if(${RELEASE})
		foreach(FLAG ${RELEASE_FLAGS})
			if(${GCC} OR ${CLANG})
				add_flag(${FLAG})
			endif()
		endforeach()
	endif()
else()
	add_definitions(
		-D_CRT_DECLARE_NONSTDC_NAMES=0
		-DNOMINMAX
		-DWIN32_LEAN_AND_MEAN
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