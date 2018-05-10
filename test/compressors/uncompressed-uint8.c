#include "test-tools.h"
#include "types.h"
int main(void) {
	int value;
	/* Payload size == 1 */
	for (u16 i = 0; i <= UINT8_MAX; ++i) {
		const u8 decompressed[] = {
			i
		};
		const u8 compressed[] = {
			0, i, END
		};
		value = testUncompressedU8(decompressed, sizeof decompressed, compressed, sizeof compressed);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
	/* Payload size == 32 */
	const u8 decompressed32[] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		END, 32
	};
	const u8 compressed32[] = {
		31,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		END, 32,
		END
	};
	value = testUncompressedU8(decompressed32, sizeof decompressed32, compressed32, sizeof compressed32);
	if (value == EXIT_FAILURE) {
		return value;
	}
	/* Payload size == 33 */
	const u8 decompressed33[] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		END, 32, END
	};
	const u8 compressed33[] = {
		224, 32,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		END, 32, END,
		END
	};
	value = testUncompressedU8(decompressed33, sizeof decompressed33, compressed33, sizeof compressed33);
	if (value == EXIT_FAILURE) {
		return value;
	}
	/* Payload size == 1024 */
	u8 decompressed1024[1024];
	u8 compressed1024[2 + 1024 + 1];
	compressed1024[0] = 227;
	compressed1024[1] = 255;
	compressed1024[1026] = END;
	for (u16 i = 0; i < 1024; ++i) {
		decompressed1024[i] = i;
		compressed1024[i + 2] = i;
	}
	value = testUncompressedU8(decompressed1024, sizeof decompressed1024, compressed1024, sizeof compressed1024);
	if (value == EXIT_FAILURE) {
		return value;
	}
}