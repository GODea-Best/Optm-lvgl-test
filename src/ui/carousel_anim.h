#ifndef CAROUSEL_ANIM_H
#define CAROUSEL_ANIM_H

#include "lvgl/lvgl.h"

/*
 * carousel_anim.h
 *
 * Galaxy Watch style horizontal depth-push transition:
 *
 *   Outgoing screen: slides left (-40px) + shrinks (100%->80%) + fades out
 *   Incoming screen: slides in from right (+128px->0) + grows (80%->100%) + fades in
 *
 * Both run simultaneously. Uses lv_snapshot_take() for pixel-perfect content.
 *
 * REQUIRES in lv_conf.h:
 *   #define LV_USE_SNAPSHOT  1
 */

#define TRANSITION_MS   620
#define SCREEN_W        128     /* match your display width  */
#define SCREEN_H        128     /* match your display height */
#define SLIDE_OFFSET    56      /* how far the old screen slides left (deeper parallax) */
#define SCALE_OUT       (int32_t)(LV_SCALE_NONE * 68 / 100)  /* 68% — stronger perspective */
#define OPA_DIM         (lv_opa_t)(LV_OPA_COVER * 20 / 100)  /* old screen dims to 20% */

typedef void (*screen_create_fn_t)(lv_obj_t * old_scr);

typedef struct {
    lv_draw_buf_t    * snap_old;
    lv_draw_buf_t    * snap_new;
    lv_obj_t         * img_old;
    lv_obj_t         * img_new;
    lv_obj_t         * overlay;
    lv_obj_t         * old_scr;
    lv_obj_t         * new_scr;
    lv_display_t     * disp_main;
    lv_display_t     * disp_ctrl;
    lv_display_t     * disp_zoom;   /* optional zoom mirror — NULL if unused */
    screen_create_fn_t create_fn;   /* used to recreate screen on zoom after anim */
    int                anims_done;  /* 6 total: opa x2, scale x2, x x2 */
} depth_ctx_t;

/* ------------------------------------------------------------------ */
/* Anim exec callbacks                                                 */
/* ------------------------------------------------------------------ */

static void _exec_opa(void * obj, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)v, LV_PART_MAIN);
}

static void _exec_scale(void * obj, int32_t v)
{
    lv_obj_set_style_transform_scale((lv_obj_t *)obj, (int16_t)v, LV_PART_MAIN);
}

static void _exec_x(void * obj, int32_t v)
{
    lv_obj_set_x((lv_obj_t *)obj, (int32_t)v);
}

/* ------------------------------------------------------------------ */
/* Cleanup                                                             */
/* ------------------------------------------------------------------ */

static void _anim_done(lv_anim_t * a)
{
    depth_ctx_t * ctx = (depth_ctx_t *)a->user_data;
    if(!ctx) return;

    ctx->anims_done++;
    if(ctx->anims_done < 6) return;   /* wait for all 6 */

    lv_display_set_default(ctx->disp_main);

    lv_scr_load(ctx->new_scr);
    lv_obj_delete(ctx->overlay);

    if(ctx->snap_old) lv_draw_buf_destroy(ctx->snap_old);
    if(ctx->snap_new) lv_draw_buf_destroy(ctx->snap_new);

    lv_obj_delete(ctx->old_scr);

    /* Mirror the new screen to the zoom display (instant load, no animation) */
    if(ctx->disp_zoom) {
        lv_display_set_default(ctx->disp_zoom);
        lv_obj_t * old_zoom_scr = lv_screen_active();
        ctx->create_fn(NULL);
        lv_obj_delete(old_zoom_scr);
    }

    lv_display_set_default(ctx->disp_ctrl);

    lv_free(ctx);
}

/* ------------------------------------------------------------------ */
/* Helper: start one animation                                         */
/* ------------------------------------------------------------------ */

static void _make_anim(lv_obj_t * obj,
                        lv_anim_exec_xcb_t exec_cb,
                        int32_t start, int32_t end,
                        uint32_t duration,
                        lv_anim_path_cb_t path,
                        depth_ctx_t * ctx)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, exec_cb);
    lv_anim_set_values(&a, start, end);
    lv_anim_set_duration(&a, duration);
    lv_anim_set_path_cb(&a, path);
    lv_anim_set_user_data(&a, ctx);
    lv_anim_set_completed_cb(&a, _anim_done);
    lv_anim_start(&a);
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                  */
/* ------------------------------------------------------------------ */

static inline void carousel_transition(screen_create_fn_t create_fn,
                                        lv_display_t * disp_main,
                                        lv_display_t * disp_ctrl,
                                        lv_display_t * disp_zoom)
{
    lv_display_set_default(disp_main);

    lv_obj_t * old_scr = lv_screen_active();

    /* Snapshot outgoing screen */
    lv_draw_buf_t * snap_old = lv_snapshot_take(
        old_scr, LV_COLOR_FORMAT_ARGB8888);

    if(!snap_old) {
        /* Fallback: instant switch */
        create_fn(NULL);
        if(disp_zoom) {
            lv_display_set_default(disp_zoom);
            lv_obj_t * old_z = lv_screen_active();
            create_fn(NULL);
            lv_obj_delete(old_z);
        }
        lv_display_set_default(disp_ctrl);
        return;
    }

    /* Build incoming screen */
    create_fn(NULL);
    lv_obj_t * new_scr = lv_screen_active();

    if(new_scr == old_scr) {
        lv_draw_buf_destroy(snap_old);
        lv_display_set_default(disp_ctrl);
        return;
    }

    /* Snapshot incoming screen */
    lv_draw_buf_t * snap_new = lv_snapshot_take(
        new_scr, LV_COLOR_FORMAT_ARGB8888);

    if(!snap_new) {
        lv_draw_buf_destroy(snap_old);
        lv_display_set_default(disp_ctrl);
        return;
    }

    /* Restore old screen while we animate */
    lv_scr_load(old_scr);

    /* Allocate context */
    depth_ctx_t * ctx = (depth_ctx_t *)lv_malloc(sizeof(depth_ctx_t));
    ctx->snap_old   = snap_old;
    ctx->snap_new   = snap_new;
    ctx->old_scr    = old_scr;
    ctx->new_scr    = new_scr;
    ctx->disp_main  = disp_main;
    ctx->disp_ctrl  = disp_ctrl;
    ctx->disp_zoom  = disp_zoom;
    ctx->create_fn  = create_fn;
    ctx->anims_done = 0;

    /* Build overlay on top layer — solid black background, always visible */
    lv_obj_t * top = lv_layer_top();

    lv_obj_t * overlay = lv_obj_create(top);
    lv_obj_set_size(overlay, SCREEN_W, SCREEN_H);
    lv_obj_set_pos(overlay, 0, 0);
    lv_obj_set_style_bg_color(overlay, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(overlay, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(overlay, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(overlay, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(overlay, 0, LV_PART_MAIN);
    ctx->overlay = overlay;

    /* Helper macro: card container with radius clips snapshot to rounded corners */
    #define MAKE_CARD(parent) ({                                                   \
        lv_obj_t * _c = lv_obj_create(parent);                                    \
        lv_obj_set_size(_c, SCREEN_W, SCREEN_H);                                  \
        lv_obj_set_style_radius(_c, 15, LV_PART_MAIN);                            \
        lv_obj_set_style_clip_corner(_c, true, LV_PART_MAIN);                     \
        lv_obj_set_style_bg_opa(_c, LV_OPA_TRANSP, LV_PART_MAIN);                 \
        lv_obj_set_style_border_width(_c, 0, LV_PART_MAIN);                       \
        lv_obj_set_style_pad_all(_c, 0, LV_PART_MAIN);                            \
        lv_obj_set_style_transform_pivot_x(_c, SCREEN_W / 2, LV_PART_MAIN);      \
        lv_obj_set_style_transform_pivot_y(_c, SCREEN_H / 2, LV_PART_MAIN);      \
        _c;                                                                        \
    })

    /* ---- Old card: starts at x=0, full size, full opacity ---- */
    lv_obj_t * img_old = MAKE_CARD(overlay);
    lv_obj_set_pos(img_old, 0, 0);
    lv_obj_set_style_opa(img_old, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_transform_scale(img_old, LV_SCALE_NONE, LV_PART_MAIN);
    lv_obj_t * _img_old_img = lv_image_create(img_old);
    lv_image_set_src(_img_old_img, snap_old);
    lv_obj_set_pos(_img_old_img, 0, 0);
    ctx->img_old = img_old;

    /* ---- New card: starts off-screen right, small, transparent ---- */
    lv_obj_t * img_new = MAKE_CARD(overlay);
    lv_obj_set_pos(img_new, SCREEN_W, 0);
    lv_obj_set_style_opa(img_new, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_transform_scale(img_new, SCALE_OUT, LV_PART_MAIN);
    lv_obj_t * _img_new_img = lv_image_create(img_new);
    lv_image_set_src(_img_new_img, snap_new);
    lv_obj_set_pos(_img_new_img, 0, 0);

    /* new card renders on top of old */
    lv_obj_move_to_index(img_new, lv_obj_get_child_count(overlay) - 1);
    ctx->img_new = img_new;

    /* ---------------------------------------------------------------- */
    /* Fire all 6 animations simultaneously                              */
    /* ---------------------------------------------------------------- */

    /* OLD: slide left by SLIDE_OFFSET */
    _make_anim(img_old, _exec_x,
               0, -SLIDE_OFFSET,
               TRANSITION_MS, lv_anim_path_ease_in_out, ctx);

    /* OLD: shrink */
    _make_anim(img_old, _exec_scale,
               LV_SCALE_NONE, SCALE_OUT,
               TRANSITION_MS, lv_anim_path_ease_in_out, ctx);

    /* OLD: dim to 20% (not fully transparent — gives depth layering) */
    _make_anim(img_old, _exec_opa,
               LV_OPA_COVER, OPA_DIM,
               TRANSITION_MS, lv_anim_path_ease_in_out, ctx);

    /* NEW: slide in from right to x=0 */
    _make_anim(img_new, _exec_x,
               SCREEN_W, 0,
               TRANSITION_MS, lv_anim_path_ease_in_out, ctx);

    /* NEW: grow with slight overshoot (ease_out gives deceleration — card "lands") */
    _make_anim(img_new, _exec_scale,
               SCALE_OUT, LV_SCALE_NONE,
               TRANSITION_MS, lv_anim_path_ease_out, ctx);

    /* NEW: fade in faster than slide so it appears to emerge from behind */
    _make_anim(img_new, _exec_opa,
               LV_OPA_TRANSP, LV_OPA_COVER,
               TRANSITION_MS * 7 / 10, lv_anim_path_ease_out, ctx);

    lv_display_set_default(disp_ctrl);
}

#endif /* CAROUSEL_ANIM_H */
