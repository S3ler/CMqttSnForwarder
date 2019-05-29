//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_
// TODO implement me


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

#ifndef DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS "224.1.1.101"
#endif

typedef struct client_network_config_ {
  // default values
  char default_client_network_protocol[sizeof(DEFAULT_MQTT_SN_CLIENT_PROTOCOL)];
  char default_client_network_broadcast_address[sizeof(DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS)];
} client_network_config;

int32_t client_network_config_init(client_network_config *cfg);
void client_network_config_cleanup(client_network_config *cfg);

int32_t is_client_network_config_command(const char* arg, int *i);
int32_t client_network_config_process_line(client_network_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char *argv[]);

void client_network_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_network_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_NETWORK_CONFIG_H_
