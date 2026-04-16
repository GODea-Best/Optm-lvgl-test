#include "lvgl/lvgl.h"
#include "screen_initial_test.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "accent_bar.h"

void screen_initial_test_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);

    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* "BLOW EVENLY" */
    lv_obj_t * lbl_blow = lv_label_create(scr);
    lv_label_set_text(lbl_blow, "BLOW\nEVENLY");
    lv_obj_set_width(lbl_blow, 112);
    lv_obj_align(lbl_blow, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_color(lbl_blow, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_blow, &lv_font_montserrat_extrabold_24, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_blow, 0, LV_PART_MAIN);

    /* Instructional message */
    lv_obj_t * lbl_msg = lv_label_create(scr);
    lv_label_set_text(lbl_msg, "Breathe steadily\ninto the device");
    lv_obj_set_size(lbl_msg, 112, 32);
    lv_obj_align(lbl_msg, LV_ALIGN_TOP_MID, 0, 72);
    lv_obj_set_style_text_color(lbl_msg, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_msg, &lv_font_montserrat_light_12, LV_PART_MAIN);

    /* Animated accent bar */
    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0x1400CA), lv_color_hex(0x1400CA));

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
