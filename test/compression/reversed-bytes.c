#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		0, 1, 2, 3, 4,
		4, 3, 2, 1, 0,
		0, 1, 2, 3, 4,
		4, 3, 2, 1, 0
	};
	const u8 compressed[] = {
		100, 0,
		196, 0, 4,
		137, 0, 0,
		END
	};
	test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}