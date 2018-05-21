foreach(TEST
	copy-bytes
	copy-bytes-bit-reversed
	copy-bytes-bit-reversed-advanced
	copy-bytes-reversed
	fill-byte-large
	fill-byte-small
	fill-bytes-large
	fill-bytes-small
	fill-bytes-small-max
	fill-incremental-sequence-0-255
	fill-incremental-sequence-0-255-0-255
	fill-incremental-sequence-small
	uncompressed-1
	uncompressed-2
)
	set(NAME decompression-${TEST})
	add_executable(${NAME}
		${TEST}.c
	)
	target_link_libraries(${NAME} test-tools io libhell-static)
	add_test(${NAME} ${NAME})
endforeach()