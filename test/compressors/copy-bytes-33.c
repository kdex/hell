#include "test-tools.h"
#include "types.h"
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 compressed[] = {
			240, 32, 0, i
		};
		testCopy(NULL, 33, compressed, sizeof compressed, COPY_BYTES, i);
	}
}