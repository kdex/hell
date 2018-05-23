#pragma once
#include "common/restrict.h"
#include "common/types.h"
#include "core/compress.h"
#include "core/compression-mode.h"
#include "core/compressors.h"
#include "core/decompress.h"
#include <stdlib.h>
void test(
	size_t (*fn)(
		const u8 *RESTRICT payload,
		size_t payloadSize,
		u8 **RESTRICT destination
	),
	const u8 *RESTRICT payload,
	size_t payloadSize,
	const u8 *RESTRICT expected,
	size_t expectedSize
);
void testUncompressed(
	const u8 *RESTRICT uncompressed,
	u16 uncompressedSize,
	const u8 *RESTRICT expected,
	u16 expectedSize
);
void testFillByte(
	const u8 *RESTRICT uncompressed,
	u16 uncompressedSize,
	const u8 *RESTRICT expected,
	u16 expectedSize,
	u8 byte
);
void testFillBytes(
	const u8 *RESTRICT uncompressed,
	u16 uncompressedSize,
	const u8 *RESTRICT expected,
	u16 expectedSize,
	u8 byteA,
	u8 byteB
);
void testFillIncrementalSequence(
	const u8 *RESTRICT uncompressed,
	u16 uncompressedSize,
	const u8 *RESTRICT expected,
	u16 expectedSize,
	u8 seed
);
void testCopy(
	const u8 *RESTRICT uncompressed,
	u16 uncompressedSize,
	const u8 *RESTRICT expected,
	u16 expectedSize,
	CompressionMode mode,
	u16 copyOffset
);
void compare(const u8 *RESTRICT actual, size_t actualSize, const u8 *RESTRICT expected, size_t expectedSize);