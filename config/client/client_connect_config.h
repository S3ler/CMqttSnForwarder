//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_CONNECT_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_CONNECT_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFAULT_CLIENT_CONNECT_MQTT_SN_GATEWAY_CONFIG_CLIENT_ID
#if defined(Arduino_h)
#else
#define DEFAULT_CLIENT_CONNECT_MQTT_SN_GATEWAY_CONFIG_CLIENT_ID {(char)('a' + (random() % (2*26))), (char)('a' + (random() % (2*26))), '\0'}
#endif
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_TIMEOUT
#define DEFAULT_MQTT_SN_CLIENT_CONNECT_TIMEOUT 30000
#endif

typedef struct client_connect_config_ {
  uint16_t duration;
  char default_client_id[24];
  char *client_id;
  char *will_topic;
  char *will_msg;
  uint8_t clean_session;
  int32_t client_connect_timeout;
} client_connect_config;

int32_t client_connect_config_init(client_connect_config *cfg);
void client_connect_config_cleanup(client_connect_config *cfg);

int32_t is_client_connect_config_command(const char *arg, int *i);
int32_t client_connect_config_process_args(client_connect_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char *argv[]);

void client_connect_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_connect_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_CONNECT_CONFIG_H_
