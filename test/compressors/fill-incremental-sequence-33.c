#include "common/types.h"
#include "test/tools.h"
#define SIZE 33
int main(void) {
	for (u16 seed = 0; seed <= UINT8_MAX; ++seed) {
		u8 uncompressed[SIZE];
		for (u8 i = 0; i < SIZE; ++i) {
			uncompressed[i] = seed + i;
		}
		const u8 compressed[] = {
			236, 32, seed
		};
		testFillIncrementalSequence(uncompressed, sizeof uncompressed, compressed, sizeof compressed, seed);
	}
}