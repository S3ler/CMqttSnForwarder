//
// Created by bele on 24.05.19.
//

#include "MqttSnMessageData.h"
#include <string.h>
#include <logging/MqttSnLoggingInterface.h>

int32_t reuse_mqtt_sn_message_data(MqttSnMessageData *mqtt_sn_message_data) {
  uint64_t current_time;
  if (get_timestamp_s(&current_time) < 0) {
    return -1;
  }
  device_address to = mqtt_sn_message_data->from;
  mqtt_sn_message_data->from = mqtt_sn_message_data->to;
  mqtt_sn_message_data->to = to;
  mqtt_sn_message_data->data_length = 0;
  memset(mqtt_sn_message_data->data, 0, sizeof(mqtt_sn_message_data->data));
  return 0;
}
