#include "lvgl/lvgl.h"
#include "screen_initial_card.h"
#include "device_config.h"
#include "fonts_bold.h"
/*
 * Initial Test — Card-on-Card design
 *
 * Rectangle 5 (outer, 128×128, r=15):
 *   radial-gradient(73.83% at 50% 50%, #000000 0%, #666666 100%)
 *   gradient circle radius ≈ 47px from center (73.83% × 64px)
 *
 * Rectangle 8 (inner, 120×120 at 4,3, r=14):
 *   radial-gradient(102.08% at 50% 50%, #000270 0%, #363636 100%)
 *   gradient circle radius ≈ 61px from center (102% × 60px)
 *   box-shadow: 0 4px 4px rgba(0,0,0,0.5)
 *
 * Text:
 *   "BLOW EVENLY"       — ExtraBold 24, screen pos (13,10), 101×60, white, center
 *   CFG_BLOW_PROMPT_MSG — Regular   14, screen pos ( 9,72), 111×32, white, center
 *
 * Drawn via LVGL vector path with lv_draw_vector_dsc_set_fill_radial_gradient
 * so the gradient is guaranteed to render correctly.
 */

#define _W   128.0f
#define _H   128.0f
#define _CX  64.0f   /* outer card center x */
#define _CY  64.0f   /* outer card center y */

/* Outer radial radius: 73.83% × 64 ≈ 47px */
#define _OUTER_R  47.0f

/* Inner card position and size */
#define _IX   4.0f
#define _IY   3.0f
#define _IW  120.0f
#define _IH  120.0f


static void _card_draw_cb(lv_event_t * e)
{
    lv_layer_t * layer = lv_event_get_layer(e);
    lv_obj_t   * obj   = lv_event_get_target(e);
    lv_area_t    coords;
    lv_obj_get_coords(obj, &coords);

    lv_matrix_t m;
    lv_matrix_identity(&m);
    lv_matrix_translate(&m, (float)coords.x1, (float)coords.y1);

    lv_grad_stop_t stops[2];

    /* ------------------------------------------------------------------
     * Pass 1 — Outer card: radial gradient #000000 → #666666
     * ------------------------------------------------------------------ */
    lv_vector_path_t * path_outer = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
    lv_vector_path_append_rectangle(path_outer, 0.0f, 0.0f, _W, _H, 15.0f, 15.0f);

    lv_draw_vector_dsc_t * dsc_outer = lv_draw_vector_dsc_create(layer);
    lv_draw_vector_dsc_set_transform(dsc_outer, &m);

    stops[0].color = lv_color_hex(0x000000); stops[0].opa = LV_OPA_COVER; stops[0].frac =   0;
    stops[1].color = lv_color_hex(0x666666); stops[1].opa = LV_OPA_COVER; stops[1].frac = 255;
    lv_draw_vector_dsc_set_fill_radial_gradient(dsc_outer, _CX, _CY, _OUTER_R);
    lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc_outer, stops, 2);
    lv_draw_vector_dsc_set_fill_gradient_spread(dsc_outer, LV_VECTOR_GRADIENT_SPREAD_PAD);

    lv_draw_vector_dsc_add_path(dsc_outer, path_outer);
    lv_draw_vector(dsc_outer);
    lv_draw_vector_dsc_delete(dsc_outer);
    lv_vector_path_delete(path_outer);

    /* ------------------------------------------------------------------
     * Pass 2 — Inner card: flat solid #000145 (deep navy)
     * CSS: background: #000145; box-shadow: 0 4px 4px rgba(0,0,0,.5)
     * ------------------------------------------------------------------ */
    lv_vector_path_t * path_inner = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
    lv_vector_path_append_rectangle(path_inner, _IX, _IY, _IW, _IH, 14.0f, 14.0f);

    lv_draw_vector_dsc_t * dsc_inner = lv_draw_vector_dsc_create(layer);
    lv_draw_vector_dsc_set_transform(dsc_inner, &m);
    lv_draw_vector_dsc_set_fill_color(dsc_inner, lv_color_hex(0x000145));
    lv_draw_vector_dsc_set_fill_opa(dsc_inner, LV_OPA_COVER);

    lv_draw_vector_dsc_add_path(dsc_inner, path_inner);
    lv_draw_vector(dsc_inner);
    lv_draw_vector_dsc_delete(dsc_inner);
    lv_vector_path_delete(path_inner);
}

void screen_initial_card_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(scr, true, LV_PART_MAIN);
    /* Black fallback — vector draw covers the full background */
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_add_event_cb(scr, _card_draw_cb, LV_EVENT_DRAW_MAIN, NULL);

    /* ------------------------------------------------------------------
     * "BLOW EVENLY"
     * CSS screen pos: left=13, top=10 → 101×60, ExtraBold 24, center
     * ------------------------------------------------------------------ */
    lv_obj_t * lbl_blow = lv_label_create(scr);
    lv_label_set_text(lbl_blow, "BLOW\nEVENLY");
    lv_obj_set_size(lbl_blow, 101, 60);
    lv_obj_set_pos(lbl_blow, 13, 10);
    lv_obj_set_style_text_color(lbl_blow, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_blow, &lv_font_montserrat_extrabold_24, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_blow, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_blow, 0, LV_PART_MAIN);

    /* ------------------------------------------------------------------
     * Instructional text
     * CSS screen pos: left=9, top=72 → 111×32, Regular 14, center
     * ------------------------------------------------------------------ */
    lv_obj_t * lbl_msg = lv_label_create(scr);
    lv_label_set_text(lbl_msg, CFG_BLOW_PROMPT_MSG);
    lv_obj_set_size(lbl_msg, 111, 32);
    lv_obj_set_pos(lbl_msg, 9, 72);
    lv_obj_set_style_text_color(lbl_msg, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_msg, &lv_font_montserrat_regular_14, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_msg, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
