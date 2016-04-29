#include "Bitmap.h"
#include "bitmaps.h"

const static uint8_t __attribute__((progmem)) bitmapData[] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x18,
    0x10, 0x30, 0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf8, 0xf8, 0xf8,
    0xf8, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0x78,
    0x38, 0x18, 0x18, 0x8, 0x8, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    /* page 1 (lines 8-15) */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0xfe, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0x3, 0x7, 0x7, 0x1f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3, 0x0, 0x0, 0x0,
    0xe0, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7c, 0x0, 0x0, 0x0,
    0x0, 0x82, 0x86, 0x4, 0x8, 0x10, 0x0, 0x0, 0x0, 0x0,
    /* page 2 (lines 16-23) */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0,
    0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x0, 0x0, 0x0, 0x80,
    0xc1, 0xc1, 0xc1, 0xc1, 0x1, 0x1, 0x1, 0x0, 0x20, 0x60, 0xf0, 0xf8, 0xfc,
    0xff, 0xff, 0xff, 0x7f, 0x3e, 0x0, 0x0, 0x0, 0x0, 0x0,
    /* page 3 (lines 24-31) */
    0x0, 0x0, 0x10, 0x10, 0x18, 0x1c, 0x1f, 0x1f, 0x1f, 0xf, 0xf, 0xf, 0xf, 0x7,
    0x7, 0xc, 0x8, 0x0, 0x0, 0xb, 0xf, 0x1f, 0x1f, 0x1f, 0x1e, 0x1c, 0x8, 0x18,
    0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x8, 0x4, 0x6, 0x3, 0x3, 0x7, 0x7,
    0xf, 0xf, 0xe, 0x1c, 0x18, 0x18, 0x10, 0x10, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const Bitmap nupoLogo(bitmapData, 63, 32);
