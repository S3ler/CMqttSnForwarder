//
// Created by SomeDude on 08.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <platform/device_address.h>
#include <stdint.h>

#ifdef WITH_LOGGING

typedef enum MQTT_SN_FORWARDER_NETWORK_ {
  MQTT_SN_FORWARDER_NETWORK_INVALID = -1,
  MQTT_SN_FORWARDER_NETWORK_GATEWAY = 0,
  MQTT_SN_FORWARDER_NETWORK_CLIENT = 1
} MQTT_SN_FORWARDER_NETWORK;

typedef enum {
  LOG_LEVEL_QUIET = 0,
  LOG_LEVEL_DEFAULT = 1,
  LOG_LEVEL_VERBOSE = 2,
#ifdef WITH_DEBUG_LOGGING
  LOG_LEVEL_DEBUG = 3
#endif
} log_level_t;

typedef struct MqttSnLoggerInterface_ {
  int (*log_init)(struct MqttSnLoggerInterface_ *logger);

  void (*log_deinit)(struct MqttSnLoggerInterface_ *logger);

  int (*log_flush)(const struct MqttSnLoggerInterface_ *logger);

  int (*log_str)(const char *str);

  int (*log_char)(char c);

  int log_level;
  int status;
} MqttSnLogger;

int MqttSnLoggerInit(MqttSnLogger *logger,
                     log_level_t log_level,
                     int (*log_init)(struct MqttSnLoggerInterface_ *logger));

void MqttSnLoggerDeinit(MqttSnLogger *logger);

int log_status(const MqttSnLogger *logger);

int is_logger_not_available(const MqttSnLogger *logger);

int shall_not_be_logged(const MqttSnLogger *logger, int max_level);

int log_flush(const MqttSnLogger *logger);

int log_char(const MqttSnLogger *logger, char c);

int log_str(const MqttSnLogger *logger, const char *str);

int log_int8(const MqttSnLogger *logger, int8_t n);
int log_int16(const MqttSnLogger *logger, int16_t n);
int log_int32(const MqttSnLogger *logger, int32_t n);
int log_int64(const MqttSnLogger *logger, int64_t n);
int log_intmax(const MqttSnLogger *logger, uintmax_t n);

int log_uint8(const MqttSnLogger *logger, uint8_t n);
int log_uint16(const MqttSnLogger *logger, uint16_t n);
int log_uint32(const MqttSnLogger *logger, uint32_t n);
int log_uint64(const MqttSnLogger *logger, uint64_t n);
int log_uintmax(const MqttSnLogger *logger, uintmax_t n);

int log_char_key_int8_value(const MqttSnLogger *logger, char key, int8_t val);

int log_char_key_uint8_value(const MqttSnLogger *logger, char key, uint8_t val);
int log_char_key_uint16_value(const MqttSnLogger *logger, char key, uint16_t val);

int log_str_key_uint8_value(const MqttSnLogger *logger, const char *key, uint8_t val);
int log_str_key_uint16_value(const MqttSnLogger *logger, const char *key, uint16_t val);

int log_uint8_array(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_length);

int get_timestamp(uint64_t *t);

int log_current_time(const MqttSnLogger *logger);

int log_msg_start(const MqttSnLogger *logger);

int log_device_address(const MqttSnLogger *logger, const device_address *address);

#endif
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
