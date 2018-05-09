#include "header-layout.h"
#include "types.h"
#include <stdlib.h>
struct HeaderLayout *makeLayout(size_t headerBits, size_t indexBits) {
	const u8 freeBits = headerBits - indexBits - MODE_BITS;
	struct HeaderLayout *layout = malloc(sizeof (struct HeaderLayout));
	*layout = (struct HeaderLayout) {
		.headerBytes = BITS_IN_BYTE * headerBits,
		.mask = 0xff << (BITS_IN_BYTE - freeBits),
		.indexShift = indexBits - (indexBits % BITS_IN_BYTE),
		.modeShift = indexBits % BITS_IN_BYTE,
		.maxStorage = 1 << indexBits
	};
	return layout;
}
u8 makeFirstByte(struct HeaderLayout *layout, CompressionMode mode, u16 size) {
	const u16 maxIndex = size - 1;
	return layout->mask + (mode << layout->modeShift) + (maxIndex >> layout->indexShift);
}
struct HeaderLayout *makeSmallHeader(void) {
	/*
	* Small layout in memory: CCCIIIII
	*   CCC = Compression mode
	* IIIII = Maximum index of compressed data
	*/
	return makeLayout(BITS_IN_BYTE, 5);
}
struct HeaderLayout *makeLargeHeader(void) {
	/*
	* Large layout in memory: 111CCCJJ IIIIIIII
	*   CCC = Compression mode
	*    JJ = Maximum index of compressed data (upper 2 bits)
	* IIIII = Maximum index of compressed data (lower 8 bits)
	*/
	return makeLayout(2 * BITS_IN_BYTE, 10);
}