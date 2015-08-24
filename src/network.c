#include <pebble.h>

#include "network.h"
#include "weather.h"
#include "keys.h"

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");

  Tuple *temperature_tuple = dict_find(iterator, KEY_TEMP);
  Tuple *icon_tuple = dict_find(iterator, KEY_ICON);
  Tuple *duration_tuple = dict_find(iterator, KEY_DURATION);

  if(temperature_tuple && icon_tuple){
    weather_info_t weather;

    weather.temperature = temperature_tuple->value->int32;
    weather.icon = icon_tuple->value->int32;
    weather.duration = duration_tuple->value->int32;

    weather_update(&weather);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed: %i!", reason);

  /* retry on timeout */
  if(reason == APP_MSG_SEND_TIMEOUT){
    network_request_update();
  }
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void network_request_update(){
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  dict_write_uint8(iter, KEY_REQUEST_UPDATE, 42);

  app_message_outbox_send();
  APP_LOG(APP_LOG_LEVEL_INFO, "Requested update");
}

void network_init(){
  /* Register callbacks */
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  /* Open AppMessage */
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void network_deinit(){
  app_message_deregister_callbacks();
}

