#pragma once
#include "types.h"
#include "compression-mode.h"
#include <stdlib.h>
struct HeaderLayout {
	u8 headerBytes;
	u8 mask;
	u8 indexShift;
	u8 modeShift;
	u16 maxStorage;
};
struct HeaderLayout *makeLayout(size_t headerBits, size_t indexBits);
u8 makeFirstByte(struct HeaderLayout *layout, CompressionMode mode, u16 size);
struct HeaderLayout *makeSmallHeader(void);
struct HeaderLayout *makeLargeHeader(void);