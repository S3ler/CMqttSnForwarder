//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLogging.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFAULT_UDP
#define DEFAULT_UDP "udp"
#endif
#ifndef DEFAULT_TCP
#define DEFAULT_TCP "tcp"
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_PROTOCOL
#define DEFAULT_MQTT_SN_CLIENT_PROTOCOL DEFAULT_UDP
#endif

#ifndef CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT
#define CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#endif

#ifndef CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT
#define CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT 1000
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_BIND_PORT
#define DEFAULT_MQTT_SN_CLIENT_BIND_PORT 10000
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS "225.1.1.1"
#endif
#ifndef DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT 1883
#endif

#ifndef WITHOUT_MQTT_SN_CLIENT_NETWORK_WHITELIST_CONFIG
#define WITH_MQTT_SN_CLIENT_NETWORK_WHITELIST_CONFIG
#endif
#ifdef WITH_MQTT_SN_CLIENT_NETWORK_WHITELIST_CONFIG
#define WITH_MQTT_SN_CLIENT_NETWORK_ADD_WHITELIST_CONFIG_LENGTH 5
#endif

#ifndef WITHOUT_MQTT_SN_CLIENT_NETWORK_BLACKLIST_CONFIG
#define WITH_MQTT_SN_CLIENT_NETWORK_BLACKLIST_CONFIG
#endif
#ifdef WITH_MQTT_SN_CLIENT_NETWORK_BLACKLIST_CONFIG
#define WITH_MQTT_SN_CLIENT_NETWORK_ADD_BLACKLIST_CONFIG_LENGTH 5
#endif

typedef struct client_network_config_ {
    // default values
    char default_client_network_protocol[sizeof(DEFAULT_MQTT_SN_CLIENT_PROTOCOL)];
    char default_client_network_broadcast_address[sizeof(DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS)];
    // client network address
    char *  client_network_protocol;
    char *  client_network_bind_address;
    int32_t client_network_bind_port;
    // timeout
    int32_t client_network_send_timeout;
    int32_t client_network_receive_timeout;
#ifdef WITH_CLIENT_NETWORK_BROADCAST
    // broadcast
    char *  client_network_broadcast_protocol;
    char *  client_network_broadcast_address;
    int32_t client_network_broadcast_bind_port;
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
    // plugin
    char *client_network_plugin_path;
#endif
    // whitelist
#ifdef WITH_MQTT_SN_CLIENT_NETWORK_WHITELIST_CONFIG
    device_address addr_whitelist[WITH_MQTT_SN_CLIENT_NETWORK_ADD_WHITELIST_CONFIG_LENGTH];
    uint16_t       addr_whitelist_len;
    uint8_t        gw_clear_whitelist;
#endif
    // blacklist
#ifdef WITH_MQTT_SN_CLIENT_NETWORK_BLACKLIST_CONFIG
    device_address addr_blacklist[WITH_MQTT_SN_CLIENT_NETWORK_ADD_BLACKLIST_CONFIG_LENGTH];
    uint16_t       addr_blacklist_len;
    uint8_t        gw_clear_blacklist;
#endif
} client_network_config;

#ifndef DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_NETWORK_PROTOCOL_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_NETWORK_PROTOCOL_LENGTH 10
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH (MQTT_SN_DEVICE_ADDRESS_LENGTH * 4 + 1)
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_BROADCAST_PROTOCOL_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_BROADCAST_PROTOCOL_LENGTH 10
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_BROADCAST_ADDRESS_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_BROADCAST_ADDRESS_LENGTH (MQTT_SN_DEVICE_ADDRESS_LENGTH * 4 + 1)
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_PLUGIN_PATH_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_PLUGIN_PATH_LENGTH 255
#endif

typedef struct client_network_config_container_ {
    char client_network_protocol[DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH];
    char client_network_bind_address[DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH];
#ifdef WITH_CLIENT_NETWORK_BROADCAST
    char client_network_broadcast_protocol[DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_BROADCAST_PROTOCOL_LENGTH];
    char client_network_broadcast_address[DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_BROADCAST_ADDRESS_LENGTH];
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
    char client_network_plugin_path[DEFAULT_MQTT_SN_CLIENT_NETWORK_CONFIG_PLUGIN_PATH_LENGTH];
#endif
} client_network_config_container;

int32_t client_network_config_copy_to_buffer(client_network_config *cfg, client_network_config_container *cfg_buffer);

int32_t client_network_config_init(client_network_config *cfg);
void    client_network_config_cleanup(client_network_config *cfg);

int32_t is_client_network_config_command(const char *arg, int *i);
int32_t client_network_config_process_args(client_network_config *cfg, const MqttSnLogger *logger, int argc, char **argv);

void client_network_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_network_config_print_usage_long(const MqttSnLogger *logger);

int32_t client_network_config_file_process_command_callback(void *cfg, const MqttSnLogger *logger, int argc, char **argv);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_
