tests = [
	'uncompressed-1',
	'uncompressed-32',
	'uncompressed-33',
	'uncompressed-1024',
	'fill-byte-1',
	'fill-byte-32',
	'fill-byte-33',
	'fill-byte-1024',
	'fill-bytes-1',
	'fill-bytes-32',
	'fill-bytes-33',
	'fill-bytes-1024'
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