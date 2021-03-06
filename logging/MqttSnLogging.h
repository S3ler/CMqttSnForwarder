//
// Created by SomeDude on 08.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H

#include <platform/device_address.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum MQTT_SN_FORWARDER_NETWORK_ {
    MQTT_SN_FORWARDER_NETWORK_INVALID = 0,
    MQTT_SN_FORWARDER_NETWORK_GATEWAY = 1,
    MQTT_SN_FORWARDER_NETWORK_CLIENT  = 2
} MQTT_SN_FORWARDER_NETWORK;

typedef enum {
    LOG_LEVEL_QUIET   = 0,
    LOG_LEVEL_DEFAULT = 1,
    LOG_LEVEL_VERBOSE = 2,
#ifdef WITH_DEBUG_LOGGING
    LOG_LEVEL_DEBUG = 3
#endif
} log_level_t;

typedef struct MqttSnLogger_ {
    int32_t (*log_init)(struct MqttSnLogger_ *logger);
    void (*log_deinit)(struct MqttSnLogger_ *logger);
    int32_t (*log_str)(const struct MqttSnLogger_ *logger, const char *str);
    int32_t (*log_char)(const struct MqttSnLogger_ *logger, char c);
    int32_t (*log_flush)(const struct MqttSnLogger_ *logger);
    log_level_t log_level;
    int32_t     status;
    void *context;

    char *log_identifier;
    char *log_filepath;
} MqttSnLogger;

int32_t MqttSnLoggerInit(MqttSnLogger *logger, log_level_t log_level, int32_t (*log_init)(struct MqttSnLogger_ *logger));

int32_t MqttSnLoggerInitFile(MqttSnLogger *logger, log_level_t log_level, char *log_filepath, int32_t (*log_init)(struct MqttSnLogger_ *),
                             void *context);

void MqttSnLoggerDeinit(MqttSnLogger *logger);

int32_t log_status(const MqttSnLogger *logger);

int32_t is_logger_not_available(const MqttSnLogger *logger);

int32_t shall_not_be_logged(const MqttSnLogger *logger, log_level_t max_level);

int32_t log_flush(const MqttSnLogger *logger);

int32_t log_char(const MqttSnLogger *logger, char c);

int32_t log_str(const MqttSnLogger *logger, const char *str);

int32_t log_bool(const MqttSnLogger *logger, uint8_t n);

int32_t log_int8(const MqttSnLogger *logger, int8_t n);
int32_t log_int16(const MqttSnLogger *logger, int16_t n);
int32_t log_int32(const MqttSnLogger *logger, int32_t n);
int32_t log_int64(const MqttSnLogger *logger, int64_t n);
int32_t log_intmax(const MqttSnLogger *logger, uintmax_t n);

int32_t log_uint8(const MqttSnLogger *logger, uint8_t n);
int32_t log_uint16(const MqttSnLogger *logger, uint16_t n);
int32_t log_uint32(const MqttSnLogger *logger, uint32_t n);
int32_t log_uint64(const MqttSnLogger *logger, uint64_t n);
int32_t log_uintmax(const MqttSnLogger *logger, uintmax_t n);

int32_t log_char_key_int8_value(const MqttSnLogger *logger, char key, int8_t val);

int32_t log_char_key_uint8_value(const MqttSnLogger *logger, char key, uint8_t val);
int32_t log_char_key_uint16_value(const MqttSnLogger *logger, char key, uint16_t val);

int32_t log_str_key_uint8_value(const MqttSnLogger *logger, const char *key, uint8_t val);
int32_t log_str_key_uint16_value(const MqttSnLogger *logger, const char *key, uint16_t val);

int32_t log_uint8_array(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_length);

int32_t get_timestamp_s(uint64_t *t_s);

int32_t get_timestamp_ms(uint64_t *t_s, uint64_t *t_ms);

int32_t  get_timestamp_ns(uint64_t *t_s, uint64_t *t_ns);
uint64_t timestamp_s_ns_to_ns(uint64_t t_s, uint64_t t_ns);
uint64_t timestamp_s_ns_to_ms(uint64_t t_s, uint64_t t_ns);

int32_t log_current_time(const MqttSnLogger *logger);

int32_t log_identifier(const MqttSnLogger *logger);

int32_t log_msg_start(const MqttSnLogger *logger);

int32_t log_device_address(const MqttSnLogger *logger, const device_address *address);

int32_t log_signal_strength(const MqttSnLogger *logger, uint8_t signal_strength);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
