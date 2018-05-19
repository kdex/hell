#include "constants.h"
#include "test-tools.h"
#include "types.h"
int main(void) {
	const u16 elements = 2 * (UINT8_MAX + 1);
	u8 decompressed[elements];
	for (u16 i = 0; i < elements; ++i) {
		decompressed[i] = i;
	}
	const u8 compressed[] = {
		236, 255, 0,
		236, 255, 0,
		END
	};
	test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}