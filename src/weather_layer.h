typedef Layer WeatherLayer;

WeatherLayer *weather_layer_create(GRect frame);
void weather_layer_destroy(WeatherLayer *weather_layer);

void weather_layer_set_temp(WeatherLayer *weather_layer, int temp);
void weather_layer_set_icon(WeatherLayer *weather_layer, uint32_t icon);
