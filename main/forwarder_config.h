#ifndef CMQTTSNFORWARDER_FORWARDER_CONFIG_H
#define CMQTTSNFORWARDER_FORWARDER_CONFIG_H

#include <stdio.h>
#include <stdbool.h>

void print_usage(void);

#define MQTT_SN_PROTOCOL_V1 0x01

typedef struct forwarder_config_ {
  // mqtt-sn gateway
  char *mqtt_sn_gateway_host;
  int mqtt_sn_gateway_port;
  // gateway network config
  char *gateway_network_bind_address;
  int gateway_network_bind_port;
  // client network config
  char *client_network_bind_address;
  int client_network_bind_port;

  int protocol_version;

  // logging config
  int verbose;
  int debug;
  int quiet;
} forwarder_config;

void forwarder_config_init(forwarder_config *fcfg);

void forwarder_config_cleanup(forwarder_config *cfcg);

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]);

#endif //CMQTTSNFORWARDER_FORWARDER_CONFIG_H
