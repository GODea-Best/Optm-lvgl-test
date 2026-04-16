#include "lvgl/lvgl.h"
#include "screen_warning_msg.h"
#include "device_config.h"
#include "fonts_bold.h"

/*
 * Warning Message screen
 *
 * Orange background (#FF4400) with 3px orange border (#FF9D00), radius 15.
 * Full-screen legal warning text — Regular 10px, centered, white.
 */

#define WARNING_TEXT \
    "Any person tampering, circumventing, or otherwise misusing " \
    "this device is subject to a fine, imprisonment, or both and " \
    "may be subject to civil liability."

void screen_warning_msg_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFF4400), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_border_color(scr, lv_color_hex(0xFF9D00), LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 3, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);

    /* Legal warning text — 121×104 at left=4, top=12 */
    lv_obj_t * lbl = lv_label_create(scr);
    lv_label_set_text(lbl, WARNING_TEXT);
    lv_obj_set_size(lbl, 121, 104);
    lv_obj_set_pos(lbl, 4, 12);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_regular_10, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl, 5, LV_PART_MAIN); /* ~150% of 10px */
    lv_obj_set_style_text_letter_space(lbl, -1, LV_PART_MAIN); /* letter-spacing: -0.011em */

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
