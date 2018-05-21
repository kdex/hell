#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		0, 1, 2, 3, 4,
		0, 128, 64, 192, 32,
		251, 252, 253, 254, 255,
		223, 63, 191, 127, 255
	};
	const u8 compressed[] = {
		100, 0,
		164, 0, 0,
		100, 251,
		164, 0, 10,
		END
	};
	test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}