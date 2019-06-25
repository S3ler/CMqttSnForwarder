//
// Created by bele on 24.05.19.
//

#ifndef CMQTTSNFORWARDER_PLATFORM_MQTTSNMESSAGEDATA_H_
#define CMQTTSNFORWARDER_PLATFORM_MQTTSNMESSAGEDATA_H_

#include <stdint.h>
#include "device_address.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH
#define MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH 1024
#endif

typedef struct MqttSnMessageData_ {
  uint64_t received_time;
  device_address from;
  device_address to;
  uint16_t data_length;
  uint8_t signal_strength;
  uint8_t data[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
} MqttSnMessageData;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PLATFORM_MQTTSNMESSAGEDATA_H_
