#include "lvgl/lvgl.h"
#include "screen_initial_test_pass.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "accent_bar.h"

void screen_initial_test_pass_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Shaped green accent bar */
    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0x1D9101), lv_color_hex(0x1D9101));

    /* "TEST PASSED" */
    lv_obj_t * lbl_head = lv_label_create(scr);
    lv_label_set_text(lbl_head, "TEST\nPASSED");
    lv_obj_set_width(lbl_head, 112);
    lv_obj_align(lbl_head, LV_ALIGN_TOP_MID, 0, 14);
    lv_obj_set_style_text_color(lbl_head, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_head, &lv_font_montserrat_extrabold_24, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_head, 6, LV_PART_MAIN);

    /* "BrAC" — small label */
    lv_obj_t * lbl_brac = lv_label_create(scr);
    lv_label_set_text(lbl_brac, "BrAC");
    lv_obj_set_width(lbl_brac, 112);
    lv_obj_align(lbl_brac, LV_ALIGN_TOP_MID, 0, 72);
    lv_obj_set_style_text_color(lbl_brac, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_brac, &lv_font_montserrat_regular_12, LV_PART_MAIN);

    /* Result value — large */
    lv_obj_t * lbl_value = lv_label_create(scr);
    lv_label_set_text(lbl_value, CFG_BRAC_VALUE);
    lv_obj_set_width(lbl_value, 112);
    lv_obj_align(lbl_value, LV_ALIGN_TOP_MID, 0, 86);
    lv_obj_set_style_text_color(lbl_value, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_value, &lv_font_montserrat_bold_22, LV_PART_MAIN);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
