#pragma once
#include "compression-mode.h"
#include "types.h"
#include <stdlib.h>
typedef struct {
	u8 size;
	u8 mask;
	u8 indexShift;
	u8 modeShift;
	u16 capacity;
} Header;
u8 makeFirstByte(Header *restrict header, CompressionMode mode, u16 size);
void initSmallHeader(Header *restrict header);
void initLargeHeader(Header *restrict header);