#include <pebble.h>

#include "weather_layer.h"

typedef struct {
  TextLayer *temp_layer_background;
  TextLayer *temp_layer;
  GBitmap *icon;
  BitmapLayer *icon_layer;
  char temp_str[6];
} WeatherLayerData;

static GFont large_font, small_font;

WeatherLayer *weather_layer_create(GRect frame){
  // Create a new layer with some extra space to save our custom Layer infos
  WeatherLayer *weather_layer = layer_create_with_data(frame, sizeof(WeatherLayerData));
  WeatherLayerData *wld = layer_get_data(weather_layer);

  large_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FUTURA_40));
  small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FUTURA_35));

  // Add background layer
  wld->temp_layer_background = text_layer_create(GRect(0, 10, 144, 68));
  text_layer_set_background_color(wld->temp_layer_background, GColorWhite);
  layer_add_child(weather_layer, text_layer_get_layer(wld->temp_layer_background));

  // Add temperature layer
  wld->temp_layer = text_layer_create(GRect(70, 19, 72, 80));
  text_layer_set_background_color(wld->temp_layer, GColorClear);
  text_layer_set_text_alignment(wld->temp_layer, GTextAlignmentCenter);
  text_layer_set_font(wld->temp_layer, large_font);
  layer_add_child(weather_layer, text_layer_get_layer(wld->temp_layer));

  // Add bitmap layer
  wld->icon_layer = bitmap_layer_create(GRect(9, 13, 60, 60));
  layer_add_child(weather_layer, bitmap_layer_get_layer(wld->icon_layer));

  wld->icon = NULL;

  return weather_layer;
}

void weather_layer_set_temp(WeatherLayer *weather_layer, int temp){
  WeatherLayerData *wld = layer_get_data(weather_layer);
  snprintf(wld->temp_str, sizeof(wld->temp_str), "%i°", temp);

  text_layer_set_text(wld->temp_layer, wld->temp_str);
}

void weather_layer_set_icon(WeatherLayer *weather_layer, uint32_t icon){
  WeatherLayerData *wld = layer_get_data(weather_layer);

  GBitmap *new_icon = gbitmap_create_with_resource(icon);
  // Display the new bitmap
  bitmap_layer_set_bitmap(wld->icon_layer, new_icon);

  // Destroy the ex-current icon if it existed
  if (wld->icon != NULL) {
    gbitmap_destroy(wld->icon);
  }
  wld->icon = new_icon;
}

void weather_layer_destroy(WeatherLayer* weather_layer){

  WeatherLayerData *wld = layer_get_data(weather_layer);

  text_layer_destroy(wld->temp_layer);
  text_layer_destroy(wld->temp_layer_background);
  bitmap_layer_destroy(wld->icon_layer);

  // Destroy the previous bitmap if we have one
  if (wld->icon != NULL){
    gbitmap_destroy(wld->icon);
  }
  layer_destroy(weather_layer);

  fonts_unload_custom_font(large_font);
  fonts_unload_custom_font(small_font);
}
