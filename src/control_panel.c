#include "lvgl/lvgl.h"
#include "control_panel.h"
#include "carousel_anim.h"
#include "screen_initial_test.h"
#include "screen_service_alert.h"
#include "screen_service_warning.h"
#include "screen_violation_warning.h"
#include "screen_test_error.h"
#include "screen_invalid_sample.h"
#include "screen_initial_test_pass.h"
#include "screen_free_restart.h"
#include "screen_diagnostic.h"
#include "screen_logo.h"
#include "screen_retest_alert.h"
#include "screen_blow_prompt.h"
#include "screen_blow_prompt_anim.h"
#include "screen_violation_lockout.h"
#include "screen_lockout.h"
#include "screen_warning_msg.h"
#include "screen_sample_analysis.h"
#include "screen_usa_logo.h"
#include "screen_usa_logo_3d.h"
#include "screen_initial_test_icons.h"
#include "screen_initial_test_pass_icons.h"
#include "screen_test_error_icons.h"
#include "screen_invalid_sample_icons.h"
#include "screen_free_restart_icons.h"
#include "screen_service_alert_icons.h"
#include "screen_service_warning_icons.h"
#include "screen_violation_warning_icons.h"
#include "screen_violation_lockout_icons.h"
#include "screen_retest_alert_icons.h"
#include "screen_lockout_icons.h"
#include "screen_initial_card.h"
#include "screen_initial_card_bold.h"
#include "screen_optmg4_logo.h"
#include "screen_tamper_warn.h"
#include "screen_service_alert_bold.h"
#include "screen_service_warning_bold.h"
#include "screen_violation_warning_bold.h"
#include "screen_test_error_bold.h"
#include "screen_invalid_sample_bold.h"
#include "screen_test_pass_bold.h"
#include "screen_free_restart_bold.h"
#include "screen_retest_alert_bold.h"
#include "screen_lockout_bold.h"
#include "screen_violation_lockout_bold.h"
#include "screen_test_pass_watch.h"

static lv_display_t * s_disp_main = NULL;
static lv_display_t * s_disp_ctrl = NULL;
static lv_display_t * s_disp_zoom = NULL;

static void btn_cb_initial_test(lv_event_t * e)     { carousel_transition(screen_initial_test_create,      s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_service_alert(lv_event_t * e)    { carousel_transition(screen_service_alert_create,     s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_service_warning(lv_event_t * e)  { carousel_transition(screen_service_warning_create,   s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_violation(lv_event_t * e)        { carousel_transition(screen_violation_warning_create, s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_test_error(lv_event_t * e)       { carousel_transition(screen_test_error_create,        s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_invalid_sample(lv_event_t * e)   { carousel_transition(screen_invalid_sample_create,    s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_test_pass(lv_event_t * e)        { carousel_transition(screen_initial_test_pass_create, s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_free_restart(lv_event_t * e)     { carousel_transition(screen_free_restart_create,      s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_diagnostic(lv_event_t * e)       { carousel_transition(screen_diagnostic_create,        s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_logo(lv_event_t * e)             { carousel_transition(screen_logo_create,               s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_retest(lv_event_t * e)            { carousel_transition(screen_retest_alert_create,        s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_blow_prompt(lv_event_t * e)       { carousel_transition(screen_blow_prompt_create,         s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_blow_prompt_anim(lv_event_t * e)  { carousel_transition(screen_blow_prompt_anim_create,    s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_violation_lockout(lv_event_t * e) { carousel_transition(screen_violation_lockout_create,   s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_lockout(lv_event_t * e)           { carousel_transition(screen_lockout_create,              s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_warning_msg(lv_event_t * e)       { carousel_transition(screen_warning_msg_create,          s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_sample_analysis(lv_event_t * e)   { carousel_transition(screen_sample_analysis_create,      s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_usa_logo(lv_event_t * e)          { carousel_transition(screen_usa_logo_create,             s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_usa_logo_3d(lv_event_t * e)            { carousel_transition(screen_usa_logo_3d_create,              s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_initial_test_icons(lv_event_t * e)     { carousel_transition(screen_initial_test_icons_create,       s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_test_pass_icons(lv_event_t * e)        { carousel_transition(screen_initial_test_pass_icons_create,  s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_test_error_icons(lv_event_t * e)       { carousel_transition(screen_test_error_icons_create,         s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_invalid_sample_icons(lv_event_t * e)   { carousel_transition(screen_invalid_sample_icons_create,     s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_free_restart_icons(lv_event_t * e)     { carousel_transition(screen_free_restart_icons_create,       s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_service_alert_icons(lv_event_t * e)    { carousel_transition(screen_service_alert_icons_create,      s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_service_warning_icons(lv_event_t * e)  { carousel_transition(screen_service_warning_icons_create,    s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_violation_warn_icons(lv_event_t * e)   { carousel_transition(screen_violation_warning_icons_create,  s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_violation_lock_icons(lv_event_t * e)   { carousel_transition(screen_violation_lockout_icons_create,  s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_retest_icons(lv_event_t * e)           { carousel_transition(screen_retest_alert_icons_create,       s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_lockout_icons(lv_event_t * e)          { carousel_transition(screen_lockout_icons_create,            s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_optmg4_logo(lv_event_t * e)                 { carousel_transition(screen_optmg4_logo_create,               s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_tamper_warn(lv_event_t * e)                 { carousel_transition(screen_tamper_warn_create,                s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_initial_card(lv_event_t * e)                { carousel_transition(screen_initial_card_create,              s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_initial_card_bold(lv_event_t * e)           { carousel_transition(screen_initial_card_bold_create,         s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_service_alert_bold(lv_event_t * e)          { carousel_transition(screen_service_alert_bold_create,        s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_service_warning_bold(lv_event_t * e)        { carousel_transition(screen_service_warning_bold_create,      s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_violation_warning_bold(lv_event_t * e)      { carousel_transition(screen_violation_warning_bold_create,    s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_test_error_bold(lv_event_t * e)             { carousel_transition(screen_test_error_bold_create,           s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_invalid_sample_bold(lv_event_t * e)         { carousel_transition(screen_invalid_sample_bold_create,       s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_test_pass_bold(lv_event_t * e)              { carousel_transition(screen_test_pass_bold_create,            s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_free_restart_bold(lv_event_t * e)           { carousel_transition(screen_free_restart_bold_create,         s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_retest_alert_bold(lv_event_t * e)           { carousel_transition(screen_retest_alert_bold_create,         s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_lockout_bold(lv_event_t * e)                { carousel_transition(screen_lockout_bold_create,              s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_violation_lockout_bold(lv_event_t * e)      { carousel_transition(screen_violation_lockout_bold_create,    s_disp_main, s_disp_ctrl, s_disp_zoom); }
static void btn_cb_test_pass_watch(lv_event_t * e)             { carousel_transition(screen_test_pass_watch_create,           s_disp_main, s_disp_ctrl, s_disp_zoom); }

static void make_button(lv_obj_t * parent,
                         const char * label,
                         lv_color_t color,
                         lv_event_cb_t cb)
{
    lv_obj_t * btn = lv_button_create(parent);
    lv_obj_set_size(btn, 200, 34);

    /* Top-to-bottom gradient: lighter top → darker bottom (raised 3D effect) */
    lv_color_t color_top  = lv_color_mix(lv_color_hex(0xFFFFFF), color, 40);  /* ~16% white */
    lv_color_t color_bot  = lv_color_mix(lv_color_hex(0x000000), color, 80);  /* ~31% black */
    lv_obj_set_style_bg_color(btn, color_top, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(btn, color_bot, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_VER, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);

    /* Top-left bevel highlight — simulates light hitting the raised edge */
    lv_color_t color_hi = lv_color_mix(lv_color_hex(0xFFFFFF), color, 70);
    lv_obj_set_style_border_color(btn, color_hi, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 1, LV_PART_MAIN);
    lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT, LV_PART_MAIN);
    lv_obj_set_style_border_opa(btn, LV_OPA_50, LV_PART_MAIN);

    /* Drop shadow — button appears raised off the surface */
    lv_obj_set_style_shadow_width(btn, 6, LV_PART_MAIN);
    lv_obj_set_style_shadow_ofs_x(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_ofs_y(btn, 3, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(btn, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(btn, LV_OPA_60, LV_PART_MAIN);

    lv_obj_set_style_radius(btn, 6, LV_PART_MAIN);
    lv_obj_set_style_pad_all(btn, 4, LV_PART_MAIN);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lbl = lv_label_create(btn);
    lv_label_set_text(lbl, label);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);
}

/* Full-width section header label — forces its own row in ROW_WRAP flex */
static void make_header(lv_obj_t * parent, const char * label)
{
    lv_obj_t * hdr = lv_label_create(parent);
    lv_label_set_text(hdr, label);
    lv_obj_set_size(hdr, lv_pct(100), 20);
    lv_obj_set_style_text_color(hdr, lv_color_hex(0xAAAAAA), LV_PART_MAIN);
    lv_obj_set_style_text_font(hdr, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_pad_top(hdr, 6, LV_PART_MAIN);
    lv_obj_set_style_text_align(hdr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
}

void control_panel_create(lv_display_t * disp_main, lv_display_t * disp_ctrl, lv_display_t * disp_zoom)
{
    s_disp_main = disp_main;
    s_disp_ctrl = disp_ctrl;
    s_disp_zoom = disp_zoom;

    lv_display_set_default(disp_ctrl);

    lv_obj_t * scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1E1E1E), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);

    lv_obj_t * title = lv_label_create(scr);
    lv_label_set_text(title, "Screen Selector");
    lv_obj_set_pos(title, 10, 8);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, LV_PART_MAIN);

    lv_obj_t * list = lv_obj_create(scr);
    lv_obj_set_size(list, 660, 940);
    lv_obj_set_pos(list, 0, 34);
    lv_obj_set_style_bg_color(list, lv_color_hex(0x1E1E1E), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(list, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(list, 8, LV_PART_MAIN);
    lv_obj_set_style_pad_row(list, 6, LV_PART_MAIN);
    lv_obj_set_style_pad_column(list, 8, LV_PART_MAIN);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_ROW_WRAP);

    make_button(list, "1. Initial Test",      lv_color_hex(0x333333), btn_cb_initial_test);
    make_button(list, "2. Service Alert",     lv_color_hex(0x005566), btn_cb_service_alert);
    make_button(list, "3. Service Warning",   lv_color_hex(0x007D99), btn_cb_service_warning);
    make_button(list, "4. Violation Warning", lv_color_hex(0x8B0000), btn_cb_violation);
    make_button(list, "5. Test Error",        lv_color_hex(0x4B0082), btn_cb_test_error);
    make_button(list, "6. Invalid Sample",    lv_color_hex(0x12246E), btn_cb_invalid_sample);
    make_button(list, "7. Test Pass",         lv_color_hex(0x1D6B00), btn_cb_test_pass);
    make_button(list, "8. Free Restart",      lv_color_hex(0x1A4D00), btn_cb_free_restart);
    make_button(list, "9. Diagnostic",        lv_color_hex(0x3D5C2A), btn_cb_diagnostic);
    make_button(list, "10. Logo",             lv_color_hex(0x1A1A1A), btn_cb_logo);
    make_button(list, "11. Retest Alert",      lv_color_hex(0xCC3300), btn_cb_retest);
    make_button(list, "12. Blow Prompt",       lv_color_hex(0x0A0070), btn_cb_blow_prompt);
    make_button(list, "12b. Blow Anim",        lv_color_hex(0x1A00AA), btn_cb_blow_prompt_anim);
    make_button(list, "13. Violation Lockout", lv_color_hex(0x5A0000), btn_cb_violation_lockout);
    make_button(list, "14. Lockout",           lv_color_hex(0x7A0000), btn_cb_lockout);
    make_button(list, "15. Warning Msg",       lv_color_hex(0xCC3300), btn_cb_warning_msg);
    make_button(list, "16. Sample Analysis",   lv_color_hex(0x0A0090), btn_cb_sample_analysis);
    make_button(list, "17. USA Logo",          lv_color_hex(0x1A1A1A), btn_cb_usa_logo);
    make_button(list, "18. USA Logo 3D",         lv_color_hex(0x2A2A2A), btn_cb_usa_logo_3d);
    make_button(list, "19. Tamper Warn",         lv_color_hex(0x8B5500), btn_cb_tamper_warn);
    make_header(list, "--- Icon Versions ---");
    make_button(list, "1b. Initial Test",        lv_color_hex(0x333333), btn_cb_initial_test_icons);
    make_button(list, "7b. Test Pass",           lv_color_hex(0x1D6B00), btn_cb_test_pass_icons);
    make_button(list, "5b. Test Error",          lv_color_hex(0x4B0082), btn_cb_test_error_icons);
    make_button(list, "6b. Invalid Sample",      lv_color_hex(0x12246E), btn_cb_invalid_sample_icons);
    make_button(list, "8b. Free Restart",        lv_color_hex(0x1A4D00), btn_cb_free_restart_icons);
    make_button(list, "2b. Service Alert",       lv_color_hex(0x005566), btn_cb_service_alert_icons);
    make_button(list, "3b. Service Warning",     lv_color_hex(0x007D99), btn_cb_service_warning_icons);
    make_button(list, "4b. Violation Warning",   lv_color_hex(0x8B0000), btn_cb_violation_warn_icons);
    make_button(list, "13b. Violation Lockout",  lv_color_hex(0x5A0000), btn_cb_violation_lock_icons);
    make_button(list, "11b. Retest Alert",       lv_color_hex(0xCC3300), btn_cb_retest_icons);
    make_button(list, "14b. Lockout",            lv_color_hex(0x7A0000), btn_cb_lockout_icons);
    make_header(list, "--- Card Style ---");
    make_button(list, "1c. Initial Card",        lv_color_hex(0x001A3A), btn_cb_initial_card);
    make_button(list, "20. OptmG4 Logo",         lv_color_hex(0x1A1A1A), btn_cb_optmg4_logo);
    make_header(list, "--- Bold Cards ---");
    make_button(list, "1d. Initial Card",         lv_color_hex(0x0A007A), btn_cb_initial_card_bold);
    make_button(list, "2d. Service Alert",        lv_color_hex(0x006688), btn_cb_service_alert_bold);
    make_button(list, "3d. Service Warning",      lv_color_hex(0x005566), btn_cb_service_warning_bold);
    make_button(list, "4d. Violation Warning",    lv_color_hex(0x7A0000), btn_cb_violation_warning_bold);
    make_button(list, "5d. Test Error",           lv_color_hex(0x6B3A00), btn_cb_test_error_bold);
    make_button(list, "6d. Invalid Sample",       lv_color_hex(0x0A1A50), btn_cb_invalid_sample_bold);
    make_button(list, "7d. Test Pass",            lv_color_hex(0x1D6B00), btn_cb_test_pass_bold);
    make_button(list, "8d. Free Restart",         lv_color_hex(0x1A4D00), btn_cb_free_restart_bold);
    make_button(list, "9d. Retest Alert",         lv_color_hex(0x8B2200), btn_cb_retest_alert_bold);
    make_button(list, "10d. Lockout",             lv_color_hex(0x7A0000), btn_cb_lockout_bold);
    make_button(list, "11d. Violation Lockout",   lv_color_hex(0x5A0000), btn_cb_violation_lockout_bold);
    make_header(list, "--- Watch Style ---");
    make_button(list, "1w. Test Pass",            lv_color_hex(0x0A2A0A), btn_cb_test_pass_watch);

    lv_display_set_default(disp_main);
}
