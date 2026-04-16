#include "lvgl/lvgl.h"
#include "screen_violation_lockout_icons.h"
#include "device_config.h"
#include "accent_bar.h"
#include "triangle_exclamation_svg_data.h"
#include "lock_solid_svg_data.h"

/* Violation Lockout — icon version
 * Top bar + bottom bar frame. Large triangle warning centred top,
 * lock icon below — no text at all. */

void screen_violation_lockout_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Top red bar */
    lv_obj_t * bar_top = lv_obj_create(scr);
    lv_obj_set_size(bar_top, 113, 9);
    lv_obj_set_pos(bar_top, 7, 6);
    accent_bar_attach(bar_top, false, lv_color_hex(0xCA0000), lv_color_hex(0xCA0000));

    /* Bottom red bar */
    ACCENT_BAR_CREATE(scr, false, lv_color_hex(0xCA0000), lv_color_hex(0xCA0000));

    /* Triangle — violation warning */
    lv_obj_t * img_tri = lv_image_create(scr);
    lv_obj_set_size(img_tri, 52, 52);
    lv_image_set_src(img_tri, &triangle_exclamation_svg_dsc);
    lv_image_set_align(img_tri, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img_tri, LV_ALIGN_CENTER, 0, -24);

    /* Lock icon — lockout state */
    lv_obj_t * img_lock = lv_image_create(scr);
    lv_obj_set_size(img_lock, 32, 32);
    lv_image_set_src(img_lock, &lock_solid_svg_dsc);
    lv_image_set_align(img_lock, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img_lock, LV_ALIGN_CENTER, 0, 32);

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
