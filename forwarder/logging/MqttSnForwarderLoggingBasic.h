//
// Created by SomeDude on 11.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGBASIC_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGBASIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MqttSnForwarderLogging.h"


int log_int8(const MqttSnLogger *logger, int8_t n);
int log_int16(const MqttSnLogger *logger, int16_t n);
int log_int32(const MqttSnLogger *logger, int32_t n);
int log_int64(const MqttSnLogger *logger, int64_t n);

int log_uint8(const MqttSnLogger *logger, uint8_t n);
int log_uint16(const MqttSnLogger *logger, uint16_t n);
int log_uint32(const MqttSnLogger *logger, uint32_t n);
int log_uint64(const MqttSnLogger *logger, uint64_t n);
int log_uintmax(const MqttSnLogger *logger, uintmax_t n);

int log_char_key_int8_value(const MqttSnLogger *logger, char key, int8_t val);

int log_char_key_uint8_value(const MqttSnLogger *logger, char key, uint8_t val);
int log_char_key_uint16_value(const MqttSnLogger *logger, char key, uint16_t val);

int log_str_key_uint8_value(const MqttSnLogger *logger, const char* key, uint8_t val);
int log_str_key_uint16_value(const MqttSnLogger *logger, const char *key, uint16_t val);

int log_uint8_array(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_length);


int get_timestamp(uint64_t *t);

int log_current_time(const MqttSnLogger *logger);

/**
 * Print timestamp + ":"
 * @param logger
 * @return -1 on error, 0 otherwise
 */
int log_msg_start(const MqttSnLogger *logger);

int log_device_address(const MqttSnLogger *logger, const device_address *address);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGBASIC_H
