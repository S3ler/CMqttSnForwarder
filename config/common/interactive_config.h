//
// Created by SomeDude on 22.09.2019.
//

#ifndef CMQTTSNFORWARDER_INTERACTIVE_CONFIG_H
#define CMQTTSNFORWARDER_INTERACTIVE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct interactive_config_ {
    uint8_t interactive;
} interactive_config;

int32_t interactive_config_init(find_gateway_monitor_config *cfg);
void interactive_config_cleanup(find_gateway_monitor_config *cfg);
int32_t is_interactive_config_command(const char* arg, int *i);
void find_interactive_print_usage_short(const MqttSnLogger *logger);
void find_interactive_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_INTERACTIVE_CONFIG_H
