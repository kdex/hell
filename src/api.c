#include "decompress.h"
#include "api.h"
const struct HALInterface HAL = {
	.decompress = decompress
};