#pragma once
#include "common/types.h"
#include "core/compress.h"
#include "core/compression-mode.h"
#include "core/compressors.h"
#include "core/decompress.h"
#include <stdlib.h>
void test(
	size_t (*fn)(
		const u8 *restrict payload,
		size_t payloadSize,
		u8 **restrict destination
	),
	const u8 *restrict payload,
	size_t payloadSize,
	const u8 *restrict expected,
	size_t expectedSize
);
void testUncompressed(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize
);
void testFillByte(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	u8 byte
);
void testFillBytes(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	u8 byteA,
	u8 byteB
);
void testFillIncrementalSequence(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	u8 seed
);
void testCopy(
	const u8 *restrict uncompressed,
	u16 uncompressedSize,
	const u8 *restrict expected,
	u16 expectedSize,
	CompressionMode mode,
	u16 copyOffset
);
void compare(const u8 *restrict actual, size_t actualSize, const u8 *restrict expected, size_t expectedSize);