#include "lvgl/lvgl.h"
#include "screen_free_restart_icons.h"
#include "device_config.h"
#include "accent_bar.h"

/* Free Restart — icon version
 * Refresh icon (restart window) + live countdown timer. */

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

typedef struct { lv_obj_t * lbl; lv_timer_t * timer; uint32_t cs; } _ctx_t;

static void _tick_cb(lv_timer_t * t)
{
    _ctx_t * ctx = (_ctx_t *)lv_timer_get_user_data(t);
    if(ctx->cs > 0) ctx->cs--;
    char buf[16]; _fmt_msh(ctx->cs, buf);
    lv_label_set_text(ctx->lbl, buf);
}

static void _del_cb(lv_event_t * e)
{
    _ctx_t * ctx = (_ctx_t *)lv_event_get_user_data(e);
    lv_timer_delete(ctx->timer);
    lv_free(ctx);
}

void screen_free_restart_icons_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* Refresh icon — restart window concept */
    lv_obj_t * lbl_icon = lv_label_create(scr);
    lv_label_set_text(lbl_icon, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_color(lbl_icon, lv_color_hex(0x1D9101), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_icon, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_align(lbl_icon, LV_ALIGN_CENTER, 0, -22);

    /* Countdown timer */
    lv_obj_t * lbl_time = lv_label_create(scr);
    lv_label_set_text(lbl_time, CFG_FREE_RESTART_TIME);
    lv_obj_set_width(lbl_time, 120);
    lv_obj_set_style_text_align(lbl_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_color(lbl_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_time, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(lbl_time, LV_ALIGN_CENTER, 0, 32);

    _ctx_t * ctx = (_ctx_t *)lv_malloc(sizeof(_ctx_t));
    ctx->lbl   = lbl_time;
    ctx->cs    = _parse_msh(CFG_FREE_RESTART_TIME);
    ctx->timer = lv_timer_create(_tick_cb, 10, ctx);
    lv_obj_add_event_cb(scr, _del_cb, LV_EVENT_DELETE, ctx);

    ACCENT_BAR_CREATE(scr, true, lv_color_hex(0x1D9101), lv_color_hex(0x092B00));

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
