#include <pebble.h>

#include "network.h"
#include "weather_layer.h"
#include "weather.h"

#define TIME_FRAME      (GRect(0, 2, 144, 168-6))
#define DATE_FRAME      (GRect(1, 66, 144, 168-62))
#define WEATHER_FRAME   (GRect(0, 90, 144, 80))

static Window *window;
static TextLayer *time_layer;
static TextLayer *date_layer;

static WeatherLayer *weather_layer;

static char time_text[] = "00:00";
static char date_text[16];

GFont font_date;
GFont font_time;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  clock_copy_time_string(time_text, sizeof(time_text));
  text_layer_set_text(time_layer, time_text);

  if (units_changed & DAY_UNIT) {
    // Update the date - Without a leading 0 on the day of the month
    char day_text[16];
    strftime(day_text, sizeof(day_text), "%B", tick_time);
    snprintf(date_text, sizeof(date_text), "%s %i", day_text, tick_time->tm_mday);
    text_layer_set_text(date_layer, date_text);
  }

  /* Refresh the weather if it is stale */
  weather_refresh();
}

static void bluetooth_handler(bool connected) {
  if(!connected){
    vibes_short_pulse();
  }
  /* Show redraw status */
  weather_draw();
}

static void initial_draw() {
  time_t now = time(NULL);
  tick_handler(localtime(&now), SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT);
  bluetooth_handler(bluetooth_connection_service_peek());
}

static void window_load(Window *window) {
  window_set_background_color(window, GColorBlack);

  font_date = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FUTURA_18));
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FUTURA_CONDENSED_53));

  /* Time */
  time_layer = text_layer_create(TIME_FRAME);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, font_time);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));


  /* Date */
  date_layer = text_layer_create(DATE_FRAME);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_font(date_layer, font_date);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

  /* Weather */
  weather_layer = weather_layer_create(WEATHER_FRAME);
  layer_add_child(window_get_root_layer(window), weather_layer);

  weather_init(weather_layer);
  network_init();

  initial_draw();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  bluetooth_connection_service_subscribe(bluetooth_handler);
}

static void window_unload(Window *window) {
  weather_deinit();
  weather_layer_destroy(weather_layer);

  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();

  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
