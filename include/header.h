#pragma once
#include "compression-mode.h"
#include "types.h"
#include <stdlib.h>
typedef struct {
	u8 size;
	u8 mask;
	u8 indexShift;
	u8 modeShift;
	u16 maxStorage;
} Header;
u8 makeFirstByte(Header *layout, CompressionMode mode, u16 size);
void initSmallHeader(Header *header);
void initLargeHeader(Header *header);