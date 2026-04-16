#ifndef SCREEN_INITIAL_CARD_H
#define SCREEN_INITIAL_CARD_H

#include "lvgl/lvgl.h"

/**
 * Initial Test — Card-on-Card design
 *
 * Outer layer: 128×128, radial gradient black → #666666, radius 15
 * Inner card : 120×120 at (4,3), radial gradient #000270 → #363636, radius 14
 * Text       : "BLOW EVENLY" (ExtraBold 24) + instructional (Regular 14)
 * Bar        : animated accent bar at bottom
 */
void screen_initial_card_create(lv_obj_t * old_scr);

#endif /* SCREEN_INITIAL_CARD_H */
