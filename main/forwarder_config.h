#ifndef CMQTTSNFORWARDER_FORWARDER_CONFIG_H
#define CMQTTSNFORWARDER_FORWARDER_CONFIG_H

#include <stdio.h>
#include <stdbool.h>

void print_usage(void);

#define MQTT_SN_PROTOCOL_V1 0x01

#define CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#define CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT 1000

#define GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#define GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT 1000

typedef struct forwarder_config_ {
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

  int protocol_version;

  // logging config
  int verbose;
  int debug;
  int quiet;

  int gateway_network_send_timeout;
  int gateway_network_receive_timeout;
  int client_network_send_timeout;
  int client_network_receive_timeout;
} forwarder_config;

void forwarder_config_init(forwarder_config *fcfg);

void forwarder_config_cleanup(forwarder_config *cfcg);

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]);

#endif //CMQTTSNFORWARDER_FORWARDER_CONFIG_H
