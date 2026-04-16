#include "lvgl/lvgl.h"
#include "screen_invalid_sample_icons.h"
#include "device_config.h"
#include "accent_bar.h"
#include "triangle_exclamation_svg_data.h"

/* Invalid Sample — icon version
 * Warning triangle (rejection) + refresh arrow (retry). No text. */

void screen_invalid_sample_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Warning triangle — sample rejected */
    lv_obj_t * img = lv_image_create(scr);
    lv_obj_set_size(img, 60, 60);
    lv_image_set_src(img, &triangle_exclamation_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, -20);

    /* Refresh — retry */
    lv_obj_t * lbl_ref = lv_label_create(scr);
    lv_label_set_text(lbl_ref, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_color(lbl_ref, lv_color_hex(0xCA0000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_ref, &lv_font_montserrat_36, LV_PART_MAIN);
    lv_obj_align(lbl_ref, LV_ALIGN_CENTER, 0, 32);

    ACCENT_BAR_CREATE(scr, true, lv_color_hex(0x12246E), lv_color_hex(0xCA0000));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
