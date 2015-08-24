
typedef struct {
  int temperature;
  int icon;
  int duration;
} weather_info_t;

void weather_init();
void weather_deinit();

void weather_refresh();
void weather_update(weather_info_t *w);

bool weather_available();
int weather_get_temperature();
int weather_get_icon();
int weather_get_duration();
