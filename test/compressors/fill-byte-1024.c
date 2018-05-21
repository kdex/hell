#include "common/types.h"
#include "test/tools.h"
int main(void) {
	const u16 size = 1024;
	const u8 byte = 42;
	u8 uncompressed[size];
	for (u16 i = 0; i < size; ++i) {
		uncompressed[i] = byte;
	}
	const u8 compressed[] = {
		231, 255, byte
	};
	testFillByte(uncompressed, sizeof uncompressed, compressed, sizeof compressed, byte);
}