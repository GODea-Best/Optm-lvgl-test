#include "lvgl/lvgl.h"
#include "screen_retest_alert_icons.h"
#include "device_config.h"
#include "gradient_bg.h"
#include "wind_svg_data.h"

/* Retest Alert — icon version
 * 4-panel cycling (900ms), same structure as text version.
 * Panel 0 (dark):   WARNING symbol  — retest required
 * Panel 1 (orange): WARNING symbol  — retest required (inverted)
 * Panel 2 (dark):   Wind SVG icon   — provide sample
 * Panel 3 (orange): VOLUME symbol   — provide sample (inverted, wind is white)
 * Timer kept on all panels. */

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

#define NUM_PANELS 4

typedef struct {
    lv_obj_t   * panels[NUM_PANELS];
    lv_obj_t   * lbl_times[NUM_PANELS];
    uint8_t      current;
    uint32_t     cs;
    lv_timer_t * toggle_timer;
    lv_timer_t * countdown_timer;
} _rta_ctx_t;

static void _toggle_cb(lv_timer_t * t)
{
    _rta_ctx_t * ctx = (_rta_ctx_t *)lv_timer_get_user_data(t);
    lv_obj_add_flag(ctx->panels[ctx->current], LV_OBJ_FLAG_HIDDEN);
    ctx->current = (uint8_t)((ctx->current + 1) % NUM_PANELS);
    lv_obj_clear_flag(ctx->panels[ctx->current], LV_OBJ_FLAG_HIDDEN);
}

static void _countdown_cb(lv_timer_t * t)
{
    _rta_ctx_t * ctx = (_rta_ctx_t *)lv_timer_get_user_data(t);
    if(ctx->cs > 0) ctx->cs--;
    char buf[16]; _fmt_msh(ctx->cs, buf);
    for(int i = 0; i < NUM_PANELS; i++)
        lv_label_set_text(ctx->lbl_times[i], buf);
}

static void _del_cb(lv_event_t * e)
{
    _rta_ctx_t * ctx = (_rta_ctx_t *)lv_event_get_user_data(e);
    lv_timer_delete(ctx->toggle_timer);
    lv_timer_delete(ctx->countdown_timer);
    lv_free(ctx);
}

static lv_obj_t * _make_panel(lv_obj_t * scr, bool hidden, bool orange)
{
    lv_obj_t * p = lv_obj_create(scr);
    lv_obj_set_size(p, 128, 128);
    lv_obj_set_pos(p, 0, 0);
    lv_obj_set_style_border_width(p, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(p, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(p, 15, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(p, true, LV_PART_MAIN);
    if(orange) {
        gradient_bg_attach(p, lv_color_hex(0xFF4400), lv_color_hex(0xFF9D00), 15);
    } else {
        lv_obj_set_style_bg_color(p, lv_color_hex(0x000000), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(p, LV_OPA_COVER, LV_PART_MAIN);
    }
    if(hidden) lv_obj_add_flag(p, LV_OBJ_FLAG_HIDDEN);
    return p;
}

static lv_obj_t * _add_timer(lv_obj_t * p, lv_color_t col)
{
    lv_obj_t * lbl = lv_label_create(p);
    lv_label_set_text(lbl, CFG_RETEST_TIME);
    lv_obj_set_width(lbl, 120);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl, col, LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 36);
    return lbl;
}

void screen_retest_alert_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    _rta_ctx_t * ctx = (_rta_ctx_t *)lv_malloc(sizeof(_rta_ctx_t));
    ctx->current = 0;
    ctx->cs      = _parse_msh(CFG_RETEST_TIME);

    /* Panel 0: dark — WARNING (retest required) */
    lv_obj_t * p0 = _make_panel(scr, false, false);
    lv_obj_t * lbl0 = lv_label_create(p0);
    lv_label_set_text(lbl0, LV_SYMBOL_WARNING);
    lv_obj_set_style_text_color(lbl0, lv_color_hex(0xFF9D00), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl0, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_align(lbl0, LV_ALIGN_CENTER, 0, -18);
    ctx->lbl_times[0] = _add_timer(p0, lv_color_hex(0xFFFFFF));
    ctx->panels[0] = p0;

    /* Panel 1: orange — WARNING (retest required, inverted) */
    lv_obj_t * p1 = _make_panel(scr, true, true);
    lv_obj_t * lbl1 = lv_label_create(p1);
    lv_label_set_text(lbl1, LV_SYMBOL_WARNING);
    lv_obj_set_style_text_color(lbl1, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl1, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_align(lbl1, LV_ALIGN_CENTER, 0, -18);
    ctx->lbl_times[1] = _add_timer(p1, lv_color_hex(0x000000));
    ctx->panels[1] = p1;

    /* Panel 2: dark — Wind SVG (provide sample) */
    lv_obj_t * p2 = _make_panel(scr, true, false);
    lv_obj_t * img2 = lv_image_create(p2);
    lv_obj_set_size(img2, 60, 60);
    lv_image_set_src(img2, &wind_svg_dsc);
    lv_image_set_align(img2, LV_IMAGE_ALIGN_CONTAIN);
    lv_obj_align(img2, LV_ALIGN_CENTER, 0, -18);
    ctx->lbl_times[2] = _add_timer(p2, lv_color_hex(0xFFFFFF));
    ctx->panels[2] = p2;

    /* Panel 3: orange — VOLUME_MAX symbol (wind/blow on orange bg) */
    lv_obj_t * p3 = _make_panel(scr, true, true);
    lv_obj_t * lbl3 = lv_label_create(p3);
    lv_label_set_text(lbl3, LV_SYMBOL_VOLUME_MAX);
    lv_obj_set_style_text_color(lbl3, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl3, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_align(lbl3, LV_ALIGN_CENTER, 0, -18);
    ctx->lbl_times[3] = _add_timer(p3, lv_color_hex(0x000000));
    ctx->panels[3] = p3;

    ctx->toggle_timer    = lv_timer_create(_toggle_cb,    900, ctx);
    ctx->countdown_timer = lv_timer_create(_countdown_cb,  10, ctx);
    lv_obj_add_event_cb(scr, _del_cb, LV_EVENT_DELETE, ctx);

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
