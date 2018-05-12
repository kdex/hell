#include "test-tools.h"
#include "types.h"
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 compressed[] = {
			243, 255, i, END
		};
		const int value = testCopyBytes(NULL, 1024, compressed, sizeof compressed, i);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
}