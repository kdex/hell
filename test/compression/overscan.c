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
		2, 'O', 'v', 'e',
		97, 'r',
		2, 'c', 'a', 'n',
		143, 0, 0,
		END
	};
	return test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}