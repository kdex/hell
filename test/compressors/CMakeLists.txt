foreach(TEST
	copy-bytes-1
	copy-bytes-32
	copy-bytes-33
	copy-bytes-1024
	copy-reversed-bits-1
	copy-reversed-bits-32
	copy-reversed-bits-33
	copy-reversed-bits-1024
	copy-reversed-bytes-1
	copy-reversed-bytes-32
	copy-reversed-bytes-33
	copy-reversed-bytes-1024
	fill-byte-1
	fill-byte-32
	fill-byte-33
	fill-byte-1024
	fill-bytes-1
	fill-bytes-32
	fill-bytes-33
	fill-bytes-1024
	fill-incremental-sequence-1
	fill-incremental-sequence-32
	fill-incremental-sequence-33
	fill-incremental-sequence-1024
	uncompressed-1
	uncompressed-32
	uncompressed-33
	uncompressed-1024
)
	set(NAME compressors-${TEST})
	add_executable(${NAME}
		${TEST}.c
	)
	target_link_libraries(${NAME} test-tools io libhell-static)
	add_test(${NAME} ${NAME})
endforeach()