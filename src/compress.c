#include "compress.h"
#include "allocation.h"
#include "compression-mode.h"
#include "compressors.h"
#include "header-layout.h"
#include "types.h"
#include <stdbool.h>
#include <stdlib.h>


#include <stdio.h>


size_t compress(const u8 *decompressed, size_t decompressedSize, u8 **compressed) {
	struct Allocation allocation = { compressed, 0 };
	*allocation.block = NULL;
	/*
	* Small layout in memory: CCCIIIII
	*   CCC = Compression mode
	* IIIII = Maximum index of compressed data
	*/
	struct HeaderLayout *small = makeLayout(BITS_IN_BYTE, 5);
	/*
	* Large layout in memory: 111CCCJJ IIIIIIII
	*   CCC = Compression mode
	*    JJ = Maximum index of compressed data (upper 2 bits)
	* IIIII = Maximum index of compressed data (lower 8 bits)
	*/
	struct HeaderLayout *large = makeLayout(2 * BITS_IN_BYTE, 10);
	printf("First byte %i\n", makeFirstByte(small, FILL_BYTE, 10), compressed, decompressed, decompressedSize);
	printf("First byte %i\n", makeFirstByte(large, UNCOMPRESSED, 100), compressed, decompressed, decompressedSize);
	free(small);
	free(large);
	while (true) {
		return 0;
	}
}