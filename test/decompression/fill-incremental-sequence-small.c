#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
int main(void) {
	const u8 compressed[] = {
		110, 1, END
	};
	const u8 decompressed[] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15
	};
	test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}