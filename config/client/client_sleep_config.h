//
// Created by SomeDude on 06.08.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_CLIENT_SLEEP_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_CLIENT_SLEEP_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLogging.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFAULT_CLIENT_MQTT_SN_SLEEP_DURATION
#define DEFAULT_CLIENT_MQTT_SN_SLEEP_DURATION (60 * 10)
#endif

typedef struct client_sleep_config_ {
    uint16_t sleep_duration;
} client_sleep_config;

int32_t client_sleep_config_init(client_sleep_config *cfg);
void    client_sleep_config_cleanup(client_sleep_config *cfg);

int32_t is_client_sleep_config_command(char *arg, int *i);
int32_t client_sleep_config_process_args(client_sleep_config *cfg, const MqttSnLogger *logger, int argc, char **argv);

void client_sleep_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_sleep_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CONFIG_CLIENT_CLIENT_SLEEP_CONFIG_H_
