#include "lvgl/lvgl.h"
#include "screen_lockout_bold.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "bottom_bar.h"

void screen_lockout_bold_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(scr, true, LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t * lbl_title = lv_label_create(scr);
    lv_label_set_text(lbl_title, "LOCKOUT");
    lv_obj_set_size(lbl_title, 114, 60);
    lv_obj_set_pos(lbl_title, 7, 10);
    lv_obj_set_style_text_color(lbl_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_extrabold_24, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_title, 0, LV_PART_MAIN);

    lv_obj_t * lbl_msg = lv_label_create(scr);
    lv_label_set_text(lbl_msg, CFG_LOCKOUT_DESC);
    lv_obj_set_size(lbl_msg, 114, 32);
    lv_obj_set_pos(lbl_msg, 7, 72);
    lv_obj_set_style_text_color(lbl_msg, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_msg, &lv_font_montserrat_regular_14, LV_PART_MAIN);

    BOTTOM_BAR_CREATE(scr, lv_color_hex(0xCA0000));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
