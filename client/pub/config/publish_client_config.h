//
// Created by SomeDude on 27.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_PUB_CONFIG_CLIENT_PUBLISH_CONFIG_H_
#define CMQTTSNFORWARDER_CLIENT_PUB_CONFIG_CLIENT_PUBLISH_CONFIG_H_

#include <logging/MqttSnLoggingInterface.h>
#include <config/mqtt_sn_version_config.h>
#include <config/mqtt_sn_gateway_config.h>
#include <config/gateway_network_config.h>
#include <config/mqtt_sn_logger_config.h>
#include <config/client_find_mqtt_sn_gateway_config.h>
#include <config/client_connect_config.h>
#include <config/client_register_config.h>
#include <config/client_publish_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct publish_client_config_ {
  mqtt_sn_version_config msvcfg;
  mqtt_sn_gateway_config msgcfg;
  gateway_network_config gncfg;
  mqtt_sn_logger_config mslcfg;

  client_find_mqtt_sn_gateway_config cfmsgcfg;
  client_connect_config cccfg;
  client_register_config crcfg;
  client_publish_config cpcfg;
} publish_client_config;

int32_t publish_client_config_init(publish_client_config *cfg);
void publish_client_config_cleanup(publish_client_config *cfg);

int32_t is_publish_client_config_command(char *arg, int *i);
int32_t publish_client_config_process_line(publish_client_config *cfg, const MqttSnLogger *logger, int argc, char **argv);
int32_t publish_client_config_process_args(publish_client_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char *argv[]);

void publish_client_config_print_usage(const MqttSnLogger *logger);
void publish_client_config_print_usage_short(const MqttSnLogger *logger);
void publish_client_config_print_usage_long(const MqttSnLogger *logger);
void publish_client_config_print_see_usage(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_PUB_CONFIG_CLIENT_PUBLISH_CONFIG_H_
