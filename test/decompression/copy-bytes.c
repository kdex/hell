#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
int main(void) {
	const u8 compressed[] = {
		0, 1, 36, 3, 0, 7,
		134, 0, 0,
		134, 0, 0,
		END
	};
	const u8 decompressed[] = {
		1, 3, 3, 3, 3, 3, 7,
		1, 3, 3, 3, 3, 3, 7,
		1, 3, 3, 3, 3, 3, 7
	};
	test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}