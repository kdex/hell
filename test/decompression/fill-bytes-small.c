#include "test-tools.h"
#include "types.h"
int main(void) {
	const u8 compressed[] = {
		70, 1, 2, END
	};
	const u8 decompressed[] = {
		1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
		1, 2, 1, 2
	};
	return test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}