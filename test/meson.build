tests = [
	'copy-bytes',
	'copy-bytes-bit-reversed',
	'copy-bytes-bit-reversed-advanced',
	'copy-bytes-reversed',
	'fill-byte-large',
	'fill-byte-small',
	'fill-bytes-large',
	'fill-bytes-small',
	'fill-bytes-small-max',
	'fill-incremental-sequence-0-255',
	'fill-incremental-sequence-0-255-0-255',
	'fill-incremental-sequence-small',
	'uncompressed-uint8',
	'uncompressed-uint16'
]
foreach test : tests
	testFile = test + '.c'
	executable = executable(test, [
		testFile,
		testDependencies
	], lut,
		c_args: cflags,
		include_directories: includes
	)
	test(test, executable)
endforeach