#include "lvgl/lvgl.h"
#include "screen_usa_logo.h"
#include "device_config.h"
#include "usa_logo_svg_data.h"

/*
 * USA Interlock logo screen
 *
 * White background with a black rounded card (126x126, r=14).
 * SVG logo centred inside the card — aspect ratio 447:361 (wider than tall),
 * so rendered at 120×97 to fill width while preserving ratio.
 */

void screen_usa_logo_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Black rounded card */
    lv_obj_t * card = lv_obj_create(scr);
    lv_obj_set_size(card, 126, 126);
    lv_obj_set_pos(card, 1, 1);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(card, 14, LV_PART_MAIN);
    lv_obj_set_style_border_width(card, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(card, 0, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(card, true, LV_PART_MAIN);

    /* USA logo SVG — fit width (120px), height proportional: 120 * 361/447 ≈ 97px */
    lv_obj_t * img = lv_image_create(card);
    lv_obj_set_size(img, 120, 97);
    lv_image_set_src(img, &usa_logo_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
