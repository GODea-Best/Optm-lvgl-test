#include "lvgl/lvgl.h"
#include "screen_blow_prompt_anim.h"
#include "device_config.h"
#include "fonts_bold.h"

/*
 * Blow Prompt (Animated) — left↔right bounce
 *
 * Two CSS states:
 *   State A (start): linear-gradient(90deg, #1400CA 0%, #000000 50%)
 *     → blue at x=0, fades to black at x=64, solid black x=64..128
 *   State B (end):   same gradient, rotate(180deg)
 *     → solid black x=0..64, black at x=64 fading to blue at x=128
 *
 * Implementation: gradient always spans 64px (half the screen).
 *   origin `ox` animates 0→64; gradient line is (ox,0)→(ox+64,0).
 *   Stops: blue@0 → black@255.
 *   At ox=0:  blue at left edge  → state A ✓
 *   At ox=64: blue at right edge → state B ✓
 *   playback_time reverses ox 64→0 for the return journey.
 */

typedef struct {
    lv_obj_t * obj;
    int32_t    ox;   /* 0..64: x-origin of the 64px gradient band */
} _blow_anim_ctx_t;

static void _blow_anim_draw_cb(lv_event_t * e)
{
    _blow_anim_ctx_t * g     = (_blow_anim_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t       * layer = lv_event_get_layer(e);
    lv_obj_t         * obj   = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    float W = (float)(coords.x2 - coords.x1 + 1);
    float H = (float)(coords.y2 - coords.y1 + 1);

    lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
    lv_fpoint_t p;
    p = (lv_fpoint_t){0, 0};  lv_vector_path_move_to(path, &p);
    p = (lv_fpoint_t){W, 0};  lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){W, H};  lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){0, H};  lv_vector_path_line_to(path, &p);
    lv_vector_path_close(path);

    lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
    lv_matrix_t m;
    lv_matrix_identity(&m);
    lv_matrix_translate(&m, (float)coords.x1, (float)coords.y1);
    lv_draw_vector_dsc_set_transform(dsc, &m);

    /* 64px gradient band: blue at ox, black at ox+64 */
    float x1 = (float)g->ox;
    float x2 = x1 + 64.0f;
    lv_draw_vector_dsc_set_fill_linear_gradient(dsc, x1, 0, x2, 0);

    lv_grad_stop_t stops[2];
    stops[0].color = lv_color_hex(0x1400CA); stops[0].opa = LV_OPA_COVER; stops[0].frac =   0;
    stops[1].color = lv_color_hex(0x000000); stops[1].opa = LV_OPA_COVER; stops[1].frac = 255;
    lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);

    lv_draw_vector_dsc_add_path(dsc, path);
    lv_draw_vector(dsc);
    lv_draw_vector_dsc_delete(dsc);
    lv_vector_path_delete(path);
}

static void _blow_anim_delete_cb(lv_event_t * e)
{
    _blow_anim_ctx_t * g = (_blow_anim_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(g, NULL);
    lv_free(g);
}

static void _blow_anim_exec(void * var, int32_t v)
{
    _blow_anim_ctx_t * g = (_blow_anim_ctx_t *)var;
    g->ox = v;
    lv_obj_invalidate(g->obj);
}

void screen_blow_prompt_anim_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, LV_PART_MAIN);

    _blow_anim_ctx_t * g = (_blow_anim_ctx_t *)lv_malloc(sizeof(_blow_anim_ctx_t));
    g->obj = scr;
    g->ox  = 0;
    lv_obj_add_event_cb(scr, _blow_anim_draw_cb,   LV_EVENT_DRAW_MAIN, g);
    lv_obj_add_event_cb(scr, _blow_anim_delete_cb, LV_EVENT_DELETE,    g);

    /* ox: 0→64 (900ms, state A→B), then playback reverses 64→0 (900ms, B→A) */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _blow_anim_exec);
    lv_anim_set_var(&a, g);
    lv_anim_set_values(&a, 0, 64);
    lv_anim_set_duration(&a, 900);
    lv_anim_set_playback_time(&a, 900);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);

    /* "BLOW EVENLY" — ExtraBold 24, left=12, top=14, 103×60 */
    lv_obj_t * lbl_blow = lv_label_create(scr);
    lv_label_set_text(lbl_blow, "BLOW\nEVENLY");
    lv_obj_set_size(lbl_blow, 103, 60);
    lv_obj_set_pos(lbl_blow, 12, 14);
    lv_obj_set_style_text_color(lbl_blow, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_blow, &lv_font_montserrat_extrabold_24, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_blow, 0, LV_PART_MAIN);

    /* Instructional — Light 16, left=15, top=76, 98×38 */
    lv_obj_t * lbl_msg = lv_label_create(scr);
    lv_label_set_text(lbl_msg, CFG_BLOW_PROMPT_MSG);
    lv_obj_set_size(lbl_msg, 98, 38);
    lv_obj_set_pos(lbl_msg, 15, 76);
    lv_obj_set_style_text_color(lbl_msg, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_msg, &lv_font_montserrat_light_16, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_msg, 4, LV_PART_MAIN);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
