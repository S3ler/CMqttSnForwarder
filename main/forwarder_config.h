#ifndef CMQTTSNFORWARDER_FORWARDER_CONFIG_H
#define CMQTTSNFORWARDER_FORWARDER_CONFIG_H

#include <stdio.h>
#include <stdbool.h>
#include "MqttSnForwarderLogging.h"

void print_usage(void);

#define MQTT_SN_PROTOCOL_V1 0x01

#define CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#define CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT 1000

#define GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#define GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT 1000

#ifndef VERSION
#define VERSION "N/D"
#endif
#ifndef MAJOR
#define MAJOR 0
#endif
#ifndef MINOR
#define MINOR 0
#endif
#ifndef TWEAK
#define TWEAK 0
#endif
#ifndef CMAKE_BUILD_TIMESTAMP
#define CMAKE_BUILD_TIMESTAMP "N/D"
#endif

typedef struct forwarder_config_ {
  char *version;
  int major;
  int minor;
  uint64_t tweak;
  char *build_date;

  int protocol_version;
  int log_lvl;

  // mqtt-sn gateway
  char *mqtt_sn_gateway_host;
  int mqtt_sn_gateway_port;
  // gateway network config
  char *gateway_network_protocol;
  char *gateway_network_bind_address;
  int gateway_network_bind_port;
  char *gateway_network_plugin_path;
  // client network config
  char *client_network_protocol;
  char *client_network_bind_address;
  int client_network_bind_port;
  char *client_network_plugin_path;


  int gateway_network_send_timeout;
  int gateway_network_receive_timeout;
  int client_network_send_timeout;
  int client_network_receive_timeout;

} forwarder_config;

void forwarder_config_init(forwarder_config *fcfg);

void forwarder_config_cleanup(forwarder_config *cfcg);

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]);

#endif //CMQTTSNFORWARDER_FORWARDER_CONFIG_H
