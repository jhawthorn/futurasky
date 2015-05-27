typedef Layer WeatherLayer;

WeatherLayer *weather_layer_create(GRect frame);
void weather_layer_destroy();

void weather_layer_draw();
void weather_layer_set_temp(int temp);
void weather_layer_set_icon(uint32_t icon);
