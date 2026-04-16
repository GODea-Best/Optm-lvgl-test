/*******************************************************************************
 * Size: 7 px
 * Bpp: 4
 * Opts: --font fonts_src/package/files/montserrat-300.ttf --size 7 --bpp 4 --format lvgl -r 0x20-0x7F --no-compress --no-prefilter --lv-font-name lv_font_montserrat_light_7 -o lv_font_montserrat_light_7.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_MONTSERRAT_LIGHT_7
#define LV_FONT_MONTSERRAT_LIGHT_7 1
#endif

#if LV_FONT_MONTSERRAT_LIGHT_7

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x51, 0x40, 0x40, 0x20, 0x30,

    /* U+0022 "\"" */
    0x44, 0x44,

    /* U+0023 "#" */
    0x4, 0x4, 0x2, 0x94, 0x92, 0x4, 0x4, 0x3,
    0x95, 0x91, 0x4, 0x3, 0x0,

    /* U+0024 "$" */
    0x0, 0x30, 0x26, 0x93, 0x50, 0x40, 0x5, 0x92,
    0x0, 0x46, 0x35, 0x95, 0x0, 0x30,

    /* U+0025 "%" */
    0x33, 0x21, 0x30, 0x40, 0x45, 0x0, 0x24, 0x43,
    0x40, 0x1, 0x34, 0x4, 0x5, 0x4, 0x42,

    /* U+0026 "&" */
    0x6, 0x61, 0x0, 0x54, 0x10, 0x19, 0x40, 0x5,
    0x4, 0x60, 0x35, 0x59, 0x10, 0x0, 0x2,

    /* U+0027 "'" */
    0x44,

    /* U+0028 "(" */
    0x5, 0x5, 0x13, 0x32, 0x13, 0x5, 0x5,

    /* U+0029 ")" */
    0x50, 0x23, 0x5, 0x5, 0x5, 0x23, 0x50,

    /* U+002A "*" */
    0x24, 0x14, 0xa1, 0x2, 0x0,

    /* U+002B "+" */
    0x0, 0x0, 0x2, 0x20, 0x28, 0x72, 0x2, 0x20,
    0x0, 0x0,

    /* U+002C "," */
    0x44, 0x0,

    /* U+002D "-" */
    0x25, 0x10,

    /* U+002E "." */
    0x40,

    /* U+002F "/" */
    0x0, 0x4, 0x0, 0x40, 0x0, 0x50, 0x0, 0x40,
    0x4, 0x0, 0x5, 0x0, 0x4, 0x0,

    /* U+0030 "0" */
    0x7, 0x55, 0x5, 0x0, 0x50, 0x50, 0x2, 0x25,
    0x0, 0x50, 0x17, 0x65, 0x0,

    /* U+0031 "1" */
    0x57, 0x5, 0x5, 0x5, 0x5,

    /* U+0032 "2" */
    0x45, 0x61, 0x0, 0x5, 0x0, 0x50, 0x5, 0x10,
    0x77, 0x53,

    /* U+0033 "3" */
    0x45, 0x82, 0x0, 0x50, 0x5, 0x50, 0x0, 0x5,
    0x45, 0x52,

    /* U+0034 "4" */
    0x0, 0x51, 0x0, 0x33, 0x0, 0x14, 0x4, 0x6,
    0x55, 0xb2, 0x0, 0x5, 0x0,

    /* U+0035 "5" */
    0x46, 0x51, 0x50, 0x0, 0x45, 0x51, 0x0, 0x5,
    0x45, 0x53,

    /* U+0036 "6" */
    0x6, 0x52, 0x54, 0x42, 0x72, 0x5, 0x60, 0x5,
    0x17, 0x55,

    /* U+0037 "7" */
    0x85, 0x58, 0x20, 0x33, 0x0, 0x60, 0x1, 0x40,
    0x6, 0x0,

    /* U+0038 "8" */
    0x25, 0x54, 0x5, 0x0, 0x50, 0x29, 0x86, 0x5,
    0x0, 0x50, 0x35, 0x55, 0x0,

    /* U+0039 "9" */
    0x45, 0x52, 0x50, 0x7, 0x25, 0x56, 0x0, 0x6,
    0x25, 0x62,

    /* U+003A ":" */
    0x40, 0x4,

    /* U+003B ";" */
    0x40, 0x4, 0x40,

    /* U+003C "<" */
    0x0, 0x0, 0x4, 0x52, 0x46, 0x0, 0x0, 0x52,

    /* U+003D "=" */
    0x25, 0x52, 0x0, 0x0, 0x25, 0x52,

    /* U+003E ">" */
    0x0, 0x0, 0x25, 0x30, 0x0, 0x64, 0x25, 0x0,

    /* U+003F "?" */
    0x45, 0x61, 0x0, 0x14, 0x0, 0x60, 0x2, 0x0,
    0x3, 0x0,

    /* U+0040 "@" */
    0x3, 0x54, 0x44, 0x2, 0x25, 0x44, 0x64, 0x41,
    0x30, 0x16, 0x44, 0x13, 0x1, 0x64, 0x22, 0x55,
    0x56, 0x40, 0x34, 0x43, 0x0,

    /* U+0041 "A" */
    0x0, 0x90, 0x0, 0x51, 0x40, 0x5, 0x5, 0x4,
    0x65, 0x73, 0x60, 0x0, 0x60,

    /* U+0042 "B" */
    0x27, 0x55, 0x32, 0x30, 0x5, 0x27, 0x57, 0x32,
    0x30, 0x5, 0x27, 0x55, 0x50,

    /* U+0043 "C" */
    0x6, 0x55, 0x34, 0x10, 0x0, 0x50, 0x0, 0x4,
    0x10, 0x0, 0x6, 0x55, 0x30,

    /* U+0044 "D" */
    0x27, 0x55, 0x50, 0x23, 0x0, 0x32, 0x23, 0x0,
    0x5, 0x23, 0x0, 0x32, 0x27, 0x55, 0x50,

    /* U+0045 "E" */
    0x27, 0x55, 0x2, 0x30, 0x0, 0x28, 0x54, 0x2,
    0x30, 0x0, 0x27, 0x55, 0x10,

    /* U+0046 "F" */
    0x27, 0x55, 0x2, 0x30, 0x0, 0x28, 0x54, 0x2,
    0x30, 0x0, 0x23, 0x0, 0x0,

    /* U+0047 "G" */
    0x6, 0x55, 0x34, 0x10, 0x0, 0x50, 0x0, 0x24,
    0x10, 0x5, 0x6, 0x55, 0x50,

    /* U+0048 "H" */
    0x23, 0x0, 0x52, 0x30, 0x5, 0x28, 0x55, 0x82,
    0x30, 0x5, 0x23, 0x0, 0x50,

    /* U+0049 "I" */
    0x23, 0x23, 0x23, 0x23, 0x23,

    /* U+004A "J" */
    0x25, 0x70, 0x5, 0x0, 0x50, 0x5, 0x55, 0x40,

    /* U+004B "K" */
    0x23, 0x3, 0x32, 0x33, 0x30, 0x26, 0x90, 0x2,
    0x70, 0x60, 0x23, 0x1, 0x50,

    /* U+004C "L" */
    0x23, 0x0, 0x2, 0x30, 0x0, 0x23, 0x0, 0x2,
    0x30, 0x0, 0x27, 0x55, 0x0,

    /* U+004D "M" */
    0x26, 0x0, 0x8, 0x28, 0x10, 0x47, 0x23, 0x50,
    0x55, 0x23, 0x37, 0x5, 0x23, 0x2, 0x5,

    /* U+004E "N" */
    0x28, 0x0, 0x52, 0x64, 0x5, 0x23, 0x42, 0x52,
    0x30, 0x66, 0x23, 0x0, 0x90,

    /* U+004F "O" */
    0x6, 0x55, 0x50, 0x41, 0x0, 0x33, 0x50, 0x0,
    0x5, 0x41, 0x0, 0x23, 0x6, 0x55, 0x50,

    /* U+0050 "P" */
    0x28, 0x55, 0x12, 0x30, 0x5, 0x23, 0x0, 0x52,
    0x85, 0x50, 0x23, 0x0, 0x0,

    /* U+0051 "Q" */
    0x6, 0x55, 0x50, 0x41, 0x0, 0x33, 0x50, 0x0,
    0x5, 0x41, 0x0, 0x23, 0x6, 0x56, 0x50, 0x0,
    0x4, 0x42,

    /* U+0052 "R" */
    0x28, 0x56, 0x12, 0x30, 0x5, 0x23, 0x0, 0x52,
    0x85, 0x80, 0x23, 0x2, 0x30,

    /* U+0053 "S" */
    0x26, 0x53, 0x50, 0x0, 0x5, 0x52, 0x0, 0x6,
    0x35, 0x55,

    /* U+0054 "T" */
    0x57, 0x74, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20,
    0x3, 0x20,

    /* U+0055 "U" */
    0x32, 0x0, 0x53, 0x20, 0x5, 0x22, 0x0, 0x51,
    0x40, 0x5, 0x5, 0x56, 0x20,

    /* U+0056 "V" */
    0x60, 0x0, 0x54, 0x10, 0x41, 0x6, 0x6, 0x0,
    0x52, 0x30, 0x0, 0xa0, 0x0,

    /* U+0057 "W" */
    0x50, 0x6, 0x30, 0x13, 0x32, 0x5, 0x50, 0x50,
    0x5, 0x22, 0x50, 0x50, 0x5, 0x50, 0x25, 0x30,
    0x3, 0x70, 0xa, 0x0,

    /* U+0058 "X" */
    0x41, 0x6, 0x0, 0x64, 0x20, 0x2, 0x90, 0x0,
    0x63, 0x30, 0x50, 0x5, 0x10,

    /* U+0059 "Y" */
    0x60, 0x4, 0x11, 0x40, 0x50, 0x5, 0x60, 0x0,
    0x6, 0x0, 0x0, 0x50, 0x0,

    /* U+005A "Z" */
    0x35, 0x5a, 0x10, 0x3, 0x30, 0x1, 0x50, 0x0,
    0x60, 0x0, 0x76, 0x55, 0x10,

    /* U+005B "[" */
    0x27, 0x23, 0x23, 0x23, 0x23, 0x23, 0x27,

    /* U+005C "\\" */
    0x13, 0x0, 0x5, 0x0, 0x5, 0x0, 0x1, 0x30,
    0x0, 0x50, 0x0, 0x50, 0x0, 0x13,

    /* U+005D "]" */
    0x54, 0x14, 0x14, 0x14, 0x14, 0x14, 0x54,

    /* U+005E "^" */
    0x4, 0x30, 0x4, 0x40, 0x22, 0x22,

    /* U+005F "_" */
    0x44, 0x42,

    /* U+0060 "`" */
    0x51,

    /* U+0061 "a" */
    0x15, 0x61, 0x14, 0x45, 0x60, 0x5, 0x34, 0x55,

    /* U+0062 "b" */
    0x41, 0x0, 0x4, 0x10, 0x0, 0x46, 0x56, 0x5,
    0x40, 0x14, 0x53, 0x1, 0x44, 0x65, 0x60,

    /* U+0063 "c" */
    0x16, 0x53, 0x50, 0x0, 0x50, 0x0, 0x16, 0x53,

    /* U+0064 "d" */
    0x0, 0x5, 0x0, 0x5, 0x16, 0x58, 0x50, 0x8,
    0x50, 0x8, 0x16, 0x59,

    /* U+0065 "e" */
    0x16, 0x54, 0x74, 0x45, 0x50, 0x0, 0x16, 0x53,

    /* U+0066 "f" */
    0x7, 0x22, 0x20, 0x77, 0x13, 0x20, 0x32, 0x3,
    0x20,

    /* U+0067 "g" */
    0x16, 0x48, 0x5, 0x0, 0x80, 0x50, 0x8, 0x1,
    0x65, 0x80, 0x15, 0x55, 0x0,

    /* U+0068 "h" */
    0x41, 0x0, 0x4, 0x10, 0x0, 0x46, 0x55, 0x4,
    0x20, 0x50, 0x41, 0x5, 0x4, 0x10, 0x50,

    /* U+0069 "i" */
    0x21, 0x0, 0x41, 0x41, 0x41, 0x41,

    /* U+006A "j" */
    0x2, 0x10, 0x0, 0x3, 0x20, 0x32, 0x3, 0x20,
    0x31, 0x37, 0x0,

    /* U+006B "k" */
    0x41, 0x0, 0x4, 0x10, 0x0, 0x41, 0x24, 0x4,
    0x47, 0x0, 0x46, 0x60, 0x4, 0x10, 0x60,

    /* U+006C "l" */
    0x41, 0x41, 0x41, 0x41, 0x41, 0x41,

    /* U+006D "m" */
    0x45, 0x55, 0x55, 0x44, 0x20, 0x70, 0x5, 0x41,
    0x5, 0x0, 0x54, 0x10, 0x50, 0x5,

    /* U+006E "n" */
    0x46, 0x55, 0x4, 0x20, 0x50, 0x41, 0x5, 0x4,
    0x10, 0x50,

    /* U+006F "o" */
    0x16, 0x64, 0x50, 0x5, 0x50, 0x5, 0x16, 0x54,

    /* U+0070 "p" */
    0x46, 0x56, 0x5, 0x30, 0x14, 0x53, 0x1, 0x44,
    0x65, 0x60, 0x41, 0x0, 0x0,

    /* U+0071 "q" */
    0x16, 0x58, 0x50, 0x9, 0x50, 0x8, 0x16, 0x58,
    0x0, 0x5,

    /* U+0072 "r" */
    0x45, 0x24, 0x20, 0x41, 0x4, 0x10,

    /* U+0073 "s" */
    0x45, 0x50, 0x53, 0x0, 0x2, 0x70, 0x45, 0x60,

    /* U+0074 "t" */
    0x22, 0x7, 0x71, 0x32, 0x2, 0x20, 0x7, 0x20,

    /* U+0075 "u" */
    0x40, 0x5, 0x40, 0x5, 0x41, 0x6, 0x6, 0x58,

    /* U+0076 "v" */
    0x50, 0x5, 0x50, 0x41, 0x5, 0x50, 0x6, 0x30,

    /* U+0077 "w" */
    0x50, 0x44, 0x5, 0x50, 0x55, 0x5, 0x14, 0x44,
    0x41, 0x8, 0x0, 0x90,

    /* U+0078 "x" */
    0x51, 0x41, 0x7, 0x40, 0x7, 0x50, 0x50, 0x42,

    /* U+0079 "y" */
    0x5, 0x0, 0x50, 0x40, 0x40, 0x0, 0x55, 0x0,
    0x9, 0x10, 0x5, 0x50, 0x0,

    /* U+007A "z" */
    0x35, 0x91, 0x2, 0x40, 0x6, 0x0, 0x75, 0x51,

    /* U+007B "{" */
    0x7, 0x0, 0x50, 0x5, 0x3, 0x40, 0x5, 0x0,
    0x50, 0x7, 0x0,

    /* U+007C "|" */
    0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23,

    /* U+007D "}" */
    0x51, 0x14, 0x14, 0x7, 0x14, 0x14, 0x51,

    /* U+007E "~" */
    0x25, 0x2, 0x20, 0x52
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 29, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 28, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 40, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 7, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 68, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 34, .adv_w = 92, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 73, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 64, .adv_w = 22, .box_w = 1, .box_h = 2, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 65, .adv_w = 36, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 72, .adv_w = 36, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 79, .adv_w = 42, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 84, .adv_w = 64, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 22, .box_w = 1, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 96, .adv_w = 43, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 98, .adv_w = 22, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 99, .adv_w = 36, .box_w = 4, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 113, .adv_w = 74, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 40, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 63, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 62, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 73, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 62, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 174, .adv_w = 67, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 184, .adv_w = 65, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 71, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 67, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 217, .adv_w = 22, .box_w = 1, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 22, .box_w = 1, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 222, .adv_w = 64, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 230, .adv_w = 64, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 236, .adv_w = 64, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 244, .adv_w = 63, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 116, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 275, .adv_w = 79, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 288, .adv_w = 84, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 301, .adv_w = 79, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 93, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 75, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 342, .adv_w = 71, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 87, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 91, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 381, .adv_w = 33, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 386, .adv_w = 55, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 79, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 66, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 107, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 435, .adv_w = 91, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 94, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 94, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 494, .adv_w = 81, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 68, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 517, .adv_w = 63, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 527, .adv_w = 89, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 540, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 123, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 72, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 586, .adv_w = 70, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 599, .adv_w = 72, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 612, .adv_w = 34, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 619, .adv_w = 36, .box_w = 4, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 633, .adv_w = 34, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 640, .adv_w = 64, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 646, .adv_w = 56, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 648, .adv_w = 67, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 649, .adv_w = 65, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 657, .adv_w = 76, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 672, .adv_w = 62, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 680, .adv_w = 76, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 692, .adv_w = 67, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 700, .adv_w = 37, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 709, .adv_w = 76, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 722, .adv_w = 75, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 737, .adv_w = 29, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 743, .adv_w = 30, .box_w = 3, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 754, .adv_w = 65, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 769, .adv_w = 29, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 119, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 789, .adv_w = 75, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 799, .adv_w = 69, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 807, .adv_w = 76, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 820, .adv_w = 76, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 830, .adv_w = 44, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 836, .adv_w = 54, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 45, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 852, .adv_w = 75, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 860, .adv_w = 59, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 868, .adv_w = 97, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 58, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 888, .adv_w = 59, .box_w = 5, .box_h = 5, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 901, .adv_w = 56, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 909, .adv_w = 36, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 920, .adv_w = 33, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 927, .adv_w = 36, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 934, .adv_w = 64, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_montserrat_light_7 = {
#else
lv_font_t lv_font_montserrat_light_7 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 8,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_FONT_MONTSERRAT_LIGHT_7*/

