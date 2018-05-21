#include "common/types.h"
#include "test/tools.h"
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 compressed[] = {
			159, 0, i
		};
		testCopy(NULL, 32, compressed, sizeof compressed, COPY_BYTES, i);
	}
}