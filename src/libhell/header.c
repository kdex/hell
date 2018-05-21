#include "header.h"
#include "common/constants.h"
#include "common/types.h"
#include <stdlib.h>
struct HeaderInternals {
	u8 mask;
	u8 indexShift;
	u8 modeShift;
};
void initHeader(Header *restrict header, u8 headerBits, u8 indexBits) {
	const u8 freeBits = headerBits - indexBits - MODE_BITS;
	*header = (Header) {
		.size = headerBits / BITS_IN_BYTE,
		.capacity = 1 << indexBits,
		.internals = malloc(sizeof (HeaderInternals))
	};
	*header->internals = (HeaderInternals) {
		.mask = 0xff << (BITS_IN_BYTE - freeBits),
		.indexShift = indexBits - (indexBits % BITS_IN_BYTE),
		.modeShift = indexBits % BITS_IN_BYTE,
	};
}
u8 makeFirstByte(const Header *restrict header, CompressionMode mode, u16 size) {
	const u16 maxIndex = size - 1;
	return header->internals->mask | mode << header->internals->modeShift | maxIndex >> header->internals->indexShift;
}
void initSmallHeader(Header *restrict header) {
	/*
	* Small header in memory: MMMIIIII
	*   MMM = Compression mode
	* IIIII = Maximum index of compressed data
	*/
	initHeader(header, BITS_IN_BYTE, 5);
}
void initLargeHeader(Header *restrict header) {
	/*
	* Large header in memory: 111MMMJJ IIIIIIII
	*   MMM = Compression mode
	*    JJ = Maximum index of compressed data (upper 2 bits)
	* IIIII = Maximum index of compressed data (lower 8 bits)
	*/
	initHeader(header, 2 * BITS_IN_BYTE, 10);
}
void freeHeader(Header *restrict header) {
	free(header->internals);
	free(header);
}