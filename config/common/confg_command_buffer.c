//
// Created by SomeDude on 17.09.2019.
//

#include "confg_command_buffer.h"
#include "config_command_helper.h"
#include <string.h>

void mqtt_sn_config_command_buffer_init(mqtt_sn_config_command_buffer *buffer) {
    memset(buffer, 0, sizeof(*buffer));
    buffer->argc                 = 0;
    buffer->argv_0[0]            = '\0';
    buffer->argv[buffer->argc++] = buffer->argv_0;
}
void mqtt_sn_config_command_buffer_reset(mqtt_sn_config_command_buffer *buffer) {
    mqtt_sn_config_command_buffer_init(buffer);
}
mqtt_sn_config_command_buffer mqtt_sn_config_command_buffer_cp(mqtt_sn_config_command_buffer *buffer) {
    mqtt_sn_config_command_buffer buffer_cp = (*buffer);
    buffer_cp.argv[0]                       = buffer_cp.argv_0;
    return buffer_cp;
}
int32_t mqtt_sn_config_command_buffer_parse_string(mqtt_sn_config_command_buffer *buffer) {
    buffer->argc = tokenize_command_str(buffer->buffer, strlen(buffer->buffer) + 1, buffer->argv_0, buffer->argv,
                                        MQTT_SN_CONFIG_COMMAND_BUFFER_ARG_ARGV_MAXIMUM_LENGTH);
    if (buffer->argc < 0) {
        return -1;
    }
    return 0;
}
