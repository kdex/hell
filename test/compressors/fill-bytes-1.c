#include "test-tools.h"
#include "types.h"
int main(void) {
	const u16 pairs = 1;
	const u16 size = 2 * pairs;
	const u8 byteA = 42;
	const u8 byteB = 43;
	u8 uncompressed[size];
	for (u16 i = 0; i < size; i += 2) {
		uncompressed[i] = byteA;
		uncompressed[i + 1] = byteB;
	}
	const u8 compressed[] = {
		64, byteA, byteB
	};
	testFillBytes(uncompressed, sizeof uncompressed, compressed, sizeof compressed, byteA, byteB);
}