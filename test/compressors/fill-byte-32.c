#include "test-tools.h"
#include "types.h"
int main(void) {
	const u16 size = 32;
	const u8 byte = 42;
	u8 uncompressed[size];
	for (u16 i = 0; i < size; ++i) {
		uncompressed[i] = byte;
	}
	const u8 compressed[] = {
		63, byte
	};
	return testFillByte(uncompressed, sizeof uncompressed, compressed, sizeof compressed, byte);
}