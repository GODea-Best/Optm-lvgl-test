#ifndef GRADIENT_BG_H
#define GRADIENT_BG_H

#include "lvgl/lvgl.h"

/* Used for background gradients — vertical (top→bottom) or horizontal (left→right) */
typedef struct {
    lv_color_t   color_start;
    lv_color_t   color_end;
    int32_t      radius;
    lv_grad_dir_t dir;
} gradient_bg_t;

static void _gradient_draw_cb(lv_event_t * e)
{
    gradient_bg_t * g     = (gradient_bg_t *)lv_event_get_user_data(e);
    lv_layer_t    * layer = lv_event_get_layer(e);
    lv_obj_t      * obj   = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);
    lv_draw_rect_dsc_t dsc;
    lv_draw_rect_dsc_init(&dsc);
    dsc.bg_color                = g->color_start;
    dsc.bg_grad.dir             = g->dir;
    dsc.bg_grad.stops[0].color  = g->color_start;
    dsc.bg_grad.stops[0].opa    = LV_OPA_COVER;
    dsc.bg_grad.stops[0].frac   = 0;
    dsc.bg_grad.stops[1].color  = g->color_end;
    dsc.bg_grad.stops[1].opa    = LV_OPA_COVER;
    dsc.bg_grad.stops[1].frac   = 255;
    dsc.bg_grad.stops_count     = 2;
    dsc.bg_opa                  = LV_OPA_COVER;
    dsc.radius                  = g->radius;
    dsc.border_width            = 0;
    lv_draw_rect(layer, &dsc, &coords);
}

static void _gradient_delete_cb(lv_event_t * e)
{
    gradient_bg_t * g = (gradient_bg_t *)lv_event_get_user_data(e);
    lv_free(g);
}

/* Vertical gradient (top → bottom) — used for screen/panel backgrounds */
static inline void gradient_bg_attach(lv_obj_t * obj,
                                       lv_color_t top,
                                       lv_color_t bottom,
                                       int32_t radius)
{
    gradient_bg_t * g = (gradient_bg_t *)lv_malloc(sizeof(gradient_bg_t));
    g->color_start  = top;
    g->color_end    = bottom;
    g->radius       = radius;
    g->dir          = LV_GRAD_DIR_VER;
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_add_event_cb(obj, _gradient_draw_cb,   LV_EVENT_DRAW_MAIN, g);
    lv_obj_add_event_cb(obj, _gradient_delete_cb, LV_EVENT_DELETE,    g);
}

/* Horizontal gradient (left → right) — used for bottom accent bars */
static inline void gradient_bg_attach_hor(lv_obj_t * obj,
                                           lv_color_t left,
                                           lv_color_t right,
                                           int32_t radius)
{
    gradient_bg_t * g = (gradient_bg_t *)lv_malloc(sizeof(gradient_bg_t));
    g->color_start  = left;
    g->color_end    = right;
    g->radius       = radius;
    g->dir          = LV_GRAD_DIR_HOR;
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_add_event_cb(obj, _gradient_draw_cb,   LV_EVENT_DRAW_MAIN, g);
    lv_obj_add_event_cb(obj, _gradient_delete_cb, LV_EVENT_DELETE,    g);
}

#endif /* GRADIENT_BG_H */
