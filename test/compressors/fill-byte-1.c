#include "test-tools.h"
#include "types.h"
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 uncompressed[] = { i };
		const u8 compressed[] = {
			32, i, END
		};
		const int value = testFillByte(uncompressed, sizeof uncompressed, compressed, sizeof compressed, i);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
}