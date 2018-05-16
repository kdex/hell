#include "test-tools.h"
#include "types.h"
int main(void) {
	for (u16 seed = 0; seed <= UINT8_MAX; ++seed) {
		const u8 uncompressed[] = { seed };
		const u8 compressed[] = {
			96, seed
		};
		const int value = testFillIncrementalSequence(uncompressed, sizeof uncompressed, compressed, sizeof compressed, seed);
		if (value == EXIT_FAILURE) {
			return value;
		}
	}
}