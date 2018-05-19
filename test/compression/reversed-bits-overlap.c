#include "constants.h"
#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		0, 1, 2, 3, 4,
		0, 128, 64, 192, 32,
		1, 2, 3, 4, 5,
		128, 64, 192, 32, 160
	};
	const u8 compressed[] = {
		100, 0,
		164, 0, 0,
		100, 1,
		164, 0, 10,
		END
	};
	test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}