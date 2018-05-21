#include "common/types.h"
#include "test/tools.h"
int main(void) {
	const u8 size = 33;
	for (u16 seed = 0; seed <= UINT8_MAX; ++seed) {
		u8 uncompressed[size];
		for (u8 i = 0; i < size; ++i) {
			uncompressed[i] = seed + i;
		}
		const u8 compressed[] = {
			236, 32, seed
		};
		testFillIncrementalSequence(uncompressed, sizeof uncompressed, compressed, sizeof compressed, seed);
	}
}