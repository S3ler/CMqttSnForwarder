#ifndef CMQTTSNFORWARDER_GLOBAL_DEFINES_H
#define CMQTTSNFORWARDER_GLOBAL_DEFINES_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_DEVICE_ADDRESS_LENGTH
#define MQTT_SN_DEVICE_ADDRESS_LENGTH 6
#endif

typedef struct device_address_ {
  uint8_t bytes[MQTT_SN_DEVICE_ADDRESS_LENGTH];
} device_address;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GLOBAL_DEFINES_H
