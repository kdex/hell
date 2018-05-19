#include "constants.h"
#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	const u8 uncompressed[] = {
		'O', 'v', 'e', 'r', 's', 'c', 'a', 'n',
		'O', 'v', 'e', 'r', 's', 'c', 'a', 'n',
		'O', 'v', 'e', 'r', 's', 'c', 'a', 'n'
	};
	const u8 compressed[] = {
		7,
		'O', 'v', 'e', 'r', 's', 'c', 'a', 'n',
		143, 0, 0,
		END
	};
	test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}