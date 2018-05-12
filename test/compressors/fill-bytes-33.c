#include "test-tools.h"
#include "types.h"
int main(void) {
	const u16 pairs = 33;
	const u16 size = 2 * pairs;
	const u8 byteA = 50;
	const u8 byteB = 51;
	u8 uncompressed[size];
	for (u16 i = 0; i < size; i += 2) {
		uncompressed[i] = byteA;
		uncompressed[i + 1] = byteB;
	}
	const u8 compressed[] = {
		232, 32, byteA, byteB, END
	};
	return testFillBytes(uncompressed, sizeof uncompressed, compressed, sizeof compressed, byteA, byteB);
}