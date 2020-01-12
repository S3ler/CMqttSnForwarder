//
// Created by SomeDude on 27.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_PUB_CONFIG_CLIENT_INTERACTIVE_CONFIG_H_
#define CMQTTSNFORWARDER_CLIENT_PUB_CONFIG_CLIENT_INTERACTIVE_CONFIG_H_

#include <logging/MqttSnLogging.h>
#include <config/common/mqtt_sn_version_config.h>
#include <config/client/client_mqtt_sn_gateway_config.h>
#include <config/network/gateway/gateway_network_config.h>
#include <config/common/mqtt_sn_logger_config.h>
#include <config/client/client_find_mqtt_sn_gateway_config.h>
#include <config/client/client_connect_config.h>
#include <config/client/client_register_config.h>
#include <config/client/client_publish_config.h>
#include <config/client/client_subscribe_config.h>
#include <config/client/client_unsubscribe_config.h>
#include <config/client/client_sleep_config.h>
#include <config/client/client_searchgw_reply_config.h>
#include <config/client/client_network_parameter_config.h>
#include <client/publish/config/publish_client_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct interactive_client_config_buffer_ {
    char executable_name[sizeof(MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME)];

    mqtt_sn_version_config msvcfg;
    client_mqtt_sn_gateway_config msgcfg;
    gateway_network_config gncfg;
    mqtt_sn_logger_config  mslcfg;

    client_network_parameter_config cnpcfg;
    client_search_gw_reply_config   csrcfg;

    client_find_mqtt_sn_gateway_config cfmsgcfg;
    client_connect_config              cccfg;
    client_register_config             crcfg;
    client_subscribe_config            cscfg;
    client_unsubscribe_config          cuscfg;
    client_publish_config              cpcfg;
    client_sleep_config                scfg;

    mqtt_sn_gateway_config_buffer msgcfgb;
    gateway_network_config_container gncfgb;
    mqtt_sn_logger_config_buffer  mslcfgb;

    client_connect_config_buffer     cccfgb;
    client_register_config_buffer    crcfgb;
    client_subscribe_config_buffer   cscfgb;
    client_unsubscribe_config_buffer cuscfgb;
    client_publish_config_buffer     cpcfgb;
} interactive_client_config_container;

int32_t interactive_client_config_copy_to_buffer(interactive_client_config_container *cfg);
void    print_interactive_client_config_print_not_copyable(const MqttSnLogger *logger);

int32_t interactive_client_config_init(interactive_client_config_container *cfg);
void    interactive_client_config_cleanup(interactive_client_config_container *cfg);

int32_t is_interactive_client_config_command(char *arg, int *i);
int32_t interactive_client_config_process_line(interactive_client_config_container *cfg, const MqttSnLogger *logger, int argc, char *argv[]);
int32_t mqtt_sn_cmd_buffer_interactive_client_simulate(interactive_client_config_container *cfg, const MqttSnLogger *logger, int argc,
                                                       char **argv);
int32_t interactive_client_config_process_args(interactive_client_config_container *cfg, const MqttSnLogger *logger, int argc, char *argv[]);

void interactive_client_config_print_usage(const MqttSnLogger *logger);
void interactive_client_config_print_usage_short(const MqttSnLogger *logger);
void interactive_client_config_print_usage_long(const MqttSnLogger *logger);
void interactive_client_config_print_see_usage(const MqttSnLogger *logger);

int32_t interactive_client_config_file_process_command_callback(void *cfg, const MqttSnLogger *logger, int argc, char *argv[]);

int32_t interactive_client_config_print_updated_config(const MqttSnLogger *logger, uint8_t success);
int32_t parse_initial_interactive_client_config(interactive_client_config_container *cfg, const MqttSnLogger *cfg_logger, int argc,
                                                char *argv[]);
#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CLIENT_PUB_CONFIG_CLIENT_INTERACTIVE_CONFIG_H_

