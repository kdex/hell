#include "test-tools.h"
#include "types.h"
int main(void) {
	const u8 decompressed[] = {
		1
	};
	const u8 compressed[] = {
		0, 1, END
	};
	return testUncompressedU8(decompressed, sizeof decompressed, compressed, sizeof compressed);
}