#include "test-tools.h"
TEST(BUFFER(
	0, 1, 36, 3, 0, 7,
	134, 0, 0,
	134, 0, 0,
	END
), BUFFER(
	1, 3, 3, 3, 3, 3, 7,
	1, 3, 3, 3, 3, 3, 7,
	1, 3, 3, 3, 3, 3, 7
))