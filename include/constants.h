#pragma once
#include "types.h"
const u8 END;
/* We're not using CHAR_BIT here, since HAL's format requires a byte size of exactly 8 */
const u8 BITS_IN_BYTE;
/* HAL always uses three bits to set the compression mode */
const u8 MODE_BITS;
/* Since Lempel-Ziv modes use two bytes to store the offset, this limits the maximum (uncompressed) file size to 2¹⁶ B */
const u32 MAX_PAYLOAD;