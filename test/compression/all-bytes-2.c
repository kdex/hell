#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		for (u16 j = 0; j <= UINT8_MAX; ++j) {
			const u8 uncompressed[] = { i, j };
			int result;
			if (i == j) {
				const u8 compressed[] = { 33, i, END };
				result = test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
			}
			else if ((u8) (i + 1) == (u8) j) {
				const u8 compressed[] = { 97, i, END };
				result = test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
			}
			else {
				const u8 compressed[] = { 1, i, j, END };
				result = test(compress, uncompressed, sizeof uncompressed, compressed, sizeof compressed);
			}
			if (result == EXIT_FAILURE) {
				return result;
			}
		}
	}
}