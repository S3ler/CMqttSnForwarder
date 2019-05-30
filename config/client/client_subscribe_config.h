//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS
#define CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS 5
#endif

typedef struct client_subscription_config_ {
  int8_t qos;
  uint8_t topic_id_type;
  uint16_t topic_id;
  char *topic_name;
} client_subscription_config;

typedef struct client_subscribe_config_ {
  uint16_t subscription_list_max_len;
  uint16_t subscription_len;
  client_subscription_config subscription_list[CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS];
} client_subscribe_config;

int32_t client_subscribe_config_init(client_subscribe_config *cfg);
void client_subscribe_config_cleanup(client_subscribe_config *cfg);
void client_subscription_config_cleanup(client_subscription_config *cfg);

int32_t is_client_subscribe_config_command(const char *arg, int *i);
int32_t client_subscribe_config_process_args(client_subscribe_config *cfg,
                                             const MqttSnLogger *logger,
                                             int argc,
                                             char *argv[]);

void client_subscribe_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_subscribe_config_print_usage_long(const MqttSnLogger *logger);

int32_t parse_client_subscription_config(const MqttSnLogger *logger, char *url, client_subscription_config *cfg);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_
