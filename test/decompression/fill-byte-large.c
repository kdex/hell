#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
int main(void) {
	const u8 compressed[] = {
		228, 32, 1, END
	};
	const u8 decompressed[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1
	};
	test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}