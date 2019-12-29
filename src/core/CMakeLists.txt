set(LUT lut.c)
add_custom_command(
	OUTPUT ${LUT}
	COMMAND make-lut ${LUT}
)
set(META meta.c)
add_custom_command(
	OUTPUT ${META}
	COMMAND make-meta ${META}
)
set(LIBRARY_SOURCES
	allocation.c
	compress.c
	compression-context.c
	compressors.c
	decompress.c
	header.c
	util.c
	${LUT}
	${META}
)
add_library(libhell-object OBJECT ${LIBRARY_SOURCES})
set_target_properties(libhell-object PROPERTIES
	POSITION_INDEPENDENT_CODE 1
)
set(OBJECTS
	$<TARGET_OBJECTS:common>
	$<TARGET_OBJECTS:libhell-object>
)
add_library(libhell-shared SHARED ${OBJECTS})
add_library(libhell-static STATIC ${OBJECTS})
set_target_properties(libhell-shared PROPERTIES
	OUTPUT_NAME ${PROJECT_NAME}
)
set_target_properties(libhell-static PROPERTIES
	OUTPUT_NAME ${PROJECT_NAME}
)
try_strip(libhell-shared)
add_subdirectory(make-lut)
add_subdirectory(make-meta)
set(PUBLIC_HEADERS
	compress.h
	decompress.h
	hell.h
)
set_target_properties(libhell-shared PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")
install(
	TARGETS libhell-shared libhell-static EXPORT HellTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)