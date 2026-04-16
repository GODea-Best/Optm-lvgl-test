#include "lvgl/lvgl.h"
#include "screen_blow_prompt.h"
#include "device_config.h"
#include "fonts_bold.h"

/*
 * Blow Prompt screen
 *
 * Background: linear-gradient(90deg, #1400CA 0%, #000000 50%)
 *   Blue at left edge, black by the midpoint, solid black right half.
 *   Implemented as a vector gradient line from (0,0)→(64,0): blue→black.
 * "BLOW EVENLY"  — ExtraBold 24, white, 103×60 at left=12, top=14
 * Instructional  — Light 16, white, 98×38 at left=15, top=76
 */

typedef struct {
    lv_color_t c1;
    lv_color_t c2;
} _blow_grad_ctx_t;

static void _blow_grad_draw_cb(lv_event_t * e)
{
    _blow_grad_ctx_t * g     = (_blow_grad_ctx_t *)lv_event_get_user_data(e);
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

    /* Gradient line from x=0 (blue) to x=W/2 (black): matches CSS 0%→50% */
    lv_draw_vector_dsc_set_fill_linear_gradient(dsc, 0, 0, W / 2.0f, 0);

    lv_grad_stop_t stops[2];
    stops[0].color = g->c1; stops[0].opa = LV_OPA_COVER; stops[0].frac =   0;
    stops[1].color = g->c2; stops[1].opa = LV_OPA_COVER; stops[1].frac = 255;
    lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);

    lv_draw_vector_dsc_add_path(dsc, path);
    lv_draw_vector(dsc);
    lv_draw_vector_dsc_delete(dsc);
    lv_vector_path_delete(path);
}

static void _blow_grad_delete_cb(lv_event_t * e)
{
    lv_free(lv_event_get_user_data(e));
}

void screen_blow_prompt_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, LV_PART_MAIN);

    _blow_grad_ctx_t * g = (_blow_grad_ctx_t *)lv_malloc(sizeof(_blow_grad_ctx_t));
    g->c1 = lv_color_hex(0x1400CA);
    g->c2 = lv_color_hex(0x000000);
    lv_obj_add_event_cb(scr, _blow_grad_draw_cb,   LV_EVENT_DRAW_MAIN, g);
    lv_obj_add_event_cb(scr, _blow_grad_delete_cb, LV_EVENT_DELETE,    g);

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
