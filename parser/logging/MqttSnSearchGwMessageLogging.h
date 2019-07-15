//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNSEARCHGWMESSAGELOGGING_H_
#define CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNSEARCHGWMESSAGELOGGING_H_

#include <logging/MqttSnLoggingInterface.h>

int32_t log_searchgw_message_byte(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_len);

int32_t log_searchgw_message_struct(const MqttSnLogger *logger, MqttSnSearchGw *search_gw);

int32_t log_searchgw_message(const MqttSnLogger *logger, uint8_t radius);

int32_t log_searchgw_radius(const MqttSnLogger *logger, uint8_t radius);

#endif //CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNSEARCHGWMESSAGELOGGING_H_
