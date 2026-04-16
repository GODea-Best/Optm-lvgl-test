#include "lvgl/lvgl.h"
#include "screen_sample_analysis.h"
#include "device_config.h"
#include "fonts_bold.h"

/*
 * Sample Analysis screen
 *
 * Black bg, radius 15.
 * Spinning arc ring: 106×106 at (10,12).
 *   - Track:     8px, cycles red→green→gray→red smoothly
 *   - Indicator: 8px, #CDCDCD (fixed 80° spinning segment, 800ms/rev)
 * "ANALYZING\nSAMPLE" — Regular 12, centered inside the ring.
 *
 * Color cycle: phase 0..767 maps to:
 *   0..255:   red  (#CA0000) → green (#1D9101)
 *   256..511: green (#1D9101) → gray  (#CDCDCD)
 *   512..767: gray  (#CDCDCD) → red   (#CA0000)
 */

typedef struct {
    lv_obj_t * arc;
    int32_t    phase;  /* 0..767 */
} _analysis_ctx_t;

static lv_color_t _lerp_color(lv_color_t a, lv_color_t b, uint8_t t)
{
    /* t=0 → a, t=255 → b */
    lv_color_t out;
    out.red   = a.red   + (int16_t)((b.red   - a.red)   * t / 255);
    out.green = a.green + (int16_t)((b.green - a.green) * t / 255);
    out.blue  = a.blue  + (int16_t)((b.blue  - a.blue)  * t / 255);
    return out;
}

static void _color_anim_exec(void * var, int32_t v)
{
    _analysis_ctx_t * ctx = (_analysis_ctx_t *)var;
    ctx->phase = v;

    static const lv_color_t red   = {.red = 0xCA, .green = 0x00, .blue = 0x00};
    static const lv_color_t green = {.red = 0x1D, .green = 0x91, .blue = 0x01};
    static const lv_color_t gray  = {.red = 0xCD, .green = 0xCD, .blue = 0xCD};

    lv_color_t col;
    if(v < 256) {
        col = _lerp_color(red,   green, (uint8_t)v);
    } else if(v < 512) {
        col = _lerp_color(green, gray,  (uint8_t)(v - 256));
    } else {
        col = _lerp_color(gray,  red,   (uint8_t)(v - 512));
    }

    lv_obj_set_style_arc_color(ctx->arc, col, LV_PART_MAIN);
}

static void _color_anim_delete_cb(lv_event_t * e)
{
    _analysis_ctx_t * ctx = (_analysis_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(ctx, NULL);
    lv_free(ctx);
}

static void _span_anim_exec(void * var, int32_t v)
{
    lv_arc_set_angles((lv_obj_t *)var, 0, (uint16_t)v);
}

void screen_sample_analysis_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Arc ring */
    lv_obj_t * arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 106, 106);
    lv_obj_set_pos(arc, 10, 12);
    lv_obj_set_style_pad_all(arc, 0, LV_PART_MAIN);

    /* Track — full 360° background ring, color animated below */
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0xCA0000), LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 8, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_MAIN);

    /* Grey indicator — fixed 80° spinning segment */
    lv_arc_set_angles(arc, 0, 80);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x4A4A4A), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_INDICATOR);

    /* Hide knob, not interactive */
    lv_obj_set_style_bg_opa(arc, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    /* Spin animation: rotate 0→360° linearly, 800ms/rev */
    lv_anim_t a_spin;
    lv_anim_init(&a_spin);
    lv_anim_set_exec_cb(&a_spin, (lv_anim_exec_xcb_t)lv_arc_set_rotation);
    lv_anim_set_var(&a_spin, arc);
    lv_anim_set_values(&a_spin, 0, 360);
    lv_anim_set_duration(&a_spin, 800);
    lv_anim_set_repeat_count(&a_spin, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a_spin, lv_anim_path_linear);
    lv_anim_start(&a_spin);

    /* Span animation: arc end angle bounces 15°↔160°, 1100ms each way.
     * Uses a different period to the spin so it feels organic — the segment
     * is short when spinning fast through a bend, long when sweeping wide. */
    lv_anim_t a_span;
    lv_anim_init(&a_span);
    lv_anim_set_exec_cb(&a_span, _span_anim_exec);
    lv_anim_set_var(&a_span, arc);
    lv_anim_set_values(&a_span, 15, 160);
    lv_anim_set_duration(&a_span, 1100);
    lv_anim_set_playback_time(&a_span, 1100);
    lv_anim_set_repeat_count(&a_span, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a_span, lv_anim_path_ease_in_out);
    lv_anim_start(&a_span);

    /* Color cycle animation: phase 0→767, 2400ms/cycle (800ms per color pair) */
    _analysis_ctx_t * ctx = (_analysis_ctx_t *)lv_malloc(sizeof(_analysis_ctx_t));
    ctx->arc   = arc;
    ctx->phase = 0;

    lv_anim_t a_col;
    lv_anim_init(&a_col);
    lv_anim_set_exec_cb(&a_col, _color_anim_exec);
    lv_anim_set_var(&a_col, ctx);
    lv_anim_set_values(&a_col, 0, 767);
    lv_anim_set_duration(&a_col, 2400);
    lv_anim_set_repeat_count(&a_col, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a_col, lv_anim_path_linear);
    lv_anim_start(&a_col);

    /* Cancel color anim and free ctx when screen is deleted */
    lv_obj_add_event_cb(scr, _color_anim_delete_cb, LV_EVENT_DELETE, ctx);

    /* "ANALYZING\nSAMPLE" centered in ring */
    lv_obj_t * lbl = lv_label_create(scr);
    lv_label_set_text(lbl, "ANALYZING\nSAMPLE");
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(lbl, 86);
    lv_obj_set_height(lbl, LV_SIZE_CONTENT);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_regular_12, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl, 3, LV_PART_MAIN);
    lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 52);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
