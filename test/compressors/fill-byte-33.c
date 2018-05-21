#include "common/types.h"
#include "test/tools.h"
int main(void) {
	const u16 size = 33;
	const u8 byte = 42;
	u8 uncompressed[size];
	for (u16 i = 0; i < size; ++i) {
		uncompressed[i] = byte;
	}
	const u8 compressed[] = {
		228, size - 1, byte
	};
	testFillByte(uncompressed, sizeof uncompressed, compressed, sizeof compressed, byte);
}