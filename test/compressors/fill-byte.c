#include "test-tools.h"
#include "types.h"
int main(void) {
	int value;
	/* Payload size == 1 */
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 uncompressed[] = { i };
		const u8 compressed[] = {
			32, i, END
		};
		value = testFillByte(uncompressed, sizeof uncompressed, compressed, sizeof compressed, i);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
	/* Payload size == 32 */
	u8 uncompressed32[32];
	for (u16 i = 0; i < 32; ++i) {
		uncompressed32[i] = 42;
	}
	const u8 compressed32[] = {
		63, 42, END
	};
	value = testFillByte(uncompressed32, sizeof uncompressed32, compressed32, sizeof compressed32, 42);
	if (value == EXIT_FAILURE) {
		return value;
	}
}