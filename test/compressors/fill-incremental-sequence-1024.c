#include "test-tools.h"
#include "types.h"
int main(void) {
	const u16 size = 1024;
	for (u16 seed = 0; seed <= UINT8_MAX; ++seed) {
		u8 uncompressed[size];
		for (u16 i = 0; i < size; ++i) {
			uncompressed[i] = seed + i;
		}
		const u8 compressed[] = {
			239, 255, seed, END
		};
		const int value = testFillIncrementalSequence(uncompressed, sizeof uncompressed, compressed, sizeof compressed, seed);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
}