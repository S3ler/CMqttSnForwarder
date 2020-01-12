//
// Created by SomeDude on 01.07.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_GATEWAY_GATEWAY_CLIENT_CONNECTION_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_GATEWAY_GATEWAY_CLIENT_CONNECTION_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include <logging/MqttSnLogging.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_CHECK_ENABLED
#define MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_CHECK_ENABLED 1
#endif

#ifndef MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_CHECK_PERIOD
#define MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_CHECK_PERIOD 20
#endif

#ifndef MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_OFFSET
#define MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_OFFSET -20
#endif

typedef struct gateway_client_connection_config_ {
  uint8_t client_connection_timeout_check_enabled;
  uint32_t client_connection_timeout_check_period;
  int32_t client_connection_timeout_offset;
} gateway_client_connection_config;

int32_t gateway_client_connection_config_init(gateway_client_connection_config *cfg);
void gateway_client_connection_config_cleanup(gateway_client_connection_config *cfg);

int32_t is_gateway_client_connection_config_command(const char* arg, int *i);
int32_t gateway_client_connection_config_process_args(gateway_client_connection_config *cfg,
                                                  const MqttSnLogger *logger,
                                                  int argc,
                                                  char **argv);

void gateway_client_connection_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void gateway_client_connection_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_GATEWAY_GATEWAY_CLIENT_CONNECTION_CONFIG_H_
