#include "common/types.h"
#include "test/tools.h"
#define SIZE 1024
int main(void) {
	u8 uncompressed[SIZE];
	u8 compressed[2 + SIZE];
	compressed[0] = 227;
	compressed[1] = 255;
	for (u16 i = 0; i < SIZE; ++i) {
		uncompressed[i] = i;
		compressed[i + 2] = i;
	}
	testUncompressed(uncompressed, sizeof uncompressed, compressed, sizeof compressed);
}