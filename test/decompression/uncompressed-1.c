#include "common/constants.h"
#include "common/types.h"
#include "test/tools.h"
#include <stdlib.h>
int main(void) {
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 decompressed[] = { i };
		const u8 compressed[] = { 0, i, END };
		test(decompress, compressed, sizeof compressed, decompressed, sizeof decompressed);
	}
}