#include "test-tools.h"
#include "types.h"
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 compressed[] = {
			128, i, END
		};
		const int value = testCopyBytes(NULL, 1, compressed, sizeof compressed, i);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
}