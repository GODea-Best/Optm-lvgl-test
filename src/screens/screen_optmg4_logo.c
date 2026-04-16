#include "lvgl/lvgl.h"
#include "screen_optmg4_logo.h"
#include "device_config.h"
#include "logo_svg_data.h"

/*
 * OptmG4 logo card — three-phase animated reveal
 *
 * Uses the same SVG-02 data as the Logo card (logo_svg_dsc).
 * The SVG has a 2000×2000 viewBox; content spans roughly x=155–1857.
 * Rendered at 120×120, the glyph boundaries scale to:
 *   O  right edge  ≈ 120 × 568/2000 = 34 px
 *   G4 left  edge  ≈ 120 × 1610/2000 = 97 px
 *
 * Phase 1 (0 – SLIDE_MS):
 *   A black mask sits just to the right of the 'O', then slides rightward
 *   off screen.  The word is uncovered left-to-right: O appears first,
 *   then p, t, m in turn.
 *
 * Phase 2 (SLIDE_MS – SLIDE_MS+FADE_MS):
 *   A second black mask sits over the 'G4' region and fades to transparent,
 *   making those two glyphs appear to materialise independently.
 */

#define LOGO_RENDER_W  120   /* rendered image width  (px) — matches logo card */
#define LOGO_RENDER_H  120   /* rendered image height (px) */
#define O_END_X         34   /* O right edge inside the rendered image         */
#define G4_START_X      97   /* G4 left edge inside the rendered image         */

#define SLIDE_MS      2000   /* phase-1 mask slide duration */
#define FADE_MS        600   /* phase-2 G4 fade-in duration */

static void _anim_x_exec  (void * obj, int32_t v) { lv_obj_set_x((lv_obj_t *)obj, v); }
static void _anim_opa_exec(void * obj, int32_t v) { lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)v, LV_PART_MAIN); }

void screen_optmg4_logo_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(scr, true, LV_PART_MAIN);

    /* Pre-compute positions in card (screen) space */
    int32_t img_x      = (128 - LOGO_RENDER_W) / 2;  /* centred horizontally */
    int32_t img_y      = (128 - LOGO_RENDER_H) / 2;  /* centred vertically   */
    int32_t o_card_end = img_x + O_END_X;             /* O right edge on screen */
    int32_t g4_card_x  = img_x + G4_START_X;          /* G4 left edge on screen */
    int32_t g4_w       = LOGO_RENDER_W - G4_START_X;  /* G4 region width        */

    /* SVG — static at its final centred position throughout */
    lv_obj_t * img = lv_image_create(scr);
    lv_obj_set_size(img, LOGO_RENDER_W, LOGO_RENDER_H);
    lv_image_set_src(img, &logo_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_set_pos(img, img_x, img_y);

    /*
     * Phase-1 mask — full card width, starts just past the 'O'.
     * Slides right until it exits the card, uncovering the word
     * left-to-right as it goes.  Added before the G4 mask so the
     * G4 mask is drawn on top (higher z-order) and keeps G4 hidden
     * even after the slide mask has passed.
     */
    lv_obj_t * slide_mask = lv_obj_create(scr);
    lv_obj_set_size(slide_mask, 128, LOGO_RENDER_H);
    lv_obj_set_pos(slide_mask, o_card_end, img_y);
    lv_obj_set_style_bg_color(slide_mask, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(slide_mask, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(slide_mask, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(slide_mask, 0, LV_PART_MAIN);
    lv_obj_remove_flag(slide_mask, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(slide_mask, LV_OBJ_FLAG_CLICKABLE);

    /*
     * Phase-2 mask — covers only the G4 region.
     * Sits on top of the slide mask (added later → higher z-order),
     * so G4 stays hidden until this mask fades out.
     */
    lv_obj_t * g4_mask = lv_obj_create(scr);
    lv_obj_set_size(g4_mask, g4_w + 4, LOGO_RENDER_H); /* +4 px overlap prevents gap */
    lv_obj_set_pos(g4_mask, g4_card_x, img_y);
    lv_obj_set_style_bg_color(g4_mask, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(g4_mask, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(g4_mask, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(g4_mask, 0, LV_PART_MAIN);
    lv_obj_remove_flag(g4_mask, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(g4_mask, LV_OBJ_FLAG_CLICKABLE);

    /* Phase 1 — slide mask rightward off screen */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _anim_x_exec);
    lv_anim_set_var(&a, slide_mask);
    lv_anim_set_values(&a, o_card_end, 128);
    lv_anim_set_duration(&a, SLIDE_MS);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);

    /* Phase 2 — fade G4 mask out once the slide completes */
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _anim_opa_exec);
    lv_anim_set_var(&a, g4_mask);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_duration(&a, FADE_MS);
    lv_anim_set_delay(&a, SLIDE_MS);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
    lv_anim_start(&a);

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
