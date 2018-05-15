tests = [
	'uncompressed-1',
	'uncompressed-2'
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