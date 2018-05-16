#include "test-tools.h"
#include "types.h"
int main(void) {
	const u8 compressed[] = {
		0, 1, 36, 3, 0, 7,
		198, 0, 6,
		198, 0, 6,
		END
	};
	const u8 decompressed[] = {
		1, 3, 3, 3, 3, 3, 7,
		7, 3, 3, 3, 3, 3, 1,
		7, 3, 3, 3, 3, 3, 1
	};
	return test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}