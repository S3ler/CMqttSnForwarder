#ifndef CMQTTSNFORWARDER_FORWARDER_CONFIG_H
#define CMQTTSNFORWARDER_FORWARDER_CONFIG_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <forwarder/MqttSnForwarderLogging.h>


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
#ifndef MANUAL_WEBSITE
#define MANUAL_WEBSITE "N/D"
#endif
#ifndef DEFAULT_LOCALHOST
#define DEFAULT_LOCALHOST "localhost"
#endif
#ifndef DEFAULT_UDP
#define DEFAULT_UDP "udp"
#endif
#ifndef DEFAULT_TCP
#define DEFAULT_TCP "tcp"
#endif


typedef struct forwarder_config_ {
  MqttSnLogger strut_logger;
  MqttSnLogger *logger;

  char version[sizeof(VERSION)];
  int major;
  int minor;
  uint64_t tweak;
  char build_date[sizeof(CMAKE_BUILD_TIMESTAMP)];

  int protocol_version;
  int log_lvl;

  // mqtt-sn gateway
  char localhost[sizeof(DEFAULT_LOCALHOST)];
  char *mqtt_sn_gateway_host;
  int mqtt_sn_gateway_port;
  // gateway network config
  char udp[sizeof(DEFAULT_UDP)];
  char *gateway_network_protocol;
  char *gateway_network_bind_address;
  int gateway_network_bind_port;
#ifdef WITH_PLUGIN
  char *gateway_network_plugin_path;
#endif

  // client network config
  char tcp[sizeof(DEFAULT_TCP)];
  char *client_network_protocol;
  char *client_network_bind_address;
  int client_network_bind_port;
#ifdef WITH_PLUGIN
  char *client_network_plugin_path;
#endif

  int gateway_network_send_timeout;
  int gateway_network_receive_timeout;
  int client_network_send_timeout;
  int client_network_receive_timeout;


} forwarder_config;

int print_usage(const MqttSnLogger *logger);

int forwarder_config_init(forwarder_config *fcfg);

void forwarder_config_cleanup(forwarder_config *cfcg);

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]);

// parsing
int get_arc_line_len(char *line);

void parse_argv_line(char **argv_line, char **argv, char *line_copy, int argc_line);

// logging
int print_invalid_port_given(const MqttSnLogger *logger, long invalid_port);
int print_invalid_timeout_given(const MqttSnLogger *logger, long timeout);
int log_could_not_read_config_file(const MqttSnLogger *logger, char *strerror);
int log_argument_value_not_specified(const MqttSnLogger *logger, const char *argument, const char *argument_name);
int log_unsupported_url_scheme(const MqttSnLogger *logger);
int log_invalid_protocol_version_given(const MqttSnLogger *logger);
#endif //CMQTTSNFORWARDER_FORWARDER_CONFIG_H
