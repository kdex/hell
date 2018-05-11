#pragma once
#include "compress.h"
#include "compressors.h"
#include "decompress.h"
#include "types.h"
#include <stdlib.h>
int test(
	size_t (*fn)(const u8 *restrict payload,
	size_t payloadSize, u8 **restrict destination),
	const u8 *restrict payload,
	size_t payloadSize,
	const u8 *restrict expected,
	size_t expectedSize
);
int testUncompressed(
	const u8 *restrict payload,
	size_t payloadSize,
	const u8 *restrict expected,
	size_t expectedSize
);
int testFillByte(
	const u8 *restrict payload,
	size_t payloadSize,
	const u8 *restrict expected,
	size_t expectedSize,
	u8 byte
);
int compare(const u8 *restrict actual, size_t actualSize, const u8 *restrict expected, size_t expectedSize);
int fail(const char *format, ...);