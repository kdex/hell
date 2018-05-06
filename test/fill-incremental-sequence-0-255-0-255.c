#include "test-tools.h"
#include "types.h"
int main() {
	const size_t elements = 2 * (UINT8_MAX + 1);
	u8 sequence[elements];
	for (size_t i = 0; i < elements; ++i) {
		sequence[i] = i;
	}
	u8 compressed[] = {
		236, 255, 0,
		236, 255, 0,
		END
	};
	return compare(compressed, sequence, sizeof sequence);
}