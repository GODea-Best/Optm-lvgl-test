#include "lvgl/lvgl.h"
#include "screen_logo.h"
#include "device_config.h"
#include "logo_svg_data.h"

/*
 * Logo screen
 *
 * Layout:
 *   - White background with a black rounded-square card (126x126, r=14)
 *   - SVG logo centred inside the card, scaled to fill ~100x100 px
 *
 * The SVG is embedded as a byte array in logo_svg_data.h (no file I/O).
 * LV_IMAGE_ALIGN_CONTAIN auto-scales it to fit the widget's explicit size.
 *
 * Requires LV_USE_SVG 1 in lv_conf.h.
 */

#define LOGO_SIZE  120   /* rendered size (px) inside the 126x126 card */

static void _img_fade_exec(void * obj, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)v, LV_PART_MAIN);
}

void screen_logo_create(lv_obj_t * old_scr)
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

    /* SVG image centred in the card.
     * Explicit size keeps the layout box at LOGO_SIZE×LOGO_SIZE;
     * LV_IMAGE_ALIGN_CONTAIN then computes scale = widget_size*256/natural_size. */
    lv_obj_t * img = lv_image_create(card);
    lv_obj_set_size(img, LOGO_SIZE, LOGO_SIZE);
    lv_image_set_src(img, &logo_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_opa(img, LV_OPA_TRANSP, LV_PART_MAIN);

    /* Fade in over 500ms */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _img_fade_exec);
    lv_anim_set_var(&a, img);
    lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_duration(&a, 1500);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
