#include "compress.h"
#include "allocation.h"
#include "compression-mode.h"
#include "types.h"
size_t compress(const u8 *decompressed, u8 **compressed) {
	struct Allocation allocation = { compressed, 0 };
	*allocation.block = NULL;
	while (true) {
		return 0;
	}
}