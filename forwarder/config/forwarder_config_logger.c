//
// Created by SomeDude on 12.05.2019.
//

#include <platform/platform_compatibility.h>
#include "forwarder_config_logger.h"
#include "forwarder_config.h"

int print_argc_argv(const MqttSnLogger *logger, int argc, char **argv) {
  log_str(logger, PSTR("argc: "));
  log_uint64(logger, argc);
  log_str(logger, PSTR(" argv: "));
  for (uint16_t i = 0; i < argc; i++) {
    log_str(logger, argv[i]);
    if (i + 1 < argc) {
      log_str(logger, PSTR(", "));
    }
  }
  log_flush(logger);
  return log_status(logger);
}

int print_usage(const MqttSnLogger *logger) {

  // forwarder description, version an build date
  log_str(logger, "cmqttsnforwarder is a simple mqtt-sn forwarder that will forward mqtt-sn messages\n");
  log_str(logger, "                 from a client network over a gateway network to a mqtt-sn gateway.\n");
  log_str(logger, "cmqttsnforwarder version ");
  log_str(logger, VERSION);
  log_str(logger, " is ");
  log_uint64(logger, MAJOR);
  log_str(logger, ".");
  log_uint64(logger, MINOR);
  log_str(logger, ".");
  log_uint64(logger, TWEAK);
  log_str(logger, " (build date ");
  log_str(logger, CMAKE_BUILD_TIMESTAMP);
  log_str(logger, ").");
  log_flush(logger);
  log_flush(logger);

  // usage and short command line descriptions
  log_str(logger, "Usage: cmqttsnforwarder {[[-h mqtt_sn_gateway_host] [-p mqtt_sn_gateway_port]] | -L URL}\n");

  log_str(logger, "                        {[[-gP gateway_network_protocol] [-gA gateway_network_bind_address]"
                  " [-gp gateway_network_bind_port]] | -gL URL}\n");
  log_str(logger, "                        {[[-cP client_network_protocol] [-cA client_network_bind_address]"
                  " [-cp client_network_bind_port]] | -cL URL}\n");

#if defined(WITH_TCP_BROADCAST) || defined(WITH_UDP_BROADCAST)
  log_str(logger,
          "                        {[[-gbP gateway_network_broadcast_protocol] [-gbA gateway_network_broadcast_address]"
          " [-gbp gateway_network_broadcast_bind_port]] | -gbL URL}\n");
  log_str(logger,
          "                        {[[-cbP client_network_broadcast_protocol] [-cbA client_network_broadcast_address]"
          " [-cbp client_network_broadcast_bind_port]] | -cbL URL}\n");
#endif

  log_str(logger, "                        {[[-gst gateway_send_timeout] [-grt gateway_receive_timeout]]"
                  " | --gt gateway_network_timeout}\n");
  log_str(logger, "                        {[[-cst client_send_timeout] [-crt client_receive_timeout]]"
                  " | --ct client_network_timeout}\n");

#ifdef WITH_LINUX_PLUGIN_NETWORK
  log_str(logger, "                        [-gnp gateway_network_plugin]\n");
  log_str(logger, "                        [-cnp client_network_plugin]\n");
#endif

#if defined(WITH_LOGGING)
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger, "                        [[-q quiet] | [-d default] | [-v verbose] | [-db debug]]\n");
#else
  log_str(logger,"                        [[-q quiet] | [-d default] | [-v verbose]]\n");
#endif
#endif

#if defined(WITH_MQTT_SN_FORWADER_CONFIG_FILE)
  log_str(logger, "                        [-c --config-file]\n");
#endif

  log_str(logger, "       cmqttsnforwarder --help\n\n");


  // long command line description
  log_str(logger, " -h : mqtt-sn gateway host to connect to.\n");
  log_str(logger, "       Defaults to ");
  log_str(logger, DEFAULT_MQTT_SN_GATEWAY_HOST);
  log_str(logger, ".\n");

  log_str(logger, " -p : mqtt-sn gateway network port to connect to.\n");
  log_str(logger, "      Defaults to 8888 for plain MQTT-SN.\n");

  log_str(logger, " -L : specify hostname, port as a URL in the form:\n");
  log_str(logger, "      mqtt-sn(s)://host[:port]\n");
  log_str(logger, "      mqtt-sn(s) uses the same protocol as the gateway network.\n");

  log_str(logger, " -gP : specify the protocol of the gateway network to use.\n");
#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK) && defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
  log_str(logger, "       Can be udp, tcp. Defaults to udp.\n");
#elif defined(WITH_LINUX_UDP_GATEWAY_NETWORK)
  log_str(logger, "       Can be udp. Defaults to udp.\n");
#elif defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
  log_str(logger, "       Can be tcp. Defaults to tcp.\n");
#endif
  log_str(logger, " -gA : bind the gateway network to the outgoing socket to this host/ip address.\n");
  log_str(logger, "       Use to control which interface the network communicates over.\n");
  log_str(logger, " -gp : listening on the specific gateway network port. Defaults to 9999.\n");
  log_str(logger, " -gL : specify protocol, broadcast address, port as a URL in the form: protocol://address[:port]\n");

  log_str(logger, " -cP : specify the protocol of the client network to use.\n");
#if defined(WITH_LINUX_UDP_CLIENT_NETWORK) && defined(WITH_LINUX_TCP_CLIENT_NETWORK)
  log_str(logger, "       Can be udp, tcp. Defaults to udp.\n");
#elif defined(WITH_LINUX_UDP_CLIENT_NETWORK)
  log_str(logger, "       Can be udp. Defaults to udp.\n");
#elif defined(WITH_LINUX_TCP_CLIENT_NETWORK)
  log_str(logger, "       Can be tcp. Defaults to tcp.\n");
#endif
  log_str(logger, " -cA : bind the client network to the outgoing socket to this host/ip address.\n");
  log_str(logger, "       Use to control which interface the network communicates over.\n");
  log_str(logger, " -cp : listening on the specific client network port. Defaults to 7777.\n");
  log_str(logger, " -cL : specify protocol, broadcast address, port as a URL in the form: protocol://address[:port]\n");

#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST) || defined(WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST)
  log_str(logger, " -gbP : specify the protocol of the gateway network broadcast to use.\n");
  log_str(logger, "        Can be udp. Defaults to udp.\n");
  log_str(logger, " -gbA : specify the gateway network broadcast address.\n");
  log_str(logger, "        Note: the gateway network broadcast binds to the gateway network bind interface.\n");
  log_str(logger, "        Defaults to ");
  log_str(logger, DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS);
  log_str(logger, ".\n");
  log_str(logger, " -gbp : listening on the specific gateway network broadcast port. Defaults to ");
  log_int32(logger, DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT);
  log_str(logger, ".\n");
  log_str(logger, " -gbL : specify protocol, address, port as a URL in the form: protocol://address[:port]\n");
#endif

#if defined(WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST) || defined(WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST)
  log_str(logger, " -cbP : specify the protocol of the client network broadcast to use.\n");
  log_str(logger, "        Can be udp. Defaults to udp.\n");
  log_str(logger, " -cbA : specify the client network broadcast address.\n");
  log_str(logger, "        Defaults to ");
  log_str(logger, DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS);
  log_str(logger, ".\n");
  log_str(logger, "        Note: the client network broadcast binds to the client network bind interface.\n");
  log_str(logger, " -cbp : listening on the specific client network broadcast port. Defaults to ");
  log_int32(logger, DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT);
  log_str(logger, ".\n");
  log_str(logger, " -cbL : specify protocol, address, port as a URL in the form: protocol://address[:port]\n");
#endif

  log_str(logger, " -gst : specify the gateway network send timeout in ms.\n");
  log_str(logger, "       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger, " -grt : specify the gateway network receive timeout in ms.\n");
  log_str(logger, "       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger, " -gt  : specify the gateway network send and receive timeout in ms. Defaults to 1000 ms.\n");
  log_str(logger, "       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger, " -cst : specify the client network send timeout in ms.\n");
  log_str(logger, "       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger, " -crt : specify the client network receive timeout in ms.\n");
  log_str(logger, "       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger, " -ct  : specify the client network send and receive timeout in ms. Defaults to 1000 ms.\n");
  log_str(logger, "       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");

  log_str(logger, " -V : specify the version of the MQTT-SN protocol to use.\n");
  log_str(logger, "      Can be mqttsnv1. Defaults to mqttsnv1.\n");
#ifdef WITH_LINUX_PLUGIN_NETWORK
  log_str(logger, " -gnp : path to the gateway network plugin.\n");
  log_str(logger, "       The gateway network protocol must match the short protocol name gained from the plugin.\n");
  log_str(logger,
          "       A gateway bind address is saved to a device address and given to the gateway network plugin.\n");
  log_str(logger, "       A port overwrites the two last bytes of the gateway network bind address. "
                  "You can set gateway port to -1 if you want to overwrite this behaviour\n");
  log_str(logger, " -cnp : absolute path to the client network plugin.\n");
  log_str(logger, "       The client network protocol must match the short protocol name gained from the plugin.\n");
  log_str(logger,
          "       A client bind address is saved to a device address and given to the gateway network plugin.\n");
  log_str(logger, "       A client port overwrites the two last bytes of the gateway network bind address. "
                  "You can set gateway port to -1 if you want to overwrite this behaviour\n");
#endif
#if defined(WITH_LOGGING)
  log_str(logger, " -q : specify quiet logging. Don't print any log messages.\n");
  log_str(logger, " -d : specify default logging. Print network status changes, and the mqtt-sn messages: "
                  "PUBLISH, CONNECT, CONNACK, DISCONNECT.\n");
  log_str(logger, " -v : specify verbose logging. Print all default logging and all MQTT-SN messages.\n");
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger,
          " -db : specify debug logging. Print all mqtt-sn messages including payload and internal information.\n");
#endif
#endif
#if defined(WITH_MQTT_SN_FORWADER_CONFIG_FILE)
  log_str(logger, " -c : specify the config file.\n");
#endif
  log_str(logger, " --help : display this message.\n");
  log_flush(logger);
  log_str(logger, "See ");
  log_str(logger, MANUAL_WEBSITE);
  log_str(logger, " for more information.\n");

  log_flush(logger);
  return log_status(logger);
}

int print_fcfg_invalid_port_given(const MqttSnLogger *logger, long invalid_port) {
  log_str(logger, "Error: Invalid port given: ");
  log_uint16(logger, invalid_port);
  log_flush(logger);
  return log_status(logger);
}

int print_invalid_timeout_given(const MqttSnLogger *logger, long timeout) {
  log_str(logger, "Error: Invalid port given: ");
  log_uint16(logger, timeout);
  log_flush(logger);
  return log_status(logger);
}

int log_could_not_read_config_file(const MqttSnLogger *logger, char *strerror) {
  log_str(logger, "Error: Could not read config file: ");
  log_str(logger, strerror);
  log_str(logger, ".");
  log_flush(logger);
  return log_status(logger);
}

int log_argument_value_not_specified(const MqttSnLogger *logger, const char *argument, const char *argument_name) {
  log_str(logger, "Error: ");
  log_str(logger, argument);
  log_str(logger, " argument given but no ");
  log_str(logger, argument_name);
  log_str(logger, " specified.\n");
  log_flush(logger);
  return log_status(logger);
}

int log_unsupported_url_scheme(const MqttSnLogger *logger) {
  log_str(logger, "Error: unsupported URL scheme.");
  log_flush(logger);
  return log_status(logger);
}

int log_invalid_protocol_version_given(const MqttSnLogger *logger) {
  log_str(logger, "Error: Invalid protocol version given.");
  log_flush(logger);
  return log_status(logger);
}

int log_unknown_option(const MqttSnLogger *logger, const char *unknown_option) {
  log_str(logger, PSTR("Error: Unknown option "));
  log_str(logger, unknown_option);
  log_str(logger, ".");
  log_flush(logger);
  return log_status(logger);
}
