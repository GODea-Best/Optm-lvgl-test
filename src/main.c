/**
 * @file main.c
 *
 * Three SDL windows arranged side by side:
 *   - Main window  : 128x128  (simulated embedded display, actual size)
 *   - Zoom window  : 128x128 logical → ZOOM_FACTOR× physical (magnified view)
 *   - Control panel: 660x980  (buttons to select which screen to show)
 */

/* Zoom multiplier for the third window (e.g. 4.0 → 512×512 physical) */
#define ZOOM_FACTOR 4.0f

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif

#include "lvgl/lvgl.h"
#include <SDL.h>
#include "hal/hal.h"
#include "screen_initial_test.h"
#include "control_panel.h"
#include "carousel_anim.h"

/* Hit-test callback: mark the entire main display window as draggable */
static SDL_HitTestResult main_win_hit_test(SDL_Window * win, const SDL_Point * area, void * data)
{
  (void)win; (void)area; (void)data;
  return SDL_HITTEST_DRAGGABLE;
}

#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  lv_init();

  /* --- Window 1: Main 128x128 display (actual embedded display size) --- */
  sdl_hal_init(128, 128);
  lv_display_t * disp_main = lv_display_get_default();
  SDL_SetWindowHitTest(lv_sdl_window_get_window(disp_main), main_win_hit_test, NULL);
  SDL_SetWindowTitle(lv_sdl_window_get_window(disp_main), "Display (1x)");

  /* --- Window 2: Zoom display — same 128x128 logical, scaled up --- */
  lv_display_t * disp_zoom = lv_sdl_window_create(128, 128);
  lv_sdl_window_set_zoom(disp_zoom, ZOOM_FACTOR);
  SDL_SetWindowHitTest(lv_sdl_window_get_window(disp_zoom), main_win_hit_test, NULL);
  SDL_SetWindowTitle(lv_sdl_window_get_window(disp_zoom), "Display (4x)");

  /* --- Window 3: Control panel --- */
  lv_display_t * disp_ctrl = lv_sdl_window_create(660, 980);
  SDL_SetWindowTitle(lv_sdl_window_get_window(disp_ctrl), "Screen Selector");

  /* --- Position windows side by side --- */
  int zoom_px = (int)(128 * ZOOM_FACTOR);
  SDL_SetWindowPosition(lv_sdl_window_get_window(disp_main), 20,              20);
  SDL_SetWindowPosition(lv_sdl_window_get_window(disp_zoom), 20 + 128 + 20,   20);
  SDL_SetWindowPosition(lv_sdl_window_get_window(disp_ctrl), 20 + 128 + 20 + zoom_px + 20, 20);

  /* --- Wire mouse input --- */
  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_display(mouse, disp_ctrl);
  lv_indev_t * mouse_zoom = lv_sdl_mouse_create();
  lv_indev_set_display(mouse_zoom, disp_zoom);

  /* --- Black bottom layer for all displays --- */
  lv_obj_set_style_bg_color(lv_display_get_layer_bottom(disp_main), lv_color_black(), 0);
  lv_obj_set_style_bg_opa(lv_display_get_layer_bottom(disp_main),  LV_OPA_COVER,    0);
  lv_obj_set_style_bg_color(lv_display_get_layer_bottom(disp_zoom), lv_color_black(), 0);
  lv_obj_set_style_bg_opa(lv_display_get_layer_bottom(disp_zoom),  LV_OPA_COVER,    0);
  lv_obj_set_style_bg_color(lv_display_get_layer_bottom(disp_ctrl), lv_color_black(), 0);
  lv_obj_set_style_bg_opa(lv_display_get_layer_bottom(disp_ctrl),  LV_OPA_COVER,    0);

  /* --- Load initial screen on both main and zoom --- */
  lv_display_set_default(disp_main);
  screen_initial_test_create(NULL);
  lv_display_set_default(disp_zoom);
  screen_initial_test_create(NULL);

  /* --- Build control panel --- */
  control_panel_create(disp_main, disp_ctrl, disp_zoom);

  /* --- Main loop --- */
  while(1) {
    uint32_t sleep_time_ms = lv_timer_handler();
    if(sleep_time_ms == LV_NO_TIMER_READY){
      sleep_time_ms = LV_DEF_REFR_PERIOD;
    }
    if(sleep_time_ms > LV_DEF_REFR_PERIOD) sleep_time_ms = LV_DEF_REFR_PERIOD;
#ifdef _MSC_VER
    Sleep(sleep_time_ms);
#else
    usleep(sleep_time_ms * 1000);
#endif
  }

  return 0;
}

#endif
