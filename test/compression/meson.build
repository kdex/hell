tests = [
	'abc',
	'all-bytes-1',
	'all-bytes-2',
	'byte-pair',
	'lz-20',
	'lz-25',
	'lz-30',
	'mixed',
	'overscan',
	'reversed-bits',
	'reversed-bits-overlap',
	'reversed-bytes'
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