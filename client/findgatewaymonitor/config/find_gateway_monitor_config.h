//
// Created by SomeDude on 03.06.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_CONFIG_FIND_GATEWAY_MONITOR_CONFIG_H_
#define CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_CONFIG_FIND_GATEWAY_MONITOR_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <config/common/mqtt_sn_version_config.h>
#include <config/common/mqtt_sn_logger_config.h>
#include <config/network/gateway/gateway_network_config.h>
#include <config/client/client_find_mqtt_sn_gateway_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FINDGATEWAYMONITOR_EXECUTABLE_NAME
#define FINDGATEWAYMONITOR_EXECUTABLE_NAME "broadcast_monitor"
#endif

typedef struct find_gateway_monitor_config_ {
  char executable_name[sizeof(FINDGATEWAYMONITOR_EXECUTABLE_NAME)];
  mqtt_sn_version_config msvcfg;
  mqtt_sn_logger_config mslcfg;
  gateway_network_config gncfg;
  client_find_mqtt_sn_gateway_config cfmqsgcfg;
  uint8_t interactive;
} find_gateway_monitor_config;

int32_t find_gateway_monitor_config_init(find_gateway_monitor_config *cfg);
void find_gateway_monitor_config_cleanup(find_gateway_monitor_config *cfg);


int32_t is_find_gateway_monitor_config_command(const char* arg, int *i);
int32_t find_gateway_monitor_config_process_line(find_gateway_monitor_config *cfg,
                                                 const MqttSnLogger *logger,
                                                 int argc,
                                                 char *argv[]);
int32_t find_gateway_monitor_config_process_args(find_gateway_monitor_config *cfg,
                                                 const MqttSnLogger *logger,
                                                 int argc,
                                                 char *argv[]);

void find_gateway_monitor_config_print_usage(const MqttSnLogger *logger);
void find_gateway_monitor_config_print_usage_short(const MqttSnLogger *logger);
void find_gateway_monitor_config_print_usage_long(const MqttSnLogger *logger);
void find_gateway_monitor_config_print_see_usage(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_CONFIG_FIND_GATEWAY_MONITOR_CONFIG_H_
