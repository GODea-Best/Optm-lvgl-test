#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include "lvgl/lvgl.h"

/*
 * Returns the largest font from 'fonts[0..n-1]' (ordered largest→smallest)
 * such that 'text' renders in a single line within max_w pixels.
 * Falls back to fonts[n-1] if nothing fits.
 */
static inline const lv_font_t * font_fit(const char * text, int32_t max_w,
                                          const lv_font_t * const * fonts, int n)
{
    for(int i = 0; i < n; i++) {
        lv_point_t sz;
        lv_text_get_size(&sz, text, fonts[i], 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        if(sz.x <= max_w) return fonts[i];
    }
    return fonts[n - 1];
}

#endif /* TEXT_UTILS_H */
