#include "test-tools.h"
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
	return testDecompress(compressed, decompressed, sizeof decompressed);
}