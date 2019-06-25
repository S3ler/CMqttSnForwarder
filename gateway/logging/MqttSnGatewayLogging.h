//
// Created by SomeDude on 20.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYLOGGING_H_
#define CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYLOGGING_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_something(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYLOGGING_H_
