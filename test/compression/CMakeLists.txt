foreach(TEST
	abc
	all-bytes-1
	all-bytes-2
	byte-pair
	lz-20
	lz-25
	lz-30
	mixed
	overscan
	reversed-bits
	reversed-bits-overlap
)
	set(NAME compression-${TEST})
	add_executable(${NAME}
		${TEST}.c
	)
	target_link_libraries(${NAME} test-tools io libhell-static)
	add_test(${NAME} ${NAME})
endforeach()