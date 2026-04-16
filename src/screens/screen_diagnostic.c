#include "lvgl/lvgl.h"
#include "screen_diagnostic.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "wind_svg_data.h"

/*
 * Test/Diagnostic screen
 *
 * Black background.
 * Two concentric lv_arc rings (left-of-centre, masked top-right):
 *   Outer 106×106 at (10,12): 6px track #272727, fill #648C4E → Volume %
 *   Inner  83×83  at (22,23): 6px track #272727, fill #648C4E → Duration %
 * Black rectangle 62×64 at (66,0) masks the top-right quadrant.
 * Wind SVG icon 36×36 at (65,3).
 * Labels: "Volume" (7px light) + "25%" (12px light) centred at ring centre.
 *         "Duration" (7px light) + "32%" (12px light) below.
 * Animation: arcs fill from 0→target over 1200ms, labels track live value.
 */

/* ---- animation helpers ---- */

typedef struct {
    lv_obj_t * arc;
    lv_obj_t * lbl;
} _diag_arc_ctx_t;

static void _arc_anim_cb(void * var, int32_t v)
{
    _diag_arc_ctx_t * ctx = (_diag_arc_ctx_t *)var;
    lv_arc_set_value(ctx->arc, (int32_t)v);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)v);
    lv_label_set_text(ctx->lbl, buf);
}

static void _arc_anim_ready_cb(lv_anim_t * a)
{
    lv_free(a->var);
}

static void _start_arc_anim(lv_obj_t * arc, lv_obj_t * lbl, int32_t target_pct)
{
    _diag_arc_ctx_t * ctx = (_diag_arc_ctx_t *)lv_malloc(sizeof(_diag_arc_ctx_t));
    ctx->arc = arc;
    ctx->lbl = lbl;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _arc_anim_cb);
    lv_anim_set_var(&a, ctx);
    lv_anim_set_values(&a, 0, target_pct);
    lv_anim_set_duration(&a, 1200);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_set_ready_cb(&a, _arc_anim_ready_cb);
    lv_anim_start(&a);
}

/* ---- screen ---- */

void screen_diagnostic_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* --- Outer arc ring: 106×106 at (10,12), Volume --- */
    lv_obj_t * arc_outer = lv_arc_create(scr);
    lv_obj_set_size(arc_outer, 106, 106);
    lv_obj_set_pos(arc_outer, 10, 12);
    lv_obj_set_style_pad_all(arc_outer, 0, LV_PART_MAIN);
    lv_arc_set_bg_angles(arc_outer, 0, 360);
    lv_arc_set_rotation(arc_outer, 270);           /* start fill from top */
    lv_arc_set_range(arc_outer, 0, 100);
    lv_arc_set_value(arc_outer, 0);
    /* Track: dark full ring */
    lv_obj_set_style_arc_color(arc_outer, lv_color_hex(0x272727), LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc_outer, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc_outer, LV_OPA_COVER, LV_PART_MAIN);
    /* Indicator: green fill */
    lv_obj_set_style_arc_color(arc_outer, lv_color_hex(0x648C4E), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc_outer, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc_outer, LV_OPA_COVER, LV_PART_INDICATOR);
    /* No knob, not interactive */
    lv_obj_set_style_bg_opa(arc_outer, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_remove_flag(arc_outer, LV_OBJ_FLAG_CLICKABLE);

    /* --- Inner arc ring: 83×83 at (22,23), Duration --- */
    lv_obj_t * arc_inner = lv_arc_create(scr);
    lv_obj_set_size(arc_inner, 83, 83);
    lv_obj_set_pos(arc_inner, 22, 23);
    lv_obj_set_style_pad_all(arc_inner, 0, LV_PART_MAIN);
    lv_arc_set_bg_angles(arc_inner, 0, 360);
    lv_arc_set_rotation(arc_inner, 270);
    lv_arc_set_range(arc_inner, 0, 100);
    lv_arc_set_value(arc_inner, 0);
    lv_obj_set_style_arc_color(arc_inner, lv_color_hex(0x272727), LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc_inner, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc_inner, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc_inner, lv_color_hex(0x648C4E), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc_inner, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc_inner, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(arc_inner, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_remove_flag(arc_inner, LV_OBJ_FLAG_CLICKABLE);

    /* --- Black mask: clips top-right quadrant of the rings (62×64 at 66,0) --- */
    lv_obj_t * mask = lv_obj_create(scr);
    lv_obj_set_size(mask, 62, 64);
    lv_obj_set_pos(mask, 66, 0);
    lv_obj_set_style_bg_color(mask, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mask, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(mask, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(mask, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(mask, 0, LV_PART_MAIN);

    /* --- Wind icon: 36×36 at (65,3) --- */
    lv_obj_t * img = lv_image_create(scr);
    lv_obj_set_size(img, 36, 36);
    lv_obj_set_pos(img, 65, 3);
    lv_image_set_src(img, &wind_svg_dsc);
    lv_image_set_align(img, LV_IMAGE_ALIGN_CONTAIN);

    /* --- "Volume" label: 27×9 at (50,39) --- */
    lv_obj_t * lbl_vol_title = lv_label_create(scr);
    lv_label_set_text(lbl_vol_title, "Volume");
    lv_obj_set_size(lbl_vol_title, 27, 9);
    lv_obj_set_pos(lbl_vol_title, 50, 39);
    lv_obj_set_style_text_color(lbl_vol_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_vol_title, &lv_font_montserrat_light_7, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_vol_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    /* --- "25%" value label: 24×15 at (51,46) — updated by animation --- */
    lv_obj_t * lbl_vol_val = lv_label_create(scr);
    lv_label_set_text(lbl_vol_val, "0%");
    lv_obj_set_size(lbl_vol_val, 24, 15);
    lv_obj_set_pos(lbl_vol_val, 51, 46);
    lv_obj_set_style_text_color(lbl_vol_val, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_vol_val, &lv_font_montserrat_light_12, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_vol_val, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    /* --- "Duration" label: 31×9 at (47,63) --- */
    lv_obj_t * lbl_dur_title = lv_label_create(scr);
    lv_label_set_text(lbl_dur_title, "Duration");
    lv_obj_set_size(lbl_dur_title, 31, 9);
    lv_obj_set_pos(lbl_dur_title, 47, 63);
    lv_obj_set_style_text_color(lbl_dur_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_dur_title, &lv_font_montserrat_light_7, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_dur_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    /* --- "32%" value label: 24×15 at (51,71) — updated by animation --- */
    lv_obj_t * lbl_dur_val = lv_label_create(scr);
    lv_label_set_text(lbl_dur_val, "0%");
    lv_obj_set_size(lbl_dur_val, 24, 15);
    lv_obj_set_pos(lbl_dur_val, 51, 71);
    lv_obj_set_style_text_color(lbl_dur_val, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_dur_val, &lv_font_montserrat_light_12, LV_PART_MAIN);
    lv_obj_set_style_text_align(lbl_dur_val, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    /* --- Start fill animations --- */
    _start_arc_anim(arc_outer, lbl_vol_val, CFG_DIAGNOSTIC_VOLUME_PCT);
    _start_arc_anim(arc_inner, lbl_dur_val, CFG_DIAGNOSTIC_DURATION_PCT);

    if(old_scr == NULL) {
        lv_scr_load(scr);
    } else {
        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
    }
}
