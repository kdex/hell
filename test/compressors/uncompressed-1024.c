#include "test-tools.h"
#include "types.h"
int main(void) {
	const u16 size = 1024;
	u8 uncompressed[size];
	u8 compressed[2 + size];
	compressed[0] = 227;
	compressed[1] = 255;
	for (u16 i = 0; i < size; ++i) {
		uncompressed[i] = i;
		compressed[i + 2] = i;
	}
	return testUncompressed(uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}