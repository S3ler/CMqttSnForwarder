//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNGWINFOMESSAGELOGGING_H_
#define CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNGWINFOMESSAGELOGGING_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <parser/MqttSnGwInfoMessage.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_gwinfo_message_byte(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_len);

int32_t log_gwinfo_message_struct(const MqttSnLogger* logger, MqttSnGwInfo* gw_info);

int32_t log_gwinfo_message(const MqttSnLogger *logger,
                           uint8_t gw_id,
                           const device_address *gw_add,
                           uint16_t gw_add_len);

int32_t log_gwinfo_gw_id(const MqttSnLogger *logger, uint8_t gw_id);

int32_t log_gwinfo_gw_add(const MqttSnLogger *logger, const device_address *gw_add, uint16_t gw_add_len);

int32_t log_gwinfo_gen_error(const MqttSnLogger *logger,
                                const char *file_name,
                                const char *function_name,
                                int line_number);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNGWINFOMESSAGELOGGING_H_
