//
// Created by SomeDude on 27.08.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_NETWORK_PARAMETER_CONFIG_H
#define CMQTTSNFORWARDER_CLIENT_NETWORK_PARAMETER_CONFIG_H
#include <stdint.h>
#include <logging/MqttSnLogging.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_TIMEOUT_MS
#define DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_TIMEOUT_MS (5 * 1000)
#endif

#ifndef DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_COUNT
#define DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_COUNT 3
#endif

#ifndef DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_CONGESTION_DELAY_S
#define DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_CONGESTION_DELAY_S (5 * 60)
#endif

#ifndef DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_SIGNAL_STRENGTH
#define DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_SIGNAL_STRENGTH UINT8_MAX
#endif
typedef struct client_network_parameter_config_ {
    int32_t  retry_timeout_ms;
    int32_t  retry_count;
    uint64_t congestion_delay_s;
    uint8_t  default_signal_strength;
} client_network_parameter_config;

int32_t client_network_parameter_config_init(client_network_parameter_config *cfg);
void    client_network_parameter_config_cleanup(client_network_parameter_config *cfg);

int32_t is_client_network_parameter_config_command(const char *arg, int *i);
int32_t client_network_parameter_config_process_args(client_network_parameter_config *cfg, const MqttSnLogger *logger, int argc,
                                                     char *argv[]);

void client_network_config_parameter_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_network_config_parameter_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CLIENT_NETWORK_PARAMETER_CONFIG_H
