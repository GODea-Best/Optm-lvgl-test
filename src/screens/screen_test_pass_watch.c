#include "lvgl/lvgl.h"
#include "screen_test_pass_watch.h"
#include "device_config.h"
#include "fonts_bold.h"
#include "bottom_bar.h"

/*
 * Watch-style Test Pass screen
 *
 * On load:
 *   1. "TEST PASSED" title fades in quickly at the top
 *   2. Green completion ring sweeps clockwise from 12 o'clock (700ms, ease-out)
 *      with a soft glow layer behind it for depth
 *   3. Centre content (BrAC label + value + PASS) fades in as the ring finishes
 */

/* Ring geometry ----------------------------------------------------------- */
#define RING_CX      64      /* centre x, absolute px */
#define RING_CY      57      /* centre y — shifted down slightly from mid     */
#define RING_R       43      /* outer radius of main ring                     */
#define RING_W        7      /* stroke width of main ring                     */
#define HALO_R       46      /* outer radius of halo ring (behind main)       */
#define HALO_W        5      /* stroke width of halo ring                     */

/* 0° = 3 o'clock in LVGL; 270° = 12 o'clock (top).
   Sweep clockwise: start=270, end animates to 270+360=630.                  */
#define ARC_START     270
#define ARC_END       630

/* Timing ------------------------------------------------------------------ */
#define ARC_ANIM_MS      700
#define TITLE_DELAY_MS   100
#define TITLE_ANIM_MS    250
#define TEXT_DELAY_MS    520
#define TEXT_ANIM_MS     300

/* -------------------------------------------------------------------------- */

typedef struct {
    lv_obj_t * obj;
    int32_t    end_angle;
} _arc_ctx_t;

/* Draw the track + animated fill ring ------------------------------------- */
static void _arc_draw_cb(lv_event_t * e)
{
    _arc_ctx_t * ctx   = (_arc_ctx_t *)lv_event_get_user_data(e);
    lv_layer_t * layer = lv_event_get_layer(e);

    lv_draw_arc_dsc_t dsc;
    lv_draw_arc_dsc_init(&dsc);
    dsc.center.x = RING_CX;
    dsc.center.y = RING_CY;
    dsc.radius   = RING_R;
    dsc.rounded  = 0;

    /* Track: full circle, dark green — 0x1A4020 survives RGB565: #184020 */
    dsc.width       = RING_W;
    dsc.color       = lv_color_hex(0x1A4020);
    dsc.opa         = LV_OPA_COVER;
    dsc.start_angle = 0;
    dsc.end_angle   = 360;
    lv_draw_arc(layer, &dsc);

    /* Only draw the fill once the animation has ticked past start */
    if(ctx->end_angle <= ARC_START) return;

    /* Halo ring: wider, slightly larger radius, solid mid-green.
     * Uses colour contrast (not alpha blending) so it is fully visible on
     * 16-bit RGB565. 0x1E7828 → RGB565 → #187828 — clearly a mid green. */
    dsc.width       = HALO_W;
    dsc.radius      = HALO_R;
    dsc.color       = lv_color_hex(0x1E7828);
    dsc.opa         = LV_OPA_COVER;
    dsc.start_angle = ARC_START;
    dsc.end_angle   = ctx->end_angle;
    dsc.rounded     = 1;
    lv_draw_arc(layer, &dsc);

    /* Main fill arc on top, standard radius */
    dsc.width       = RING_W;
    dsc.radius      = RING_R;
    dsc.color       = lv_color_hex(0x28C43A);
    dsc.opa         = LV_OPA_COVER;
    lv_draw_arc(layer, &dsc);
}

static void _arc_anim_exec(void * var, int32_t v)
{
    _arc_ctx_t * ctx = (_arc_ctx_t *)var;
    ctx->end_angle = v;
    lv_obj_invalidate(ctx->obj);
}

static void _arc_delete_cb(lv_event_t * e)
{
    _arc_ctx_t * ctx = (_arc_ctx_t *)lv_event_get_user_data(e);
    lv_anim_delete(ctx, NULL);
    lv_free(ctx);
}

static void _opa_anim_exec(void * obj, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)v, 0);
}

/* -------------------------------------------------------------------------- */

void screen_test_pass_watch_create(lv_obj_t * old_scr)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, 128, 128);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(scr, 15, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(scr, true, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);

    /* --- Ring overlay ---------------------------------------------------- */
    _arc_ctx_t * ctx = (_arc_ctx_t *)lv_malloc(sizeof(_arc_ctx_t));
    lv_obj_t * arc_obj = lv_obj_create(scr);
    lv_obj_set_size(arc_obj, 128, 128);
    lv_obj_set_pos(arc_obj, 0, 0);
    lv_obj_set_style_bg_opa(arc_obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(arc_obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(arc_obj, 0, LV_PART_MAIN);
    lv_obj_remove_flag(arc_obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(arc_obj, LV_OBJ_FLAG_CLICKABLE);

    ctx->obj       = arc_obj;
    ctx->end_angle = ARC_START;
    lv_obj_add_event_cb(arc_obj, _arc_draw_cb,   LV_EVENT_DRAW_MAIN, ctx);
    lv_obj_add_event_cb(arc_obj, _arc_delete_cb, LV_EVENT_DELETE,    ctx);

    /* --- Title: "TEST PASSED" — top, fades in first ---------------------- */
    lv_obj_t * lbl_title = lv_label_create(scr);
    lv_label_set_text(lbl_title, "TEST PASSED");
    lv_obj_align(lbl_title, LV_ALIGN_TOP_MID, 0, 4);
    lv_obj_set_style_text_color(lbl_title, lv_color_hex(0x888888), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_title, &lv_font_montserrat_regular_10, LV_PART_MAIN);
    lv_obj_set_style_opa(lbl_title, LV_OPA_TRANSP, LV_PART_MAIN);

    /* --- Centre content: BrAC / value / PASS — fades in as ring ends ----- */
    /* 70×46 container centred at (RING_CX, RING_CY) */
    lv_obj_t * content = lv_obj_create(scr);
    lv_obj_set_size(content, 70, 46);
    lv_obj_set_pos(content, RING_CX - 35, RING_CY - 23);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(content, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(content, 0, LV_PART_MAIN);
    lv_obj_set_style_opa(content, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_remove_flag(content, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(content, LV_OBJ_FLAG_CLICKABLE);

    /* "BrAC" — small, muted, top of content block */
    lv_obj_t * lbl_brac = lv_label_create(content);
    lv_label_set_text(lbl_brac, "BrAC");
    lv_obj_align(lbl_brac, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_color(lbl_brac, lv_color_hex(0x888888), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_brac, &lv_font_montserrat_regular_10, LV_PART_MAIN);

    /* Result value — large, white, centre of ring */
    lv_obj_t * lbl_val = lv_label_create(content);
    lv_label_set_text(lbl_val, CFG_BRAC_VALUE);
    lv_obj_align(lbl_val, LV_ALIGN_TOP_MID, 0, 12);
    lv_obj_set_style_text_color(lbl_val, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_val, &lv_font_montserrat_extrabold_20, LV_PART_MAIN);

    /* "PASS" — green accent, below value */
    lv_obj_t * lbl_pass = lv_label_create(content);
    lv_label_set_text(lbl_pass, "PASS");
    lv_obj_align(lbl_pass, LV_ALIGN_TOP_MID, 0, 34);
    lv_obj_set_style_text_color(lbl_pass, lv_color_hex(0x28C43A), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl_pass, &lv_font_montserrat_semibold_12, LV_PART_MAIN);

    /* --- Bottom bar ------------------------------------------------------ */
    BOTTOM_BAR_CREATE(scr, lv_color_hex(0x1D9101));

    /* --- Animations ------------------------------------------------------- */

    lv_anim_t a;

    /* 1. Arc sweep: 12 o'clock → full circle */
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _arc_anim_exec);
    lv_anim_set_var(&a, ctx);
    lv_anim_set_values(&a, ARC_START, ARC_END);
    lv_anim_set_duration(&a, ARC_ANIM_MS);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);

    /* 2. Title fade-in — begins almost immediately */
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _opa_anim_exec);
    lv_anim_set_var(&a, lbl_title);
    lv_anim_set_values(&a, 0, 255);
    lv_anim_set_duration(&a, TITLE_ANIM_MS);
    lv_anim_set_delay(&a, TITLE_DELAY_MS);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);

    /* 3. Centre content fade-in — timed to arc completion */
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, _opa_anim_exec);
    lv_anim_set_var(&a, content);
    lv_anim_set_values(&a, 0, 255);
    lv_anim_set_duration(&a, TEXT_ANIM_MS);
    lv_anim_set_delay(&a, TEXT_DELAY_MS);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);

    if(old_scr == NULL) lv_scr_load(scr);
    else lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, CFG_SLIDE_MS, 0, true);
}
