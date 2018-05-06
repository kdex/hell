#include "test-tools.h"
#include "types.h"
#include <stdlib.h>
int main() {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 uncompressed[] = { i };
		const u8 compressed[] = { 0, i, END };
		const int result = compare(compressed, uncompressed, sizeof uncompressed);
		if (result == EXIT_FAILURE) {
			return result;
		}
	}
}