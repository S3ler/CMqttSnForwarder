#ifndef GATEWAY_GLOBAL_DEFINES_H
#define GATEWAY_GLOBAL_DEFINES_H

#include <stdint.h>

#define CMQTTSNFORWARDER_DEVICE_ADDRESS_LENGTH 6

typedef struct device_address {
  uint8_t bytes[CMQTTSNFORWARDER_DEVICE_ADDRESS_LENGTH];
} device_address;



#endif //GATEWAY_GLOBAL_DEFINES_H
