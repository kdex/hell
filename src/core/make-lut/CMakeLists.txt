add_executable(make-lut
	main.c
)
target_link_libraries(make-lut $<TARGET_OBJECTS:common> io)