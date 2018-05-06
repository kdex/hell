#include "test-tools.h"
#include "types.h"
int main() {
	u8 sequence[UINT8_MAX + 1];
	for (size_t i = 0; i <= UINT8_MAX; ++i) {
		sequence[i] = i;
	}
	u8 compressed[] = {
		236, 255, 0, END
	};
	return compare(compressed, sequence, sizeof sequence);
}