#include "test-tools.h"
#include "types.h"
int main(void) {
	const u8 uncompressed[] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		END, 32
	};
	const u8 compressed[] = {
		31,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		END, 32
	};
	return testUncompressed(uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}