#ifndef CMQTTSNFORWARDER_FORWARDER_CONFIG_H
#define CMQTTSNFORWARDER_FORWARDER_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <logging/MqttSnLoggingInterface.h>

#define MQTT_SN_PROTOCOLID_V1 0x01

#ifndef DEFAULT_MQTT_SN_PROTOCOL_ID
#define DEFAULT_MQTT_SN_PROTOCOL_ID MQTT_SN_PROTOCOLID_V1
#endif

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
#ifndef MANUAL_WEBSITE
#define MANUAL_WEBSITE "N/D"
#endif

#ifdef Arduino_h
#ifndef DEFAULT_MQTT_SN_GATEWAY_HOST
#define DEFAULT_MQTT_SN_GATEWAY_HOST "arsmb.de"
#endif
#else
#ifndef DEFAULT_MQTT_SN_GATEWAY_HOST
#define DEFAULT_MQTT_SN_GATEWAY_HOST "localhost"
#endif
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_PORT
#define DEFAULT_MQTT_SN_GATEWAY_PORT 8888
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS "224.1.1.100"
#endif
#ifndef DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT 5353
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS "224.1.1.101"
#endif
#ifndef DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT 5352
#endif

#ifndef DEFAULT_UDP
#define DEFAULT_UDP "udp"
#endif
#ifndef DEFAULT_TCP
#define DEFAULT_TCP "tcp"
#endif

#define FORWARDER_CONFIG_HELP 2

typedef struct forwarder_config_ {
  // TODO change to following schema:
  // mqtt_sn_version_config - contains version, buildate and stuff
  // mqtt_sn_gateway_config
  //
  MqttSnLogger *logger;
  // TODO replace int by int32_t
  char version[sizeof(VERSION)];
  int major;
  int minor;
  uint64_t tweak;
  char build_date[sizeof(CMAKE_BUILD_TIMESTAMP)];

  int protocol_version;
  log_level_t log_lvl;

  // mqtt-sn gateway
  char localhost[sizeof(DEFAULT_MQTT_SN_GATEWAY_HOST)];
  char *mqtt_sn_gateway_host;
  int mqtt_sn_gateway_port;

  // gateway network config
  char udp[sizeof(DEFAULT_UDP)];
  char *gateway_network_protocol;
  char *gateway_network_bind_address;
  int gateway_network_bind_port;
  char gateway_network_default_broadcast_address[sizeof(DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS)];
  char *gateway_network_broadcast_protocol;
  char *gateway_network_broadcast_address;
  int gateway_network_broadcast_bind_port;
  int gateway_network_send_timeout;
  int gateway_network_receive_timeout;
#ifdef WITH_LINUX_PLUGIN_NETWORK
  char *gateway_network_plugin_path;
#endif

  // client network config
  char tcp[sizeof(DEFAULT_TCP)];
  char *client_network_protocol;
  char *client_network_bind_address;
  int client_network_bind_port;
  char client_network_default_broadcast_address[sizeof(DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS)];
  char *client_network_broadcast_protocol;
  char *client_network_broadcast_address;
  int client_network_broadcast_bind_port;
  int client_network_send_timeout;
  int client_network_receive_timeout;
#ifdef WITH_LINUX_PLUGIN_NETWORK
  char *client_network_plugin_path;
#endif

} forwarder_config;

int forwarder_config_init(forwarder_config *fcfg, MqttSnLogger *logger);

void forwarder_config_cleanup(forwarder_config *cfcg);

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]);

int process_forwarder_config_str(forwarder_config *fcfg, char *line, size_t len, const char *argv_o);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_CONFIG_H
