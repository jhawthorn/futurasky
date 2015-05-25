#include <pebble.h>

#include "weather.h"
#include "network.h"
#include "keys.h"
#include "storage.h"

/* Update weather every 10 minutes */
#define WEATHER_UPDATE_INTERVAL 10 * 60

static void *layer = NULL;

weather_info_t current_weather;
time_t weather_last_updated = 0;

static void draw(){
  weather_layer_set_temp(layer, current_weather.temperature);
  weather_layer_set_icon(layer, current_weather.icon);
}

static void load_data(){
  persist_read_data(STORAGE_CURRENT_WEATHER, &current_weather, sizeof(current_weather));
  persist_read_data(STORAGE_LAST_UPDATE, &weather_last_updated, sizeof(weather_last_updated));
}

static void save_data(){
  persist_write_data(STORAGE_CURRENT_WEATHER, &current_weather, sizeof(current_weather));
  persist_write_data(STORAGE_LAST_UPDATE, &weather_last_updated, sizeof(weather_last_updated));
}

void weather_init(WeatherLayer *weather_layer){
  layer = weather_layer;

  if (persist_exists(STORAGE_CURRENT_WEATHER)) {
    load_data();
    draw();
  } else {
    weather_layer_set_icon(layer, RESOURCE_ID_ICON_LOADING);
  }
}

void weather_update(weather_info_t *w){
  current_weather.temperature = w->temperature;
  current_weather.icon = w->icon;

  weather_last_updated = time(NULL);

  save_data();
  draw();
}

void weather_refresh(){
  time_t now = time(NULL);

  if(now - weather_last_updated >= WEATHER_UPDATE_INTERVAL){
    network_request_update();
  }
}

void weather_deinit(){
  layer = NULL;
}
