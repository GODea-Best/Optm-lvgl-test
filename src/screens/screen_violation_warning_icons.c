#include "lvgl/lvgl.h"
#include "screen_violation_warning_icons.h"
#include "device_config.h"
#include "gradient_bg.h"
#include "accent_bar.h"
#include "lock_solid_svg_data.h"
#include "fonts_bold.h"

/* Violation Warning — icon version
 * Red gradient bg, lock icon conveys lockout, date and days kept as data. */

void screen_violation_warning_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    gradient_bg_attach(scr, lv_color_hex(0xCA0000), lv_color_hex(0x740B0B), 15);

    /* Lock icon */
    lv_obj_t * img = lv_image_create(scr);
    lv_obj_set_size(img, 52, 52);
    lv_image_set_src(img, &lock_solid_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 12);

    /* Date */
    lv_obj_t * lbl_date = lv_label_create(scr);
    lv_label_set_text(lbl_date, CFG_VIOLATION_DATE);
    lv_obj_set_style_text_color(lbl_date, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_date, &lv_font_montserrat_semibold_16, LV_PART_MAIN);
    lv_obj_align(lbl_date, LV_ALIGN_TOP_MID, 0, 72);

    /* Days remain */
    lv_obj_t * lbl_days = lv_label_create(scr);
    lv_label_set_text(lbl_days, CFG_VIOLATION_DAYS);
    lv_obj_set_style_text_color(lbl_days, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_days, &lv_font_montserrat_regular_12, LV_PART_MAIN);
    lv_obj_align(lbl_days, LV_ALIGN_TOP_MID, 0, 96);

    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0x000000), lv_color_hex(0x000000));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
