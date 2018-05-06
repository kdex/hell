#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main() {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		for (u16 j = 0; j <= UINT8_MAX; ++j) {
			const u8 uncompressed[] = { i, j };
			const u8 compressed[] = { 1, i, j, END };
			const int result = compare(compressed, uncompressed, sizeof uncompressed);
			if (result == EXIT_FAILURE) {
				return result;
			}
		}
	}
}