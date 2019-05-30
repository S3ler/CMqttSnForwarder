//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_CONFIG_ECHOGATEWAY_CONFIG_H_
#define CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_CONFIG_ECHOGATEWAY_CONFIG_H_

#include <config/common/mqtt_sn_version_config.h>
#include <config/network/client_network_config.h>
#include <config/common/mqtt_sn_logger_config.h>
#include <config/gateway/gateway_advertisment_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct echogateway_config_ {
  mqtt_sn_version_config msvcfg;
  client_network_config cncfg;
  gateway_advertisement_config gacfg;
  mqtt_sn_logger_config mslcfg;
} echogateway_config;

int32_t echogateway_config_init(echogateway_config *cfg);
void echogateway_config_cleanup(echogateway_config *cfg);

int32_t process_echogateway_config_line(echogateway_config *cfg, const MqttSnLogger *logger, int argc, char *argv[]);

void echogateway_config_print_usage(const MqttSnLogger *logger);
void echogateway_config_print_usage_short(const MqttSnLogger *logger);
void echogateway_config_print_usage_long(const MqttSnLogger *logger);
void echogateway_config_print_see_usage(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_CONFIG_ECHOGATEWAY_CONFIG_H_
