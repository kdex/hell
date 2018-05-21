#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
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
	test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}