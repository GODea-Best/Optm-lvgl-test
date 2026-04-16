#ifndef SCREEN_INITIAL_CARD_BOLD_H
#define SCREEN_INITIAL_CARD_BOLD_H

#include "lvgl/lvgl.h"

/**
 * Initial Test — Bold Card style
 *
 * Outer card : 128×128, solid black, border-radius 15
 * Text       : left-aligned "BLOW EVENLY" (ExtraBold 24) + instructional (Regular 14)
 * Bottom bar : 128×21 at y=107, #1400CA, animated shimmer pulse (left→right)
 *              Parent clip_corner provides the rounded bottom corners.
 */
void screen_initial_card_bold_create(lv_obj_t * old_scr);

#endif /* SCREEN_INITIAL_CARD_BOLD_H */
