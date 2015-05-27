#include <pebble.h>

#include "weather_layer.h"
#include "weather.h"
#include "network.h"
#include "keys.h"
#include "storage.h"

/* Update weather every 10 minutes */
#define WEATHER_UPDATE_INTERVAL 10 * 60

weather_info_t current_weather;
time_t weather_last_updated = 0;

static void load_data(){
  persist_read_data(STORAGE_CURRENT_WEATHER, &current_weather, sizeof(current_weather));
  persist_read_data(STORAGE_LAST_UPDATE, &weather_last_updated, sizeof(weather_last_updated));
}

static void save_data(){
  persist_write_data(STORAGE_CURRENT_WEATHER, &current_weather, sizeof(current_weather));
  persist_write_data(STORAGE_LAST_UPDATE, &weather_last_updated, sizeof(weather_last_updated));
}

void weather_init(){
  if (persist_exists(STORAGE_CURRENT_WEATHER)) {
    load_data();
  }
  weather_layer_draw();
}

bool weather_available(){
  return !!weather_last_updated;
}

int weather_get_temperature(){
  return current_weather.temperature;
}

int weather_get_icon(){
  return current_weather.icon;
}

void weather_update(weather_info_t *w){
  current_weather.temperature = w->temperature;
  current_weather.icon = w->icon;

  weather_last_updated = time(NULL);

  save_data();
  weather_layer_draw();
}

void weather_refresh(){
  time_t now = time(NULL);

  if(now - weather_last_updated >= WEATHER_UPDATE_INTERVAL){
    network_request_update();
  }
}

void weather_deinit(){
}
