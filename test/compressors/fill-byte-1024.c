#include "common/types.h"
#include "test/tools.h"
#define SIZE 1024
int main(void) {
	const u8 byte = 42;
	u8 uncompressed[SIZE];
	for (u16 i = 0; i < SIZE; ++i) {
		uncompressed[i] = byte;
	}
	const u8 compressed[] = {
		231, 255, byte
	};
	testFillByte(uncompressed, sizeof uncompressed, compressed, sizeof compressed, byte);
}