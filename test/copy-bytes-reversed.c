#include "test-tools.h"
TEST(BUFFER(
	0, 1, 36, 3, 0, 7,
	198, 6, 0,
	198, 6, 0,
	END
), BUFFER(
	1, 3, 3, 3, 3, 3, 7,
	7, 3, 3, 3, 3, 3, 1,
	7, 3, 3, 3, 3, 3, 1
))