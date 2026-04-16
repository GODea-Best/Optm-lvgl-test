#include "lvgl/lvgl.h"
#include "screen_tamper_warn.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "bottom_bar.h"
#include "accent_bar.h"

void screen_tamper_warn_create(lv_obj_t * old_scr)
{
    /* Black screen — fills the display; shows as the border colour behind
     * the card's rounded corners instead of the display's white default. */
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Gradient card — orange→yellow, r=15 clips the corners cleanly */
    lv_obj_t * card = lv_obj_create(scr);
    lv_obj_set_size(card, 128, 128);
    lv_obj_set_pos(card, 0, 0);
    lv_obj_set_style_pad_all(card, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(card, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(card, 15, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(card, true, LV_PART_MAIN);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xFF9D00), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(card, lv_color_hex(0xFFD900), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_VER, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    /* Title — ExtraBold 18, black, explicit newline, no auto-wrap */
    lv_obj_t * lbl_title = lv_label_create(card);
    lv_label_set_text(lbl_title, "TAMPER\nDETECTED");
    lv_label_set_long_mode(lbl_title, LV_LABEL_LONG_CLIP);
    lv_obj_set_size(lbl_title, 106, 50);
    lv_obj_set_pos(lbl_title, 17, 18);
    lv_obj_set_style_text_color(lbl_title, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_extrabold_18, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_title, 5, LV_PART_MAIN);

    /* Message — Light 12, black, explicit newline, no auto-wrap */
    lv_obj_t * lbl_msg = lv_label_create(card);
    lv_label_set_text(lbl_msg, "INSTRUCTIONAL\nMESSAGE");
    lv_label_set_long_mode(lbl_msg, LV_LABEL_LONG_CLIP);
    lv_obj_set_size(lbl_msg, 106, 32);
    lv_obj_set_pos(lbl_msg, 19, 72);
    lv_obj_set_style_text_color(lbl_msg, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_msg, &lv_font_montserrat_light_12, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_msg, 3, LV_PART_MAIN);

     /* Animated accent bar */
    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0x7E0000), lv_color_hex(0x7E0000));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
