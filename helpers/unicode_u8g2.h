#pragma once

#include <stdint.h>
#include <u8g2/u8g2.h>

#define unicode_pgm_read(adr) (*(const uint8_t*)(adr))
#define UNICODE_FONT_DATA_STRUCT_SIZE 23
