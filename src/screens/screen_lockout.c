#include "lvgl/lvgl.h"
#include "screen_lockout.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "lock_solid_svg_data.h"

/*
 * Lockout screen
 *
 * A red "corner glow" (matching the Figma: #CA0000→#000000 diagonal) sweeps
 * from top-left to bottom-right, leaving black in its wake, then loops.
 *
 * Technique: linear gradient whose START point (red) is animated from
 * (-128,-128) to (256,256). The END point follows 180px behind it.
 * When the start is off the bottom-right corner the screen is all black —
 * a 700ms playback_delay holds that darkness before repeating.
 */

typedef struct {
    lv_obj_t * obj;
    int32_t    offset;   /* 0..384: maps to t = -128..256 */
} _lockout_ctx_t;

static void _lockout_draw_cb(lv_event_t * e)
{
    _lockout_ctx_t * g     = (_lockout_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t     * layer = lv_event_get_layer(e);
    lv_obj_t       * obj   = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    float W = (float)(coords.x2 - coords.x1 + 1);
    float H = (float)(coords.y2 - coords.y1 + 1);

    /* t: -128 → 256 as offset: 0 → 384 */
    float t = (float)g->offset - 128.0f;

    /* Red origin sweeps diagonally; fade length ~180px along diagonal */
    float gx1 = t,         gy1 = t;          /* bright red point */
    float gx2 = t + 180.0f, gy2 = t + 180.0f; /* fades to black here */

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

    lv_draw_vector_dsc_set_fill_linear_gradient(dsc, gx1, gy1, gx2, gy2);

    /* Fade red colour toward black over the last 100 units of travel (offset 284→384).
     * Both stops stay fully opaque so the fill is always solid — no transparency bleed. */
    uint8_t red_ch;
    if(g->offset >= 284) {
        red_ch = (uint8_t)(0xCA * (384 - g->offset) / 100);
    } else {
        red_ch = 0xCA;
    }

    lv_grad_stop_t stops[2];
    stops[0].color = lv_color_make(red_ch, 0, 0); stops[0].opa = LV_OPA_COVER; stops[0].frac =   0;
    stops[1].color = lv_color_hex(0x000000);       stops[1].opa = LV_OPA_COVER; stops[1].frac = 255;
    lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);

    lv_draw_vector_dsc_add_path(dsc, path);
    lv_draw_vector(dsc);
    lv_draw_vector_dsc_delete(dsc);
    lv_vector_path_delete(path);
}

static void _lockout_delete_cb(lv_event_t * e)
{
    _lockout_ctx_t * g = (_lockout_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(g, NULL);
    lv_free(g);
}

static void _lockout_anim_exec(void * var, int32_t v)
{
    _lockout_ctx_t * g = (_lockout_ctx_t *)var;
    g->offset = v;
    lv_obj_invalidate(g->obj);
}

void screen_lockout_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, LV_PART_MAIN);

    _lockout_ctx_t * g = (_lockout_ctx_t *)lv_malloc(sizeof(_lockout_ctx_t));
    g->obj    = scr;
    g->offset = 0;

    lv_obj_add_event_cb(scr, _lockout_draw_cb,   LV_EVENT_DRAW_MAIN, g);
    lv_obj_add_event_cb(scr, _lockout_delete_cb, LV_EVENT_DELETE,    g);

    /* Sweep: red gradient crosses screen diagonally, black trails behind it.
     * Loops immediately — red re-enters top-left as soon as it exits bottom-right,
     * giving a continuous red→black→red→black cycle. */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _lockout_anim_exec);
    lv_anim_set_var(&a, g);
    lv_anim_set_values(&a, 0, 384);
    lv_anim_set_duration(&a, 1800);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 400);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_start(&a);

    /* "LOCKOUT" — ExtraBold 16, single line, centered */
    lv_obj_t * lbl_head = lv_label_create(scr);
    lv_label_set_text(lbl_head, "LOCKOUT");
    lv_label_set_long_mode(lbl_head, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(lbl_head, 112);
    lv_obj_align(lbl_head, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_text_color(lbl_head, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_head, &lv_font_montserrat_extrabold_16, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_head, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    /* Lock icon — 29×29 */
    lv_obj_t * img = lv_image_create(scr);
    lv_obj_set_size(img, 29, 29);
    lv_image_set_src(img, &lock_solid_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 54);

    /* Instructional text — Light 12, explicit \n only, no auto-wrap */
    lv_obj_t * lbl_msg = lv_label_create(scr);
    lv_label_set_text(lbl_msg, CFG_LOCKOUT_DESC);
    lv_label_set_long_mode(lbl_msg, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(lbl_msg, 112);
    lv_obj_set_height(lbl_msg, LV_SIZE_CONTENT);
    lv_obj_align(lbl_msg, LV_ALIGN_TOP_MID, 0, 88);
    lv_obj_set_style_text_color(lbl_msg, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_msg, &lv_font_montserrat_light_12, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_msg, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_line_space(lbl_msg, 3, LV_PART_MAIN);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
