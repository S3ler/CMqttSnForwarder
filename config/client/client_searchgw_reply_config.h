//
// Created by SomeDude on 04.09.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_SEARCHGW_REPLY_CONFIG_H
#define CMQTTSNFORWARDER_CLIENT_SEARCHGW_REPLY_CONFIG_H
#include <stdint.h>
#include <logging/MqttSnLogging.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_SEARCH_GW_REPLY_ENDABLED
#define DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_SEARCH_GW_REPLY_ENDABLED 1
#endif

#ifndef DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_GW_INFO_DELAY_MS
#define DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_GW_INFO_DELAY_MS 5000
#endif

typedef struct client_search_gw_reply_config_ {
    uint8_t search_gw_reply_enabled;
    uint16_t gw_info_delay_ms;
} client_search_gw_reply_config;

int32_t client_search_gw_reply_config_init(client_search_gw_reply_config *cfg);
void    client_search_gw_reply_config_cleanup(client_search_gw_reply_config *cfg);

int32_t is_client_search_gw_reply_config_command(const char *arg, int *i);
int32_t client_search_gw_reply_config_process_args(client_search_gw_reply_config *cfg, const MqttSnLogger *logger, int argc, char *argv[]);

void client_search_gw_reply_config_parameter_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_search_gw_reply_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CLIENT_SEARCHGW_REPLY_CONFIG_H
