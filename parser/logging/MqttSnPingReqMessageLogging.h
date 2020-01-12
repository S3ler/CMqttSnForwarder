//
// Created by SomeDude on 23.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNPINGREQMESSAGELOGGING_H
#define CMQTTSNFORWARDER_MQTTSNPINGREQMESSAGELOGGING_H
#include <stdint.h>
#include <logging/MqttSnLogging.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t log_pingreq_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNPINGREQMESSAGELOGGING_H
