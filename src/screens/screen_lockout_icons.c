#include "lvgl/lvgl.h"
#include "screen_lockout_icons.h"
#include "device_config.h"
#include "lock_solid_svg_data.h"

/* Lockout — icon version
 * Animated diagonal red sweep (same as text version) + large centred lock.
 * No text at all. */

typedef struct {
    lv_obj_t * obj;
    int32_t    offset;
} _lo_ctx_t;

static void _lo_draw_cb(lv_event_t * e)
{
    _lo_ctx_t  * g     = (_lo_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t * layer = lv_event_get_layer(e);
    lv_obj_t   * obj   = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    float W = (float)(coords.x2 - coords.x1 + 1);
    float H = (float)(coords.y2 - coords.y1 + 1);
    float t = (float)g->offset - 128.0f;

    lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);
    lv_fpoint_t p;
    p = (lv_fpoint_t){0, 0}; lv_vector_path_move_to(path, &p);
    p = (lv_fpoint_t){W, 0}; lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){W, H}; lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){0, H}; lv_vector_path_line_to(path, &p);
    lv_vector_path_close(path);

    lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
    lv_matrix_t m; lv_matrix_identity(&m);
    lv_matrix_translate(&m, (float)coords.x1, (float)coords.y1);
    lv_draw_vector_dsc_set_transform(dsc, &m);
    lv_draw_vector_dsc_set_fill_linear_gradient(dsc, t, t, t + 180.0f, t + 180.0f);

    uint8_t red_ch = (g->offset >= 284) ? (uint8_t)(0xCA * (384 - g->offset) / 100) : 0xCA;

    lv_grad_stop_t stops[2];
    stops[0].color = lv_color_make(red_ch, 0, 0); stops[0].opa = LV_OPA_COVER; stops[0].frac =   0;
    stops[1].color = lv_color_hex(0x000000);       stops[1].opa = LV_OPA_COVER; stops[1].frac = 255;
    lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);
    lv_draw_vector_dsc_add_path(dsc, path);
    lv_draw_vector(dsc);
    lv_draw_vector_dsc_delete(dsc);
    lv_vector_path_delete(path);
}

static void _lo_delete_cb(lv_event_t * e)
{
    _lo_ctx_t * g = (_lo_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(g, NULL);
    lv_free(g);
}

static void _lo_anim_exec(void * var, int32_t v)
{
    _lo_ctx_t * g = (_lo_ctx_t *)var;
    g->offset = v;
    lv_obj_invalidate(g->obj);
}

void screen_lockout_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, LV_PART_MAIN);

    _lo_ctx_t * g = (_lo_ctx_t *)lv_malloc(sizeof(_lo_ctx_t));
    g->obj = scr; g->offset = 0;
    lv_obj_add_event_cb(scr, _lo_draw_cb,   LV_EVENT_DRAW_MAIN, g);
    lv_obj_add_event_cb(scr, _lo_delete_cb, LV_EVENT_DELETE,    g);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _lo_anim_exec);
    lv_anim_set_var(&a, g);
    lv_anim_set_values(&a, 0, 384);
    lv_anim_set_duration(&a, 1800);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 400);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_start(&a);

    /* Large lock icon — centred */
    lv_obj_t * img = lv_image_create(scr);
    lv_obj_set_size(img, 64, 64);
    lv_image_set_src(img, &lock_solid_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
