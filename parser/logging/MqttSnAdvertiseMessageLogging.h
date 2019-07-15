//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNADVERTISEMESSAGELOGGING_H_
#define CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNADVERTISEMESSAGELOGGING_H_

#include <logging/MqttSnLoggingInterface.h>
#include <parser/MqttSnMessageParser.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_advertise_message_byte(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_len);

int32_t log_advertise_message_struct(const MqttSnLogger *logger, MqttSnAdvertise *advertise);

int32_t log_advertise_message(const MqttSnLogger *logger, uint8_t gw_id, uint16_t duration);

int32_t log_advertise_gw_id(const MqttSnLogger *logger, uint8_t gw_id);

int32_t log_advertise_duration(const MqttSnLogger *logger, uint16_t duration);

int32_t log_advertise_gen_error(const MqttSnLogger *logger,
                                const char *file_name,
                                const char *function_name,
                                int line_number);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNADVERTISEMESSAGELOGGING_H_
