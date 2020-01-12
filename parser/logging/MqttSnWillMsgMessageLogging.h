//
// Created by SomeDude on 23.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNWILLMSGMESSAGELOGGING_H
#define CMQTTSNFORWARDER_MQTTSNWILLMSGMESSAGELOGGING_H

#include <stdint.h>
#include <logging/MqttSnLogging.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t log_will_msg_msg_length_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number);
int32_t log_will_msg_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNWILLMSGMESSAGELOGGING_H
