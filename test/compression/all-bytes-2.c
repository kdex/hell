#include "constants.h"
#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		for (u16 j = 0; j <= UINT8_MAX; ++j) {
			const u8 uncompressed[] = { i, j };
			const u8 compressed[] = { 1, i, j, END };
			test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
		}
	}
}