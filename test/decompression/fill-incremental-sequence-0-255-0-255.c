#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
#define ELEMENTS (2 * (UINT8_MAX + 1))
int main(void) {
	u8 decompressed[ELEMENTS];
	for (u16 i = 0; i < ELEMENTS; ++i) {
		decompressed[i] = i;
	}
	const u8 compressed[] = {
		236, 255, 0,
		236, 255, 0,
		END
	};
	test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
}