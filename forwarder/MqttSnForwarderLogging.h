//
// Created by SomeDude on 08.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <global_defines.h>

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

typedef struct MqttSnLogger_ {
  int (*log_init)(struct MqttSnLogger_ *logger);

  void (*log_deinit)(struct MqttSnLogger_ *logger);

  int (*log_flush)(const struct MqttSnLogger_ *logger);

  int (*log_str)(const char *str);

  int (*log_char)(char c);

  int log_level;
  int status;
} MqttSnLogger;

int MqttSnLoggerInit(MqttSnLogger *logger, log_level_t log_level, int (*log_init)(struct MqttSnLogger_ *logger));

void MqttSnLoggerDeinit(MqttSnLogger *logger);

int log_status(const MqttSnLogger *logger);

int is_logger_not_available(const MqttSnLogger *logger);

int shall_not_be_logged(const MqttSnLogger *logger, int max_level);

int log_flush(const MqttSnLogger *logger);

int log_char(const MqttSnLogger *logger, char c);

int log_str(const MqttSnLogger *logger, const char *str);

int log_forwarder_started(const MqttSnLogger *logger,
                          const char *version,
                          int major,
                          int minor,
                          int tweak,
                          const char *build_date);

int log_forwarder_terminated(const MqttSnLogger *logger,
                             const char *version,
                             uint32_t major,
                             uint32_t minor,
                             uintmax_t tweak);

int log_network_connect(const MqttSnLogger *logger,
                        const char *protocol_name,
                        const char *network_name,
                        const device_address *as,
                        const device_address *to);

int log_network_connect_fail(const MqttSnLogger *logger,
                             const char *protocol_name,
                             const char *network_name,
                             const device_address *as,
                             const device_address *to);

int log_network_disconnect(const MqttSnLogger *logger,
                           const char *protocol_name,
                           const char *network_name,
                           const device_address *address);

int log_protocol_mismatch(const MqttSnLogger *logger, const char *expected, const char *actual);

int log_too_long_message(const MqttSnLogger *logger,
                         const device_address *from,
                         MQTT_SN_FORWARDER_NETWORK network,
                         const uint8_t *data,
                         uint16_t data_len);

#ifdef WITH_DEBUG_LOGGING

int log_db_rec_client_message(const MqttSnLogger *logger,
                              const device_address *from,
                              const device_address *to,
                              const uint8_t *data,
                              uint16_t data_len);

int log_db_rec_gateway_message(const MqttSnLogger *logger,
                               const device_address *from,
                               const device_address *to,
                               const uint8_t *data,
                               uint16_t data_len);

int log_db_send_client_message(const MqttSnLogger *logger,
                               const device_address *from,
                               const device_address *dst,
                               const uint8_t *data,
                               uint16_t data_len);

int log_db_send_gateway_message(const MqttSnLogger *logger,
                                const device_address *from,
                                const device_address *dst,
                                const uint8_t *data,
                                uint16_t data_len);

int log_incomplete_message(const MqttSnLogger *logger,
                           const device_address *address,
                           const uint8_t *data,
                           uint16_t data_len);

#endif

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
