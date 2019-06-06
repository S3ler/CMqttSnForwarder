#ifndef CMQTTSNFORWARDER_GLOBAL_DEFINES_H
#define CMQTTSNFORWARDER_GLOBAL_DEFINES_H

#include <stdint.h>

#ifndef MQTT_SN_DEVICE_ADDRESS_LENGTH
#define MQTT_SN_DEVICE_ADDRESS_LENGTH 6
#endif

typedef struct device_address_ {
  uint8_t bytes[MQTT_SN_DEVICE_ADDRESS_LENGTH];
  uint16_t len;
} device_address;

#endif //CMQTTSNFORWARDER_GLOBAL_DEFINES_H
