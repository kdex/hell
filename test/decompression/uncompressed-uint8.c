#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 decompressed[] = { i };
		const u8 compressed[] = { 0, i, END };
		const int result = testDecompress(compressed, decompressed, sizeof decompressed);
		if (result == EXIT_FAILURE) {
			return result;
		}
	}
}