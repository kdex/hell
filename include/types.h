#pragma once
#include <stdint.h>
#define END 0xff
/* We're not using CHAR_BIT here, since HAL's format requires a byte size of exactly 8 */
#define BITS_IN_BYTE 8
/* HAL always uses three bits to set the compression mode. */
#define MODE_BITS 3
typedef uint8_t u8;
typedef uint16_t u16;