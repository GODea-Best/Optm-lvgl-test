#include "lvgl/lvgl.h"
#include "screen_violation_warning.h"
#include "gradient_bg.h"
#include "device_config.h"
#include "text_utils.h"
#include "fonts_bold.h"
#include "accent_bar.h"

void screen_violation_warning_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);

    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);

    gradient_bg_attach(scr,
                       lv_color_hex(0xCA0000),
                       lv_color_hex(0x740B0B),
                       15);

    /* Shaped black accent bar */
    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0x000000), lv_color_hex(0x000000));

    /* "VIOLATION LOCKOUT" */
    lv_obj_t * lbl_title = lv_label_create(scr);
    lv_label_set_text(lbl_title, "VIOLATION\nLOCKOUT");
    lv_obj_set_width(lbl_title, 112);
    lv_obj_align(lbl_title, LV_ALIGN_TOP_MID, 0, 19);
    lv_obj_set_style_text_color(lbl_title, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_extrabold_16, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_title, 4, LV_PART_MAIN);

    /* Date — auto-shrink if too wide */
    static const lv_font_t * const date_fonts[] = {
        &lv_font_montserrat_semibold_16, &lv_font_montserrat_regular_14, &lv_font_montserrat_regular_12,
    };
    lv_obj_t * lbl_date = lv_label_create(scr);
    lv_label_set_text(lbl_date, CFG_VIOLATION_DATE);
    lv_obj_set_width(lbl_date, 112);
    lv_obj_align(lbl_date, LV_ALIGN_TOP_MID, 0, 68);
    lv_obj_set_style_text_color(lbl_date, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_date,
        font_fit(CFG_VIOLATION_DATE, 112, date_fonts, 3), LV_PART_MAIN);

    /* Days remain */
    lv_obj_t * lbl_days = lv_label_create(scr);
    lv_label_set_text(lbl_days, CFG_VIOLATION_DAYS);
    lv_obj_set_width(lbl_days, 112);
    lv_obj_align(lbl_days, LV_ALIGN_TOP_MID, 0, 92);
    lv_obj_set_style_text_color(lbl_days, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_days, &lv_font_montserrat_regular_12, LV_PART_MAIN);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
