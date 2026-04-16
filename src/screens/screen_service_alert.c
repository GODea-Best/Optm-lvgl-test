#include "lvgl/lvgl.h"
#include "screen_service_alert.h"
#include "device_config.h"
#include "text_utils.h"
#include "fonts_bold.h"
#include "accent_bar.h"

void screen_service_alert_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);

    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Black card */
    lv_obj_t * card = lv_obj_create(scr);
    lv_obj_set_size(card, 126, 126);
    lv_obj_set_pos(card, 1, 1);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(card, 14, LV_PART_MAIN);
    lv_obj_set_style_border_width(card, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(card, 0, LV_PART_MAIN);

    /* Shaped cyan accent bar */
    ACCENT_BAR_CREATE(card, false, lv_color_hex(0x00D0FF), lv_color_hex(0x00D0FF));

    /* "SERVICE DUE BY" */
    lv_obj_t * lbl_title = lv_label_create(card);
    lv_label_set_text(lbl_title, "SERVICE\nDUE BY");
    lv_obj_set_width(lbl_title, 110);
    lv_obj_align(lbl_title, LV_ALIGN_TOP_MID, 0, 14);
    lv_obj_set_style_text_color(lbl_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_extrabold_22, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_title, 0, LV_PART_MAIN);

    /* Date — auto-shrink if too wide */
    static const lv_font_t * const date_fonts[] = {
        &lv_font_montserrat_semibold_22, &lv_font_montserrat_semibold_20, &lv_font_montserrat_semibold_16,
        &lv_font_montserrat_regular_14,  &lv_font_montserrat_regular_12,
    };
    lv_obj_t * lbl_date = lv_label_create(card);
    lv_label_set_text(lbl_date, CFG_SERVICE_ALERT_DATE);
    lv_obj_set_width(lbl_date, 110);
    lv_obj_align(lbl_date, LV_ALIGN_TOP_MID, 0, 63);
    lv_obj_set_style_text_color(lbl_date, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_date,
        font_fit(CFG_SERVICE_ALERT_DATE, 110, date_fonts, 5), LV_PART_MAIN);

    /* Days remain */
    lv_obj_t * lbl_days = lv_label_create(card);
    lv_label_set_text(lbl_days, CFG_SERVICE_ALERT_DAYS);
    lv_obj_set_width(lbl_days, 110);
    lv_obj_align(lbl_days, LV_ALIGN_TOP_MID, 0, 91);
    lv_obj_set_style_text_color(lbl_days, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_days, &lv_font_montserrat_regular_12, LV_PART_MAIN);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
