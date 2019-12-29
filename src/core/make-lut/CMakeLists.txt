add_executable(make-lut
	main.c
)
try_strip(make-lut)
target_link_libraries(make-lut $<TARGET_OBJECTS:common> io)