#ifndef BOTTOM_BAR_H
#define BOTTOM_BAR_H

#include "lvgl/lvgl.h"

/*
 * bottom_bar.h
 *
 * Full-width (128×21) animated bottom bar for bold card screens.
 * A white shimmer sweeps left→right over a solid-colour base fill.
 * The parent card's clip_corner (r=15) naturally rounds the bottom corners.
 *
 * Usage:
 *   BOTTOM_BAR_CREATE(parent, lv_color_hex(0x1400CA));
 */

#define _BBR_W          128
#define _BBR_H           21
#define _BBR_SHIMMER_B   30   /* shimmer half-band width in px */
#define _BBR_SHINE_OPA  100   /* peak shimmer opacity (0–255)  */

typedef struct {
    lv_obj_t  * obj;
    int32_t     t;
    lv_color_t  color;
    float       radius;
} _bbr_ctx_t;

/* One shimmer half-band; path clamped to [0, bar_w] */
static void _bbr_draw_band(lv_draw_vector_dsc_t * dsc,
                            float x0, float x1,
                            float gx0, float gx1,
                            float bar_w, float bar_h,
                            lv_opa_t opa_l, lv_opa_t opa_r)
{
    if(x0 < 0)       x0 = 0;
    if(x1 > bar_w)   x1 = bar_w;
    if(x0 >= x1)     return;

    lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
    lv_fpoint_t p;
    p = (lv_fpoint_t){x0, 0};      lv_vector_path_move_to(path, &p);
    p = (lv_fpoint_t){x1, 0};      lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){x1, bar_h};  lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){x0, bar_h};  lv_vector_path_line_to(path, &p);
    lv_vector_path_close(path);

    lv_draw_vector_dsc_set_fill_linear_gradient(dsc, gx0, 0, gx1, 0);
    lv_grad_stop_t stops[2];
    stops[0].color = lv_color_hex(0xFFFFFF); stops[0].opa = opa_l; stops[0].frac =   0;
    stops[1].color = lv_color_hex(0xFFFFFF); stops[1].opa = opa_r; stops[1].frac = 255;
    lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);
    lv_draw_vector_dsc_add_path(dsc, path);
    lv_draw_vector(dsc);
    lv_vector_path_delete(path);
}

static void _bbr_draw_cb(lv_event_t * e)
{
    _bbr_ctx_t * ctx   = (_bbr_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t * layer = lv_event_get_layer(e);
    lv_obj_t   * obj   = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    /* Use actual object dimensions so bars of any size render correctly */
    float W = (float)(coords.x2 - coords.x1 + 1);
    float H = (float)(coords.y2 - coords.y1 + 1);

    lv_matrix_t mat;
    lv_matrix_identity(&mat);
    lv_matrix_translate(&mat, (float)coords.x1, (float)coords.y1);

    float t = (float)ctx->t;
    float B = (float)_BBR_SHIMMER_B;

    /* Pass 1 — base fill (rounded if ctx->radius > 0) */
    {
        lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
        if(ctx->radius > 0.0f) {
            lv_vector_path_append_rectangle(path, 0, 0, W, H, ctx->radius, ctx->radius);
        } else {
            lv_fpoint_t p;
            p = (lv_fpoint_t){0, 0}; lv_vector_path_move_to(path, &p);
            p = (lv_fpoint_t){W, 0}; lv_vector_path_line_to(path, &p);
            p = (lv_fpoint_t){W, H}; lv_vector_path_line_to(path, &p);
            p = (lv_fpoint_t){0, H}; lv_vector_path_line_to(path, &p);
            lv_vector_path_close(path);
        }

        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);
        lv_draw_vector_dsc_set_fill_color(dsc, ctx->color);
        lv_draw_vector_dsc_set_fill_opa(dsc, LV_OPA_COVER);
        lv_draw_vector_dsc_add_path(dsc, path);
        lv_draw_vector(dsc);
        lv_vector_path_delete(path);
        lv_draw_vector_dsc_delete(dsc);
    }
    /* Pass 2 — leading edge: transparent → peak */
    {
        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);
        _bbr_draw_band(dsc, t-B, t, t-B, t, W, H, LV_OPA_TRANSP, _BBR_SHINE_OPA);
        lv_draw_vector_dsc_delete(dsc);
    }
    /* Pass 3 — trailing edge: peak → transparent */
    {
        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);
        _bbr_draw_band(dsc, t, t+B, t, t+B, W, H, _BBR_SHINE_OPA, LV_OPA_TRANSP);
        lv_draw_vector_dsc_delete(dsc);
    }
}

static void _bbr_anim_exec(void * var, int32_t v)
{
    _bbr_ctx_t * ctx = (_bbr_ctx_t *)var;
    ctx->t = v;
    lv_obj_invalidate(ctx->obj);
}

static void _bbr_delete_cb(lv_event_t * e)
{
    _bbr_ctx_t * ctx = (_bbr_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(ctx, NULL);
    lv_free(ctx);
}

static inline void bottom_bar_attach(lv_obj_t * obj, lv_color_t color, float radius)
{
    _bbr_ctx_t * ctx = (_bbr_ctx_t *)lv_malloc(sizeof(_bbr_ctx_t));
    ctx->obj    = obj;
    ctx->t      = 0;
    ctx->color  = color;
    ctx->radius = radius;

    lv_obj_set_style_bg_opa(obj,       LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj, 0,             LV_PART_MAIN);
    lv_obj_set_style_pad_all(obj,      0,             LV_PART_MAIN);

    lv_obj_add_event_cb(obj, _bbr_draw_cb,   LV_EVENT_DRAW_MAIN, ctx);
    lv_obj_add_event_cb(obj, _bbr_delete_cb, LV_EVENT_DELETE,    ctx);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _bbr_anim_exec);
    lv_anim_set_var(&a, ctx);
    lv_anim_set_values(&a, 0, lv_obj_get_width(obj) + _BBR_SHIMMER_B * 2);
    lv_anim_set_duration(&a, 800);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 1000);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);
}

/* Creates, sizes, positions, and attaches the bar at y=107 */
#define BOTTOM_BAR_CREATE(parent, color) do {              \
    lv_obj_t * _bar = lv_obj_create(parent);              \
    lv_obj_set_size(_bar, _BBR_W, _BBR_H);                \
    lv_obj_set_pos(_bar, 0, 107);                         \
    bottom_bar_attach(_bar, (color), 0.0f);                \
} while(0)

#endif /* BOTTOM_BAR_H */
