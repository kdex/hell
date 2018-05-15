#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		1, 2, 3, 4, 5,
		1, 2, 3, 4, 5,
		1, 2, 3, 4, 5,
		1, 2, 3, 4, 5
	};
	const u8 compressed[] = {
		100, 1, END,
		132, 5, END,
		137, 10, END
	};
	return test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}