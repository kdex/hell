#include "constants.h"
#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 uncompressed[] = { i };
		const u8 compressed[] = { 0, i, END };
		test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
	}
}