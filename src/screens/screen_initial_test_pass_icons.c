#include "lvgl/lvgl.h"
#include "screen_initial_test_pass_icons.h"
#include "device_config.h"
#include "accent_bar.h"

/* Initial Test Pass — icon version
 * Large green checkmark + BrAC value (kept as data). */

void screen_initial_test_pass_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Large green checkmark */
    lv_obj_t * lbl_ok = lv_label_create(scr);
    lv_label_set_text(lbl_ok, LV_SYMBOL_OK);
    lv_obj_set_style_text_color(lbl_ok, lv_color_hex(0x1D9101), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_ok, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_align(lbl_ok, LV_ALIGN_CENTER, 0, -18);

    /* BrAC value — kept as data */
    lv_obj_t * lbl_brac = lv_label_create(scr);
    lv_label_set_text(lbl_brac, CFG_BRAC_VALUE);
    lv_obj_set_style_text_color(lbl_brac, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_brac, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_align(lbl_brac, LV_ALIGN_CENTER, 0, 30);

    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0x1D9101), lv_color_hex(0x1D9101));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
