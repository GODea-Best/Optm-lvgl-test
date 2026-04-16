#include "lvgl/lvgl.h"
#include "screen_test_error_icons.h"
#include "device_config.h"
#include "accent_bar.h"

/* Test Error — icon version
 * X mark (fail) + refresh arrow (try again). No text. */

void screen_test_error_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Large X — sample failed */
    lv_obj_t * lbl_x = lv_label_create(scr);
    lv_label_set_text(lbl_x, LV_SYMBOL_CLOSE);
    lv_obj_set_style_text_color(lbl_x, lv_color_hex(0xFF9D00), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_x, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_align(lbl_x, LV_ALIGN_CENTER, 0, -22);

    /* Refresh arrow — try again */
    lv_obj_t * lbl_ref = lv_label_create(scr);
    lv_label_set_text(lbl_ref, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_color(lbl_ref, lv_color_hex(0x1400CA), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_ref, &lv_font_montserrat_36, LV_PART_MAIN);
    lv_obj_align(lbl_ref, LV_ALIGN_CENTER, 0, 28);

    ACCENT_BAR_CREATE(scr, true, lv_color_hex(0x1400CA), lv_color_hex(0xFF9D00));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
