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
		4,
		'a', ' ',
		'b', ' ',
		'c',
		END
	};
	return test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}