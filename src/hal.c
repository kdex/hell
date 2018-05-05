#include "decompress.h"
#include "api.h"
#include "types.h"
#include <stdio.h>
int main(void) {
	/* TODO: Implement shell tool */
	u8 compressed[] = {
		0, 1, 36, 3, 0, 7,
		166, 0, 0,
		166, 0, 0,
		END
	};
	u8 result[] = { 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2 };
	u8 *decompressed = NULL;
	const size_t decompressedSize = HAL.decompress(compressed, &decompressed);
	free(decompressed);
}