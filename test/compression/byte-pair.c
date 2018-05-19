#include "constants.h"
#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
		1, 2, 1, 2, 1, 2, 1, 2, 1, 2
	};
	const u8 compressed[] = {
		73, 1, 2, END
	};
	test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}