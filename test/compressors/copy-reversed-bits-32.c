#include "test-tools.h"
#include "types.h"
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 compressed[] = {
			191, i, END
		};
		const int value = testCopy(NULL, 32, compressed, sizeof compressed, COPY_REVERSED_BITS, i);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
}