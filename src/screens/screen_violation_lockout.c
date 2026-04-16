#include "lvgl/lvgl.h"
#include "screen_violation_lockout.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "accent_bar.h"
#include "triangle_exclamation_svg_data.h"

void screen_violation_lockout_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Top shaped red bar (positioned near top edge) */
    lv_obj_t * bar_top = lv_obj_create(scr);
    lv_obj_set_size(bar_top, 113, 9);
    lv_obj_set_pos(bar_top, 7, 6);
    accent_bar_attach(bar_top, false, lv_color_hex(0xCA0000), lv_color_hex(0xCA0000));

    /* Bottom shaped red bar */
    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0xCA0000), lv_color_hex(0xCA0000));

    /* "VIOLATION" — ExtraBold 18px → Bold 18 */
    lv_obj_t * lbl_title = lv_label_create(scr);
    lv_label_set_text(lbl_title, "VIOLATION");
    lv_obj_set_width(lbl_title, 112);
    lv_obj_align(lbl_title, LV_ALIGN_TOP_MID, 0, 19);
    lv_obj_set_style_text_color(lbl_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_extrabold_18, LV_PART_MAIN);

    /* Warning icon — triangle-exclamation SVG, 36×36, centred */
    lv_obj_t * img_icon = lv_image_create(scr);
    lv_obj_set_size(img_icon, 36, 36);
    lv_image_set_src(img_icon, &triangle_exclamation_svg_dsc);
    lv_image_set_align(img_icon, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img_icon, LV_ALIGN_CENTER, 0, -5);

    /* Description — Regular 12, centred */
    lv_obj_t * lbl_desc = lv_label_create(scr);
    lv_label_set_text(lbl_desc, CFG_LOCKOUT_DESC);
    lv_obj_set_size(lbl_desc, 112, 30);
    lv_obj_align(lbl_desc, LV_ALIGN_TOP_MID, 0, 81);
    lv_obj_set_style_text_color(lbl_desc, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_desc, &lv_font_montserrat_regular_12, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_desc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
