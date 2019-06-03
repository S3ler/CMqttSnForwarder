//
// Created by SomeDude on 03.06.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_CONFIG_FIND_GATEWAY_MONITOR_CONFIG_H_
#define CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_CONFIG_FIND_GATEWAY_MONITOR_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct find_gateway_monitor_config_ {
  uint8_t interactive;
} find_gateway_monitor_config;

int32_t find_gateway_monitor_config_init(find_gateway_monitor_config *cfg);
int32_t find_gateway_monitor_config_cleanup(find_gateway_monitor_config *cfg);


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
