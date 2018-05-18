#include "constants.h"
#include "test-tools.h"
#include "types.h"
int main(void) {
	const u8 compressed[] = {
		0, 1, 36, 3, 0, 7,
		166, 0, 0,
		166, 0, 0,
		148, 0, 0,
		END
	};
	const u8 decompressed[] = {
		1, 3, 3, 3, 3, 3, 7,
		128, 192, 192, 192, 192, 192, 224,
		128, 192, 192, 192, 192, 192, 224,
		1, 3, 3, 3, 3, 3, 7,
		128, 192, 192, 192, 192, 192, 224,
		128, 192, 192, 192, 192, 192, 224
	};
	return test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}