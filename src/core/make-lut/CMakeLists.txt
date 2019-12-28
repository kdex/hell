add_executable(make-lut
	main.c
)
if(NOT APPLE AND NOT MSVC)
	add_custom_command(
		TARGET make-lut
		POST_BUILD
		COMMAND ${CMAKE_STRIP} make-lut
	)
endif()
target_link_libraries(make-lut $<TARGET_OBJECTS:common> io)