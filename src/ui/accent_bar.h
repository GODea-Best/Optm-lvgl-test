#ifndef ACCENT_BAR_H
#define ACCENT_BAR_H

#include "lvgl/lvgl.h"

/*
 * accent_bar.h
 *
 * Standard shaped bottom accent bar (113×9, asymmetric radii) with an
 * animated white shimmer sweeping left→right repeatedly.
 *
 * The shimmer is a soft transparent→white→transparent band drawn over the
 * base fill in two separate gradient passes (LVGL vector is capped at 2
 * gradient stops). Each pass path is clipped to the actual band rectangle
 * so gradient clamping never bleeds outside the band.
 *
 * Usage:
 *   ACCENT_BAR_CREATE(parent, false, lv_color_hex(0x1400CA), lv_color_hex(0x1400CA));
 *   ACCENT_BAR_CREATE(parent, true,  lv_color_hex(0x1D9101), lv_color_hex(0x092B00));
 */

#define _AB_W          113
#define _AB_H          9
#define _AB_SHIMMER_B  22      /* shimmer half-width in px */
#define _AB_SHINE_OPA  90      /* peak shimmer opacity (0-255) */

typedef struct {
    lv_obj_t * obj;
    int32_t    t;
    bool       gradient;
    lv_color_t color1;
    lv_color_t color2;
} _ab_ctx_t;

/* ------------------------------------------------------------------ */
/* Shaped bar path                                                      */
/* ------------------------------------------------------------------ */

static lv_vector_path_t * _ab_make_path(void)
{
    const float W = _AB_W, H = _AB_H;
    const float rt_rx = 4.0f, rt_ry = 2.0f;
    const float rb_rx = 14.0f, rb_ry = 7.0f;

    lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
    lv_fpoint_t p;
    p = (lv_fpoint_t){rt_rx, 0};     lv_vector_path_move_to(path, &p);
    p = (lv_fpoint_t){W-rt_rx, 0};   lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){W, rt_ry};     lv_vector_path_arc_to(path, rt_rx, rt_ry, 0, false, true, &p);
    p = (lv_fpoint_t){W, H-rb_ry};   lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){W-rb_rx, H};   lv_vector_path_arc_to(path, rb_rx, rb_ry, 0, false, true, &p);
    p = (lv_fpoint_t){rb_rx, H};     lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){0, H-rb_ry};   lv_vector_path_arc_to(path, rb_rx, rb_ry, 0, false, true, &p);
    p = (lv_fpoint_t){0, rt_ry};     lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){rt_rx, 0};     lv_vector_path_arc_to(path, rt_rx, rt_ry, 0, false, true, &p);
    lv_vector_path_close(path);
    return path;
}

/* ------------------------------------------------------------------ */
/* Draw one shimmer half-band (rect clipped to [0, _AB_W])             */
/* ------------------------------------------------------------------ */

static void _ab_draw_band(lv_draw_vector_dsc_t * dsc,
                           float x0, float x1,
                           float gx0, float gx1,
                           lv_opa_t opa_l, lv_opa_t opa_r)
{
    if(x0 < 0) x0 = 0;
    if(x1 > _AB_W) x1 = (float)_AB_W;
    if(x0 >= x1) return;

    lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
    lv_fpoint_t p;
    p = (lv_fpoint_t){x0, 0};       lv_vector_path_move_to(path, &p);
    p = (lv_fpoint_t){x1, 0};       lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){x1, _AB_H};   lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){x0, _AB_H};   lv_vector_path_line_to(path, &p);
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

/* ------------------------------------------------------------------ */
/* Draw callback: base fill + shimmer                                   */
/* ------------------------------------------------------------------ */

static void _ab_draw_cb(lv_event_t * e)
{
    _ab_ctx_t  * ctx   = (_ab_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t * layer = lv_event_get_layer(e);
    lv_obj_t   * obj   = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    lv_matrix_t mat;
    lv_matrix_identity(&mat);
    lv_matrix_translate(&mat, (float)coords.x1, (float)coords.y1);

    /* Pass 1: base fill — shaped bar, solid or gradient */
    {
        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);

        if(ctx->gradient) {
            lv_draw_vector_dsc_set_fill_linear_gradient(dsc, 0, 0, _AB_W, 0);
            lv_grad_stop_t stops[2];
            stops[0].color = ctx->color1; stops[0].opa = LV_OPA_COVER; stops[0].frac =   0;
            stops[1].color = ctx->color2; stops[1].opa = LV_OPA_COVER; stops[1].frac = 255;
            lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);
        } else {
            lv_draw_vector_dsc_set_fill_color(dsc, ctx->color1);
            lv_draw_vector_dsc_set_fill_opa(dsc, LV_OPA_COVER);
        }

        lv_vector_path_t * path = _ab_make_path();
        lv_draw_vector_dsc_add_path(dsc, path);
        lv_draw_vector(dsc);
        lv_vector_path_delete(path);
        lv_draw_vector_dsc_delete(dsc);
    }

    float t = (float)ctx->t;
    float B = (float)_AB_SHIMMER_B;

    /* Pass 2: leading edge — transparent → white */
    {
        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);
        _ab_draw_band(dsc, t - B, t, t - B, t, LV_OPA_TRANSP, _AB_SHINE_OPA);
        lv_draw_vector_dsc_delete(dsc);
    }

    /* Pass 3: trailing edge — white → transparent */
    {
        lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
        lv_draw_vector_dsc_set_transform(dsc, &mat);
        _ab_draw_band(dsc, t, t + B, t, t + B, _AB_SHINE_OPA, LV_OPA_TRANSP);
        lv_draw_vector_dsc_delete(dsc);
    }
}

static void _ab_delete_cb(lv_event_t * e)
{
    _ab_ctx_t * ctx = (_ab_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(ctx, NULL);
    lv_free(ctx);
}

static void _ab_anim_exec(void * var, int32_t v)
{
    _ab_ctx_t * ctx = (_ab_ctx_t *)var;
    ctx->t = v;
    lv_obj_invalidate(ctx->obj);
}

/* ------------------------------------------------------------------ */
/* Public API                                                           */
/* ------------------------------------------------------------------ */

static inline void accent_bar_attach(lv_obj_t * obj,
                                      bool gradient,
                                      lv_color_t color1,
                                      lv_color_t color2)
{
    _ab_ctx_t * ctx = (_ab_ctx_t *)lv_malloc(sizeof(_ab_ctx_t));
    ctx->obj      = obj;
    ctx->t        = 0;
    ctx->gradient = gradient;
    ctx->color1   = color1;
    ctx->color2   = color2;

    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);

    lv_obj_add_event_cb(obj, _ab_draw_cb,   LV_EVENT_DRAW_MAIN, ctx);
    lv_obj_add_event_cb(obj, _ab_delete_cb, LV_EVENT_DELETE,    ctx);

    /* Shimmer: left→right in 900ms, pause 1800ms, repeat */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _ab_anim_exec);
    lv_anim_set_var(&a, ctx);
    lv_anim_set_values(&a, 0, _AB_W + _AB_SHIMMER_B * 2);
    lv_anim_set_duration(&a, 700);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 600);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_start(&a);
}

/* Convenience macro — creates, sizes, positions and attaches the bar */
#define ACCENT_BAR_CREATE(parent, gradient, c1, c2) do { \
    lv_obj_t * _bar = lv_obj_create(parent); \
    lv_obj_set_size(_bar, _AB_W, _AB_H); \
    lv_obj_set_pos(_bar, 7, 113); \
    accent_bar_attach(_bar, (gradient), (c1), (c2)); \
} while(0)

#endif /* ACCENT_BAR_H */
