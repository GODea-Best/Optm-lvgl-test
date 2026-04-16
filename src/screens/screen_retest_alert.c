#include "lvgl/lvgl.h"
#include "screen_retest_alert.h"
#include "device_config.h"
#include "gradient_bg.h"
#include "fonts_bold.h"

/*
 * Retest Alert screen  — four panels, cycling every 900 ms:
 *
 *   0  RETEST REQUIRED  — black bg, white text, orange gradient bar
 *   1  RETEST REQUIRED  — orange gradient bg, black text, black bar
 *   2  PROVIDE SAMPLE   — black bg, white text, orange gradient bar
 *   3  PROVIDE SAMPLE   — orange gradient bg, black text, black bar
 *
 * A single countdown timer (1 s) drives all four time labels from the
 * same ctx->secs counter.
 */

/* ------------------------------------------------------------------ */
/* Time helpers                                                        */
/* ------------------------------------------------------------------ */

static uint32_t _parse_msh(const char * s)
{
    while(*s == ' ') s++;
    uint32_t m  = (uint32_t)(s[0]-'0')*10 + (uint32_t)(s[1]-'0'); s += 2;
    while(*s == ' ' || *s == ':') s++;
    uint32_t sec = (uint32_t)(s[0]-'0')*10 + (uint32_t)(s[1]-'0'); s += 2;
    while(*s == ' ' || *s == '.') s++;
    uint32_t hh = (uint32_t)(s[0]-'0')*10 + (uint32_t)(s[1]-'0');
    return m * 6000u + sec * 100u + hh;
}

static void _fmt_msh(uint32_t cs, char * buf)
{
    int m  = (int)(cs / 6000);
    int s  = (int)((cs % 6000) / 100);
    int hh = (int)(cs % 100);
    lv_snprintf(buf, 16, "%02d:%02d.%02d", m, s, hh);
}

/* ------------------------------------------------------------------ */
/* Accent bar: per-corner elliptical radii via vector path             */
/*                                                                     */
/* CSS source:  width:9px; height:113px;                               */
/*              border-radius: 4px 14px 14px 4px;                     */
/*              transform: rotate(90deg);                              */
/*                                                                     */
/* After clamping and 90° CW rotation:                                 */
/*   top corners  → rx=4, ry=2                                        */
/*   bottom corners → rx=14, ry=7                                     */
/* ------------------------------------------------------------------ */

typedef struct {
    float      rt_rx, rt_ry;
    float      rb_rx, rb_ry;
    bool       gradient;
    lv_color_t color1;
    lv_color_t color2;
} bar_ctx_t;

static void _bar_draw_cb(lv_event_t * e)
{
    bar_ctx_t  * bc    = (bar_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t * layer = lv_event_get_layer(e);
    lv_obj_t   * obj   = lv_event_get_target(e);
    lv_area_t    coords;
    lv_obj_get_coords(obj, &coords);

    float W = (float)lv_obj_get_width(obj);
    float H = (float)lv_obj_get_height(obj);

    lv_vector_path_t * path =
        lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);

    lv_fpoint_t p;
    p = (lv_fpoint_t){bc->rt_rx, 0};              lv_vector_path_move_to(path, &p);
    p = (lv_fpoint_t){W - bc->rt_rx, 0};          lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){W, bc->rt_ry};              lv_vector_path_arc_to(path, bc->rt_rx, bc->rt_ry, 0, false, true, &p);
    p = (lv_fpoint_t){W, H - bc->rb_ry};          lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){W - bc->rb_rx, H};          lv_vector_path_arc_to(path, bc->rb_rx, bc->rb_ry, 0, false, true, &p);
    p = (lv_fpoint_t){bc->rb_rx, H};              lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){0, H - bc->rb_ry};          lv_vector_path_arc_to(path, bc->rb_rx, bc->rb_ry, 0, false, true, &p);
    p = (lv_fpoint_t){0, bc->rt_ry};              lv_vector_path_line_to(path, &p);
    p = (lv_fpoint_t){bc->rt_rx, 0};              lv_vector_path_arc_to(path, bc->rt_rx, bc->rt_ry, 0, false, true, &p);
    lv_vector_path_close(path);

    lv_draw_vector_dsc_t * dsc = lv_draw_vector_dsc_create(layer);
    lv_matrix_t m;
    lv_matrix_identity(&m);
    lv_matrix_translate(&m, (float)coords.x1, (float)coords.y1);
    lv_draw_vector_dsc_set_transform(dsc, &m);

    if(bc->gradient) {
        lv_draw_vector_dsc_set_fill_linear_gradient(dsc, 0, 0, W, 0);
        lv_grad_stop_t stops[2];
        stops[0].color = bc->color1;  stops[0].opa = LV_OPA_COVER;  stops[0].frac = 0;
        stops[1].color = bc->color2;  stops[1].opa = LV_OPA_COVER;  stops[1].frac = 255;
        lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc, stops, 2);
    } else {
        lv_draw_vector_dsc_set_fill_color(dsc, bc->color1);
        lv_draw_vector_dsc_set_fill_opa(dsc, LV_OPA_COVER);
    }

    lv_draw_vector_dsc_add_path(dsc, path);
    lv_draw_vector(dsc);
    lv_draw_vector_dsc_delete(dsc);
    lv_vector_path_delete(path);
}

static void _bar_delete_cb(lv_event_t * e)
{
    lv_free(lv_event_get_user_data(e));
}

static void _attach_bar(lv_obj_t * obj,
                         float rt_rx, float rt_ry,
                         float rb_rx, float rb_ry,
                         bool gradient,
                         lv_color_t color1, lv_color_t color2)
{
    bar_ctx_t * bc  = (bar_ctx_t *)lv_malloc(sizeof(bar_ctx_t));
    bc->rt_rx = rt_rx;  bc->rt_ry = rt_ry;
    bc->rb_rx = rb_rx;  bc->rb_ry = rb_ry;
    bc->gradient = gradient;
    bc->color1 = color1;  bc->color2 = color2;
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(obj, _bar_draw_cb,   LV_EVENT_DRAW_MAIN, bc);
    lv_obj_add_event_cb(obj, _bar_delete_cb, LV_EVENT_DELETE,    bc);
}

/* ------------------------------------------------------------------ */
/* Cycling context                                                     */
/* ------------------------------------------------------------------ */

#define NUM_PANELS 4

typedef struct {
    lv_obj_t   * panels[NUM_PANELS];
    lv_obj_t   * lbl_times[NUM_PANELS];
    uint8_t      current;
    uint32_t     cs;
    lv_timer_t * toggle_timer;
    lv_timer_t * countdown_timer;
} retest_ctx_t;

static void _toggle_cb(lv_timer_t * t)
{
    retest_ctx_t * ctx = (retest_ctx_t *)lv_timer_get_user_data(t);
    lv_obj_add_flag(ctx->panels[ctx->current], LV_OBJ_FLAG_HIDDEN);
    ctx->current = (uint8_t)((ctx->current + 1) % NUM_PANELS);
    lv_obj_clear_flag(ctx->panels[ctx->current], LV_OBJ_FLAG_HIDDEN);
}

static void _countdown_cb(lv_timer_t * t)
{
    retest_ctx_t * ctx = (retest_ctx_t *)lv_timer_get_user_data(t);
    if(ctx->cs > 0) ctx->cs--;
    char buf[16];
    _fmt_msh(ctx->cs, buf);
    for(int i = 0; i < NUM_PANELS; i++) {
        lv_label_set_text(ctx->lbl_times[i], buf);
    }
}

static void _scr_delete_cb(lv_event_t * e)
{
    retest_ctx_t * ctx = (retest_ctx_t *)lv_event_get_user_data(e);
    lv_timer_delete(ctx->toggle_timer);
    lv_timer_delete(ctx->countdown_timer);
    lv_free(ctx);
}

/* ------------------------------------------------------------------ */
/* Helpers                                                             */
/* ------------------------------------------------------------------ */

static lv_obj_t * _add_text(lv_obj_t * parent,
                             const char * text,
                             int32_t x, int32_t y,
                             const lv_font_t * font,
                             lv_color_t color,
                             int32_t line_space)
{
    (void)x; /* x unused — label is horizontally centred */
    lv_obj_t * lbl = lv_label_create(parent);
    lv_label_set_text(lbl, text);
    lv_obj_set_width(lbl, 120);
    lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_set_style_text_color(lbl, color, LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl, font, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, 0);
    if(line_space) lv_obj_set_style_text_line_space(lbl, line_space, LV_PART_MAIN);
    return lbl;
}

/* Build a full-size panel on 'scr', optionally hidden, optionally gradient bg. */
static lv_obj_t * _make_panel(lv_obj_t * scr, bool hidden, bool orange_grad)
{
    lv_obj_t * p = lv_obj_create(scr);
    lv_obj_set_size(p, 128, 128);
    lv_obj_set_pos(p, 0, 0);
    lv_obj_set_style_border_width(p, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(p, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(p, 15, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(p, true, LV_PART_MAIN);
    if(orange_grad) {
        gradient_bg_attach(p, lv_color_hex(0xFF4400), lv_color_hex(0xFF9D00), 15);
    } else {
        lv_obj_set_style_bg_color(p, lv_color_hex(0x000000), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(p, LV_OPA_COVER, LV_PART_MAIN);
    }
    if(hidden) lv_obj_add_flag(p, LV_OBJ_FLAG_HIDDEN);
    return p;
}

/* Attach the shaped bottom bar to a panel. */
static void _add_panel_bar(lv_obj_t * panel, bool gradient)
{
    lv_obj_t * bar = lv_obj_create(panel);
    lv_obj_set_size(bar, 113, 9);
    lv_obj_set_pos(bar, 7, 113);
    lv_obj_set_style_pad_all(bar, 0, LV_PART_MAIN);
    if(gradient) {
        _attach_bar(bar, 4.0f, 2.0f, 14.0f, 7.0f, true,
                    lv_color_hex(0xFF4400), lv_color_hex(0xFFA600));
    } else {
        _attach_bar(bar, 4.0f, 2.0f, 14.0f, 7.0f, false,
                    lv_color_hex(0x000000), lv_color_hex(0x000000));
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                  */
/* ------------------------------------------------------------------ */

void screen_retest_alert_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    retest_ctx_t * ctx = (retest_ctx_t *)lv_malloc(sizeof(retest_ctx_t));
    ctx->current = 0;
    ctx->cs      = _parse_msh(CFG_RETEST_TIME);

    lv_color_t white = lv_color_hex(0xFFFFFF);
    lv_color_t black = lv_color_hex(0x000000);

    /* ---- Panel 0: RETEST REQUIRED — dark (visible first) ----------- */
    lv_obj_t * p0 = _make_panel(scr, false, false);
    _add_text(p0, "RETEST\nREQUIRED", 11, 15, &lv_font_montserrat_extrabold_18, white, 5);
    _add_text(p0, "TIME REMAINING",  11, 64, &lv_font_montserrat_light_12,  white, 0);
    ctx->lbl_times[0] = _add_text(p0, CFG_RETEST_TIME, 7, 79,
                                   &lv_font_montserrat_semibold_24, white, 0);
    _add_panel_bar(p0, true);
    ctx->panels[0] = p0;

    /* ---- Panel 1: RETEST REQUIRED — orange gradient (hidden) ------- */
    lv_obj_t * p1 = _make_panel(scr, true, true);
    _add_text(p1, "RETEST\nREQUIRED", 14, 18, &lv_font_montserrat_extrabold_18, black, 5);
    _add_text(p1, "TIME REMAINING",  11, 67, &lv_font_montserrat_light_12,  black, 0);
    ctx->lbl_times[1] = _add_text(p1, CFG_RETEST_TIME, 10, 80,
                                   &lv_font_montserrat_semibold_24, black, 0);
    _add_panel_bar(p1, false);
    ctx->panels[1] = p1;

    /* ---- Panel 2: PROVIDE SAMPLE — dark (hidden) ------------------- */
    lv_obj_t * p2 = _make_panel(scr, true, false);
    _add_text(p2, "PROVIDE\nSAMPLE", 11, 15, &lv_font_montserrat_extrabold_18, white, 4);
    _add_text(p2, "TIME REMAINING", 11, 64, &lv_font_montserrat_light_12,  white, 0);
    ctx->lbl_times[2] = _add_text(p2, CFG_RETEST_TIME, 7, 79,
                                   &lv_font_montserrat_semibold_24, white, 0);
    _add_panel_bar(p2, true);
    ctx->panels[2] = p2;

    /* ---- Panel 3: PROVIDE SAMPLE — orange gradient (hidden) -------- */
    lv_obj_t * p3 = _make_panel(scr, true, true);
    _add_text(p3, "PROVIDE\nSAMPLE", 14, 18, &lv_font_montserrat_extrabold_18, black, 4);
    _add_text(p3, "TIME REMAINING", 11, 67, &lv_font_montserrat_light_12,  black, 0);
    ctx->lbl_times[3] = _add_text(p3, CFG_RETEST_TIME, 10, 80,
                                   &lv_font_montserrat_semibold_24, black, 0);
    _add_panel_bar(p3, false);
    ctx->panels[3] = p3;

    /* ---- Timers ---- */
    ctx->toggle_timer    = lv_timer_create(_toggle_cb,    900, ctx);
    ctx->countdown_timer = lv_timer_create(_countdown_cb,  10, ctx);
    lv_obj_add_event_cb(scr, _scr_delete_cb, LV_EVENT_DELETE, ctx);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
