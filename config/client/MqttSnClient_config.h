//
// Created by SomeDude on 01.06.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_MQTTSNCLIENT_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_MQTTSNCLIENT_CONFIG_H_

#include <stdint.h>
typedef struct MqttSnClient_config_{
  int32_t default_duration;
  uint16_t connect_duration;
}MqttSnClient_config;
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_MQTTSNCLIENT_CONFIG_H_
