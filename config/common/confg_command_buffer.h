//
// Created by SomeDude on 17.09.2019.
//

#ifndef CMQTTSNFORWARDER_CONFG_COMMAND_BUFFER_H
#define CMQTTSNFORWARDER_CONFG_COMMAND_BUFFER_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#ifndef MQTT_SN_CONFIG_COMMAND_BUFFER_ARG_BUFFER_MAXIMUM_LENGTH
#define MQTT_SN_CONFIG_COMMAND_BUFFER_ARG_BUFFER_MAXIMUM_LENGTH 1024
#endif
#ifndef MQTT_SN_CONFIG_COMMAND_BUFFER_ARG_ARGV_MAXIMUM_LENGTH
#define MQTT_SN_CONFIG_COMMAND_BUFFER_ARG_ARGV_MAXIMUM_LENGTH 1024
#endif

typedef struct mqtt_sn_config_command_buffer_ {
    char    argv_0[1];
    char    buffer[MQTT_SN_CONFIG_COMMAND_BUFFER_ARG_BUFFER_MAXIMUM_LENGTH];
    int32_t argc;
    char *  argv[MQTT_SN_CONFIG_COMMAND_BUFFER_ARG_ARGV_MAXIMUM_LENGTH];
} mqtt_sn_config_command_buffer;

void                          mqtt_sn_config_command_buffer_init(mqtt_sn_config_command_buffer *buffer);
void                          mqtt_sn_config_command_buffer_reset(mqtt_sn_config_command_buffer *buffer);
mqtt_sn_config_command_buffer mqtt_sn_config_command_buffer_cp(mqtt_sn_config_command_buffer *buffer);
int32_t                       mqtt_sn_config_command_buffer_parse_string(mqtt_sn_config_command_buffer *buffer);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CONFG_COMMAND_BUFFER_H
