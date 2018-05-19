#include "constants.h"
#include "test-tools.h"
#include "types.h"
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