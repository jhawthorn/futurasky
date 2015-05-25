#include <pebble.h>

#include "weather.h"
#include "network.h"
#include "keys.h"

static void *layer = NULL;

int weather_loaded = 0;

void weather_init(WeatherLayer *weather_layer){
  layer = weather_layer;

  //weather_layer_set_temp(weather_layer, 10);
  weather_layer_set_icon(layer, RESOURCE_ID_ICON_LOADING);
}

void weather_set_temp(int temperature){
  if(layer)
    weather_layer_set_temp(layer, temperature);
}

void weather_set_icon(int icon){
  if(layer)
    weather_layer_set_icon(layer, icon);
}

void weather_refresh(){
  network_request_update();
}

void weather_deinit(){
  layer = NULL;
}
