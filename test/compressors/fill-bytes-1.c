#include "common/types.h"
#include "test/tools.h"
#define PAIRS 1
#define SIZE (2 * PAIRS)
int main(void) {
	const u8 byteA = 42;
	const u8 byteB = 43;
	u8 uncompressed[SIZE];
	for (u16 i = 0; i < SIZE; i += 2) {
		uncompressed[i] = byteA;
		uncompressed[i + 1] = byteB;
	}
	const u8 compressed[] = {
		64, byteA, byteB
	};
	testFillBytes(uncompressed, sizeof uncompressed, compressed, sizeof compressed, byteA, byteB);
}