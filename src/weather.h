#include "weather_layer.h"

typedef struct {
  int temperature;
  int icon;
} weather_info_t;

void weather_init(WeatherLayer *weather_layer);
void weather_deinit();

void weather_refresh();
void weather_update(weather_info_t *w);
