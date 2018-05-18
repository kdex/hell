#include "constants.h"
#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		'a', ' ',
		'b', ' ',
		'c',
	};
	const u8 compressed[] = {
		2, 'a', ' ', 'b',
		128, 0, 1,
		0, 'c',
		END
	};
	return test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}