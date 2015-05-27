#include <pebble.h>

#include "weather.h"
#include "weather_layer.h"

static TextLayer *background_layer;
static TextLayer *temp_layer;
static GBitmap *bitmap;
static BitmapLayer *icon_layer;

static GFont large_font, small_font;
WeatherLayer *weather_layer;

WeatherLayer *weather_layer_create(GRect frame){
  // Create a new layer with some extra space to save our custom Layer infos
  weather_layer = layer_create(frame);

  large_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FUTURA_40));
  small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FUTURA_35));

  // Add background layer
  background_layer = text_layer_create(GRect(0, 10, 144, 68));
  text_layer_set_background_color(background_layer, GColorWhite);
  layer_add_child(weather_layer, text_layer_get_layer(background_layer));

  // Add temperature layer
  temp_layer = text_layer_create(GRect(70, 19, 72, 80));
  text_layer_set_background_color(temp_layer, GColorClear);
  text_layer_set_text_alignment(temp_layer, GTextAlignmentCenter);
  text_layer_set_font(temp_layer, large_font);
  layer_add_child(weather_layer, text_layer_get_layer(temp_layer));

  // Add bitmap layer
  icon_layer = bitmap_layer_create(GRect(9, 13, 60, 60));
  layer_add_child(weather_layer, bitmap_layer_get_layer(icon_layer));

  bitmap = NULL;

  return weather_layer;
}

static void weather_layer_set_temp(int temp){
  static char temp_str[6];
  snprintf(temp_str, sizeof(temp_str), "%i°", temp);

  text_layer_set_text(temp_layer, temp_str);
}

static void weather_layer_set_icon(uint32_t icon){
  GBitmap *new_bitmap = gbitmap_create_with_resource(icon);
  // Display the new bitmap
  bitmap_layer_set_bitmap(icon_layer, new_bitmap);

  // Destroy the ex-current bitmap if it existed
  if (bitmap != NULL) {
    gbitmap_destroy(bitmap);
  }
  bitmap = new_bitmap;
}

void weather_layer_draw(){
  if(!bluetooth_connection_service_peek()){
    /* bluetooth disconnected */
    weather_layer_set_icon(RESOURCE_ID_ICON_LOADING);
  } else if(weather_available()) {
    weather_layer_set_temp(weather_get_temperature());
    weather_layer_set_icon(weather_get_icon());
  } else {
    weather_layer_set_icon(RESOURCE_ID_ICON_LOADING);
  }
}

void weather_layer_destroy(WeatherLayer* weather_layer){
  text_layer_destroy(temp_layer);
  text_layer_destroy(background_layer);
  bitmap_layer_destroy(icon_layer);

  // Destroy the previous bitmap if we have one
  if (bitmap != NULL){
    gbitmap_destroy(bitmap);
  }
  layer_destroy(weather_layer);

  fonts_unload_custom_font(large_font);
  fonts_unload_custom_font(small_font);
}

