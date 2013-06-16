
/*
 * @name Lively
 * @type Watchface
 * @author Edward Hotchkiss <edwardhotchkiss@me.com>
 * @description Utilitarian Pebble Watchface
 * @license MIT
 */

#include "lively.h"
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "resource_ids.auto.h"

#define MY_UUID { 0xA4, 0x1B, 0xB0, 0xE2, 0xD2, 0x62, 0x4E, 0xA6, 0xAA, 0x30, 0xED, 0xBE, 0x01, 0xE3, 0x8A, 0x02 }
PBL_APP_INFO(MY_UUID, "Lively", "Edward Hotchkiss", 1, 0, RESOURCE_ID_IMAGE_MENU_ICON_WHITE, APP_INFO_WATCH_FACE);

// called on single click
void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

}

// called for multi-clicks
void select_multi_click_handler(ClickRecognizerRef recognizer, Window *window) {

}

// called on long click start
void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {

}

// called when long click is released
void select_long_click_release_handler(ClickRecognizerRef recognizer, Window *window) {

}

// handles setting time updates
void update_time(AppContextRef ctx, PblTm *current_time) {
  char *time_format;
  // set date
  string_format_time(date_text, sizeof(date_text), "%b %d", current_time);
  text_layer_set_text(&date_text_layer, date_text);
  // check military time
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }
  // set time
  string_format_time(time_text, sizeof(time_text), time_format, current_time);
  text_layer_set_text(&time_text_layer, time_text);
}

// handle clock ticks (by the minute)
void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
  update_time(ctx, t->tick_time);
}

// button click/hold bindings
void config_provider(ClickConfig **config, Window *window) {
  // single click / repeat-on-hold
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
  config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 1000;
  // multi click config
  config[BUTTON_ID_SELECT]->multi_click.handler = (ClickHandler) select_multi_click_handler;
  config[BUTTON_ID_SELECT]->multi_click.min = 2;
  config[BUTTON_ID_SELECT]->multi_click.max = 10;
  config[BUTTON_ID_UP]->multi_click.last_click_only = true;
  // long click
  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;
  config[BUTTON_ID_SELECT]->long_click.release_handler = (ClickHandler) select_long_click_release_handler;
  config[BUTTON_ID_SELECT]->long_click.delay_ms = 700;
}

// init app and setup
void handle_init(AppContextRef ctx) {
  (void)ctx;

  // init main window and app wide resources
  window_init(&main_window, "Lively");
  window_stack_push(&main_window, true);
  window_set_background_color(&main_window, GColorClear);
  resource_init_current_app(&APP_RESOURCES);

  // custom fonts
  avenir_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_AVENIR_NEXT_COND_42));
  silkscreen_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SILKSCREEN_8));

  // background image
  layer_init(&background_layer, GRect(0, 0, SCREEN_WIDWTH, SCREEN_HEIGHT));
  bmp_init_container(RESOURCE_ID_EDWARD_AND_KATE, &background_image);
  layer_add_child(&main_window.layer, &background_image.layer.layer);
  layer_set_frame(&background_image.layer.layer, GRect(0, 0, SCREEN_WIDWTH, SCREEN_HEIGHT));

  // date text
  text_layer_init(&date_text_layer, GRect(99, 100, 45, 8));
  text_layer_set_font(&date_text_layer, silkscreen_font);
  text_layer_set_text_alignment(&date_text_layer, GTextAlignmentRight);
  text_layer_set_text_color(&date_text_layer, GColorWhite);
  text_layer_set_background_color(&date_text_layer, /*GColorClear*/ GColorBlack);
  layer_add_child(&main_window.layer, &date_text_layer.layer); 

  // init time text
  text_layer_init(&time_text_layer, GRect(22, 108, 120, 42));
  text_layer_set_font(&time_text_layer, avenir_font);
  text_layer_set_text_alignment(&time_text_layer, GTextAlignmentRight);
  text_layer_set_text_color(&time_text_layer, GColorWhite);
  text_layer_set_background_color(&time_text_layer, GColorBlack);
  layer_add_child(&main_window.layer, &time_text_layer.layer);

  // update initial time
  PblTm current_time;
  get_time(&current_time);
  update_time(ctx, &current_time);

  // button bindings
  window_set_click_config_provider(&main_window, (ClickConfigProvider) config_provider);
}

// dealloccation when watchface is closed
void handle_deinit(AppContextRef ctx) {
  (void)ctx;
  bmp_deinit_container(&background_image);
  fonts_unload_custom_font(avenir_font);
  fonts_unload_custom_font(silkscreen_font);
}

// main pebble watchface loop
void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}

/* EOF */