//
// Created by SomeDude on 27.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_GATEWAY_NETWORK_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_GATEWAY_NETWORK_CONFIG_H_

#include <stdint.h>
#include <stddef.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
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
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS "224.1.1.100"
#endif
#ifndef DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT 5353
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

typedef struct linux_gateway_network_config_ {
  // default values
  char default_gateway_network_protocol[sizeof(DEFAULT_MQTT_SN_GATEWAY_PROTOCOL)];
  char default_gateway_network_broadcast_address[sizeof(DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS)];
  // gateway network address
  char *gateway_network_protocol;
  char *gateway_network_bind_address;
  int32_t gateway_network_bind_port;
  // timeout
  int32_t gateway_network_send_timeout;
  int32_t gateway_network_receive_timeout;
  // broadcast
#ifdef WITH_GATEWAY_NETWORK_BROADCAST
  char *gateway_network_broadcast_protocol;
  char *gateway_network_broadcast_address;
  int32_t gateway_network_broadcast_bind_port;
#endif
  // plugin
#ifdef WITH_LINUX_PLUGIN_NETWORK
  char *gateway_network_plugin_path;
#endif
} gateway_network_config;

int32_t gateway_network_config_init(gateway_network_config *lgncfg);

void gateway_network_config_cleanup(gateway_network_config *lgncfg);
int32_t is_gateway_network_config_command(const char *arg, int *i);
int32_t gateway_network_config_process_args(gateway_network_config *lgncfg,
                                            const MqttSnLogger *logger,
                                            int argc,
                                            char **argv);
void gateway_network_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void gateway_network_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_GATEWAY_NETWORK_CONFIG_H_
