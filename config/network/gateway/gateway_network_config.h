//
// Created by SomeDude on 27.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_GATEWAY_NETWORK_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_GATEWAY_NETWORK_CONFIG_H_

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

#ifndef DEFAULT_MQTT_SN_GATEWAY_PROTOCOL
#define DEFAULT_MQTT_SN_GATEWAY_PROTOCOL DEFAULT_UDP
#endif

#ifndef GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT
#define GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#endif

#ifndef GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT
#define GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT 1000
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_BIND_PORT
#define DEFAULT_MQTT_SN_GATEWAY_BIND_PORT 9999
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS "225.1.1.1"
#endif
#ifndef DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT 1883
#endif

#ifndef WITHOUT_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
#define WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
#define WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_WHITELIST_CONFIG_LENGTH 5
#define WITH_MQTT_SN_GATEWAY_NETWORK_ADD_WHITELIST_CONFIG_LENGTH 5
#endif

#ifndef WITHOUT_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
#define WITH_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
#define WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_BLACKLIST_CONFIG_LENGTH 5
#define WITH_MQTT_SN_GATEWAY_NETWORK_ADD_BLACKLIST_CONFIG_LENGTH 5
#endif

typedef struct linux_gateway_network_config_ {
    // default values
    char default_gateway_network_protocol[sizeof(DEFAULT_MQTT_SN_GATEWAY_PROTOCOL)];
    char default_gateway_network_broadcast_address[sizeof(DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS)];
    // gateway network address
    char *  gateway_network_protocol;
    char *  gateway_network_bind_address;
    int32_t gateway_network_bind_port;
    // timeout
    int32_t gateway_network_send_timeout;
    int32_t gateway_network_receive_timeout;
    // broadcast
#ifdef WITH_GATEWAY_NETWORK_BROADCAST
    char *  gateway_network_broadcast_protocol;
    char *  gateway_network_broadcast_address;
    int32_t gateway_network_broadcast_bind_port;
#endif
    // plugin
#ifdef WITH_LINUX_PLUGIN_NETWORK
    char *gateway_network_plugin_path;
#endif
    // whitelist
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    uint8_t  gw_id_whitelist[WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_WHITELIST_CONFIG_LENGTH];
    uint16_t gw_id_whitelist_len;

    device_address gw_addr_whitelist[WITH_MQTT_SN_GATEWAY_NETWORK_ADD_WHITELIST_CONFIG_LENGTH];
    uint16_t       gw_addr_whitelist_len;

    uint8_t gw_clear_whitelist;
#endif
    // blacklist
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
    uint8_t  gw_id_blacklist[WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_BLACKLIST_CONFIG_LENGTH];
    uint16_t gw_id_blacklist_len;

    device_address gw_addr_blacklist[WITH_MQTT_SN_GATEWAY_NETWORK_ADD_BLACKLIST_CONFIG_LENGTH];
    uint16_t       gw_addr_blacklist_len;

    uint8_t gw_clear_blacklist;
#endif
} gateway_network_config;

#ifndef DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_PROTOCOL_LENGTH
#define DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_PROTOCOL_LENGTH 10
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH
#define DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH (MQTT_SN_DEVICE_ADDRESS_LENGTH * 4 + 1)
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_PROTOCOL_LENGTH
#define DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_PROTOCOL_LENGTH 10
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_ADDRESS_LENGTH
#define DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_ADDRESS_LENGTH (MQTT_SN_DEVICE_ADDRESS_LENGTH * 4 + 1)
#endif


#ifndef DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_PLUGIN_PATH_LENGTH
#define DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_PLUGIN_PATH_LENGTH 255
#endif

typedef struct linux_gateway_network_config_container_ {
    // gateway network address
    char gateway_network_protocol[DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_PROTOCOL_LENGTH];
    char gateway_network_bind_address[DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH];

#ifdef WITH_GATEWAY_NETWORK_BROADCAST
    // broadcast
    char gateway_network_broadcast_protocol[DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_PROTOCOL_LENGTH];
    char gateway_network_broadcast_address[DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_ADDRESS_LENGTH];
#endif
    // plugin
#ifdef WITH_LINUX_PLUGIN_NETWORK
    char gateway_network_plugin_path[DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_PLUGIN_PATH_LENGTH];
#endif
} gateway_network_config_container;

int32_t gateway_network_config_copy_to_buffer(gateway_network_config *cfg, gateway_network_config_container *cfg_buffer);

int32_t gateway_network_config_init(gateway_network_config *cfg);
void    gateway_network_config_cleanup(gateway_network_config *cfg);

int32_t is_gateway_network_config_command(const char *arg, int *i);
int32_t gateway_network_config_process_args(gateway_network_config *cfg, const MqttSnLogger *logger, int argc, char **argv);

void gateway_network_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void gateway_network_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CONFIG_GATEWAY_NETWORK_CONFIG_H_
