#include "weather_layer.h"

void weather_init(WeatherLayer *weather_layer);
void weather_deinit();

void weather_refresh();
void weather_set_temp(int temperature);
