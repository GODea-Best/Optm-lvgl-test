#include "lvgl/lvgl.h"
#include "screen_usa_logo_3d.h"
#include "device_config.h"
#include "usa_logo_svg_data.h"

/*
 * USA Logo — simulated 3D via moving light gradient
 *
 * A soft white diagonal sheen makes one pass across the card every ~4s.
 *
 * LVGL vector gradients are capped at 2 stops, so the bell-curve sheen
 * (transparent → white → transparent) is split into two halves.  Each half
 * is drawn with its own independent draw descriptor so paths never
 * accumulate across calls.  The path for each half is clipped to exactly
 * the diagonal strip it covers, preventing the 2-stop gradient from
 * clamping outside its range.
 */

#define CARD_W      106
#define CARD_H      106
#define SHEEN_BAND  48   /* half-width of gradient band — wider = softer */
#define SHEEN_PEAK  70   /* max opacity of the sheen at its centre (0-255) */

typedef struct {
    lv_obj_t * obj;
    int32_t    t;
} _sheen_ctx_t;

/* ------------------------------------------------------------------ */
/* Sutherland-Hodgman: clip card rect to diagonal strip x+y in [lo,hi] */
/* ------------------------------------------------------------------ */

static int _clip_strip(float W, float H, float sum_lo, float sum_hi,
                       float * outx, float * outy)
{
    /* Card corners */
    float inx[4] = {0, W, W, 0};
    float iny[4] = {0, 0, H, H};
    float tx[8], ty[8];
    int n = 4, tn = 0;

    /* Clip against x+y >= sum_lo */
    for(int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        float x0 = inx[i], y0 = iny[i], x1 = inx[j], y1 = iny[j];
        int in0 = (x0 + y0 >= sum_lo);
        int in1 = (x1 + y1 >= sum_lo);
        if(in0) { tx[tn] = x0; ty[tn++] = y0; }
        if(in0 != in1) {
            float d = (x1 - x0) + (y1 - y0);
            float k = (sum_lo - x0 - y0) / d;
            tx[tn] = x0 + k * (x1 - x0);
            ty[tn++] = y0 + k * (y1 - y0);
        }
    }
    n = tn; tn = 0;

    /* Clip against x+y <= sum_hi */
    for(int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        float x0 = tx[i], y0 = ty[i], x1 = tx[j], y1 = ty[j];
        int in0 = (x0 + y0 <= sum_hi);
        int in1 = (x1 + y1 <= sum_hi);
        if(in0) { outx[tn] = x0; outy[tn++] = y0; }
        if(in0 != in1) {
            float d = (x1 - x0) + (y1 - y0);
            float k = (sum_hi - x0 - y0) / d;
            outx[tn] = x0 + k * (x1 - x0);
            outy[tn++] = y0 + k * (y1 - y0);
        }
    }
    return tn;
}

/* ------------------------------------------------------------------ */
/* Sheen draw callback                                                  */
/* ------------------------------------------------------------------ */

static void _sheen_draw_cb(lv_event_t * e)
{
    _sheen_ctx_t * s     = (_sheen_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t   * layer = lv_event_get_layer(e);
    lv_obj_t     * obj   = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    float W = (float)(coords.x2 - coords.x1 + 1);
    float H = (float)(coords.y2 - coords.y1 + 1);
    float t = (float)s->t;
    float B = (float)SHEEN_BAND;

    lv_matrix_t mat;
    lv_matrix_identity(&mat);
    lv_matrix_translate(&mat, (float)coords.x1, (float)coords.y1);

    float vx[8], vy[8];
    int n;
    lv_grad_stop_t stops[2];

    /* -- Leading half: transparent → peak, strip x+y in [t-B, t] -- */
    n = _clip_strip(W, H, t - B, t, vx, vy);
    if(n >= 3) {
        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);

        lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
        lv_fpoint_t p = {vx[0], vy[0]};
        lv_vector_path_move_to(path, &p);
        for(int i = 1; i < n; i++) { p = (lv_fpoint_t){vx[i], vy[i]}; lv_vector_path_line_to(path, &p); }
        lv_vector_path_close(path);

        lv_draw_vector_dsc_set_fill_linear_gradient(dsc, (t-B)*0.5f, (t-B)*0.5f, t*0.5f, t*0.5f);
        stops[0].color = lv_color_hex(0xFFFFFF); stops[0].opa = LV_OPA_TRANSP; stops[0].frac =   0;
        stops[1].color = lv_color_hex(0xFFFFFF); stops[1].opa = SHEEN_PEAK;    stops[1].frac = 255;
        lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);
        lv_draw_vector_dsc_add_path(dsc, path);
        lv_draw_vector(dsc);

        lv_vector_path_delete(path);
        lv_draw_vector_dsc_delete(dsc);
    }

    /* -- Trailing half: peak → transparent, strip x+y in [t, t+B] -- */
    n = _clip_strip(W, H, t, t + B, vx, vy);
    if(n >= 3) {
        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);

        lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
        lv_fpoint_t p = {vx[0], vy[0]};
        lv_vector_path_move_to(path, &p);
        for(int i = 1; i < n; i++) { p = (lv_fpoint_t){vx[i], vy[i]}; lv_vector_path_line_to(path, &p); }
        lv_vector_path_close(path);

        lv_draw_vector_dsc_set_fill_linear_gradient(dsc, t*0.5f, t*0.5f, (t+B)*0.5f, (t+B)*0.5f);
        stops[0].color = lv_color_hex(0xFFFFFF); stops[0].opa = SHEEN_PEAK;    stops[0].frac =   0;
        stops[1].color = lv_color_hex(0xFFFFFF); stops[1].opa = LV_OPA_TRANSP; stops[1].frac = 255;
        lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);
        lv_draw_vector_dsc_add_path(dsc, path);
        lv_draw_vector(dsc);

        lv_vector_path_delete(path);
        lv_draw_vector_dsc_delete(dsc);
    }
}

static void _sheen_delete_cb(lv_event_t * e)
{
    _sheen_ctx_t * s = (_sheen_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(s, NULL);
    lv_free(s);
}

static void _sheen_anim_exec(void * var, int32_t v)
{
    _sheen_ctx_t * s = (_sheen_ctx_t *)var;
    s->t = v;
    lv_obj_invalidate(s->obj);
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                   */
/* ------------------------------------------------------------------ */

void screen_usa_logo_3d_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Static card */
    lv_obj_t * card = lv_obj_create(scr);
    lv_obj_set_size(card, CARD_W, CARD_H);
    lv_obj_align(card, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(card, 14, LV_PART_MAIN);
    lv_obj_set_style_border_width(card, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(card, 0, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(card, true, LV_PART_MAIN);

    /* USA logo */
    lv_obj_t * img = lv_image_create(card);
    lv_obj_set_size(img, 90, 73);
    lv_image_set_src(img, &usa_logo_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    /* Sheen overlay — child of scr, covers the full 128×128 display area */
    _sheen_ctx_t * ctx = (_sheen_ctx_t *)lv_malloc(sizeof(_sheen_ctx_t));
    lv_obj_t * sheen = lv_obj_create(scr);
    lv_obj_set_size(sheen, 128, 128);
    lv_obj_set_pos(sheen, 0, 0);
    lv_obj_set_style_bg_opa(sheen, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(sheen, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(sheen, 0, LV_PART_MAIN);
    lv_obj_remove_flag(sheen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(sheen, LV_OBJ_FLAG_CLICKABLE);

    ctx->obj = sheen;
    ctx->t   = -(SHEEN_BAND * 2);
    lv_obj_add_event_cb(sheen, _sheen_draw_cb,   LV_EVENT_DRAW_MAIN, ctx);
    lv_obj_add_event_cb(sheen, _sheen_delete_cb, LV_EVENT_DELETE,    ctx);

    /* One sweep top-left → bottom-right in 800ms, then 3200ms pause, repeat */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _sheen_anim_exec);
    lv_anim_set_var(&a, ctx);
    lv_anim_set_values(&a, -(SHEEN_BAND * 2), 256 + SHEEN_BAND * 2);
    lv_anim_set_duration(&a, 800);
    lv_anim_set_delay(&a, CFG_USA_3D_SHEEN_DELAY_MS);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 3200);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
