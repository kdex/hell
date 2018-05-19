#include "constants.h"
#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		1, 2, 3, 4, 5,
		10, 10, 10, 10, 10,
		8, 9, 8, 9, 8, 9,
		8, 9, 8, 9, 8, 9
	};
	const u8 compressed[] = {
		100, 1,
		36, 10,
		69, 8, 9,
		END
	};
	test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}