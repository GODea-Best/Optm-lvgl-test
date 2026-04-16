#include "lvgl/lvgl.h"
#include "screen_initial_test_icons.h"
#include "device_config.h"
#include "accent_bar.h"
#include "wind_svg_data.h"

/* Initial Test — icon version
 * Large wind icon conveys "blow into the device". No text. */

void screen_initial_test_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Wind icon — centered upper half */
    lv_obj_t * img = lv_image_create(scr);
    lv_obj_set_size(img, 72, 72);
    lv_image_set_src(img, &wind_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, -12);

    /* Right-pointing arrow — suggests "blow this way" */
    lv_obj_t * lbl_arrow = lv_label_create(scr);
    lv_label_set_text(lbl_arrow, LV_SYMBOL_RIGHT LV_SYMBOL_RIGHT LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(lbl_arrow, lv_color_hex(0x1400CA), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_arrow, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(lbl_arrow, LV_ALIGN_CENTER, 0, 36);

    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0x1400CA), lv_color_hex(0x1400CA));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
