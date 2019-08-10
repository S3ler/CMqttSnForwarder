//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

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
#define DEFAULT_MQTT_SN_CLIENT_BIND_PORT 8888
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS "225.1.1.1" // TODO old "224.1.1.101"
#endif
#ifndef DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT 1883 // TODO old 5352
#endif

typedef struct client_network_config_ {
  // default values
  char default_client_network_protocol[sizeof(DEFAULT_MQTT_SN_CLIENT_PROTOCOL)];
  char default_client_network_broadcast_address[sizeof(DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS)];
  // client network address
  char *client_network_protocol;
  char *client_network_bind_address;
  int32_t client_network_bind_port;
  // timeout
  int32_t client_network_send_timeout;
  int32_t client_network_receive_timeout;
#ifdef WITH_CLIENT_NETWORK_BROADCAST
  // broadcast
  char *client_network_broadcast_protocol;
  char *client_network_broadcast_address;
  int32_t client_network_broadcast_bind_port;
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
  // plugin
  char *client_network_plugin_path;
#endif
} client_network_config;

int32_t client_network_config_init(client_network_config *cfg);
void client_network_config_cleanup(client_network_config *cfg);

int32_t is_client_network_config_command(const char *arg, int *i);
int32_t client_network_config_process_args(client_network_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char **argv);

void client_network_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_network_config_print_usage_long(const MqttSnLogger *logger);

int32_t client_network_config_file_process_command_callback(void *cfg,
                                                            const MqttSnLogger *logger,
                                                            int argc,
                                                            char **argv);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_
