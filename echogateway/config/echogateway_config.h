//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_CONFIG_ECHOGATEWAY_CONFIG_H_
#define CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_CONFIG_ECHOGATEWAY_CONFIG_H_

#include <config/common/mqtt_sn_version_config.h>
#include <config/network/client/client_network_config.h>
#include <config/common/mqtt_sn_logger_config.h>
#include <config/gateway/gateway_advertisment_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_ECHOGATEWAY_EXECUTABLE_NAME
#define MQTT_SN_ECHOGATEWAY_EXECUTABLE_NAME "echogateway"
#endif


typedef struct echogateway_config_ {
  char executable_name[sizeof(MQTT_SN_ECHOGATEWAY_EXECUTABLE_NAME)];
  mqtt_sn_version_config msvcfg;
  mqtt_sn_logger_config mslcfg;
  client_network_config cncfg;
  gateway_advertise_config gacfg;
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
