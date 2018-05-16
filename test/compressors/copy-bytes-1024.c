#include "test-tools.h"
#include "types.h"
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 compressed[] = {
			243, 255, 0, i
		};
		const int value = testCopy(NULL, 1024, compressed, sizeof compressed, COPY_BYTES, i);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
}