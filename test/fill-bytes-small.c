#include "test-tools.h"
TEST(BUFFER(
	70, 1, 2, END
), BUFFER(
	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	1, 2, 1, 2
))