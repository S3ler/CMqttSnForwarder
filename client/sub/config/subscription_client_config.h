//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_SUB_CONFIG_SUBSCRIBE_CLIENT_CONFIG_H_
#define CMQTTSNFORWARDER_CLIENT_SUB_CONFIG_SUBSCRIBE_CLIENT_CONFIG_H_

#include <config/mqtt_sn_version_config.h>
#include <config/gateway_network_config.h>
#include <config/mqtt_sn_gateway_config.h>
#include <config/mqtt_sn_logger_config.h>
#include <config/client_connect_config.h>
#include <config/client_register_config.h>
#include <config/client_subscribe_config.h>
#include <config/client_find_mqtt_sn_gateway_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct subscription_client_config_ {
  mqtt_sn_version_config msvcfg;
  mqtt_sn_gateway_config msgcfg;
  gateway_network_config gncfg;
  mqtt_sn_logger_config mslcfg;

  client_find_mqtt_sn_gateway_config cfmsgcfg;
  client_connect_config cccfg;
  client_register_config crcfg;
  client_subscribe_config cscfg;
} subscription_client_config;

int32_t subscription_client_config_init(subscription_client_config *cfg);
void subscription_client_config_cleanup(subscription_client_config *cfg);
int32_t subscription_client_config_process_line(subscription_client_config *cfg,
                                                const MqttSnLogger *logger,
                                                int argc,
                                                char **argv);
int32_t is_subscription_client_config_process_command(const char *arg, int *i);
int32_t subscription_client_config_process_args(subscription_client_config *cpfg,
                                                const MqttSnLogger *logger,
                                                int argc,
                                                char **argv);

void subscription_client_config_print_usage(const MqttSnLogger *logger);
void subscription_client_config_print_usage_short(const MqttSnLogger *logger);
void subscription_client_config_print_usage_long(const MqttSnLogger *logger);
void subscription_client_config_print_see_usage(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_SUB_CONFIG_SUBSCRIBE_CLIENT_CONFIG_H_
