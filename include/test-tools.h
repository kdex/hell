#include "types.h"
#include <stdlib.h>
#define BUFFER(...) __VA_ARGS__
#define TEST(COMPRESSED, EXPECTED)\
	int main(void) {\
		const u8 decompressedExpected[] = { EXPECTED };\
		u8 compressed[] = { COMPRESSED };\
		return compare(compressed, decompressedExpected, sizeof decompressedExpected);\
	}
int compare(const u8 *compressed, const u8 *decompressedExpected, size_t decompressedSizeExpected);
int fail(const char *format, ...);