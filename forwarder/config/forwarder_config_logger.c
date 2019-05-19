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
  log_str(logger, PSTR("cmqttsnforwarder is a simple mqtt-sn forwarder that will forward mqtt-sn messages\n"));
  log_str(logger, PSTR("                 from a client network over a gateway network to a mqtt-sn gateway.\n"));
  log_str(logger, PSTR("cmqttsnforwarder version "));
  log_str(logger, VERSION);
  log_str(logger, PSTR(" is "));
  log_uint64(logger, MAJOR);
  log_str(logger, PSTR("."));
  log_uint64(logger, MINOR);
  log_str(logger, PSTR("."));
  log_uint64(logger, TWEAK);
  log_str(logger, PSTR(" (build date "));
  log_str(logger, CMAKE_BUILD_TIMESTAMP);
  log_str(logger, PSTR(")."));
  log_flush(logger);
  log_flush(logger);

  // usage and short command line descriptions
  log_str(logger, PSTR("Usage: cmqttsnforwarder {[[-h mqtt_sn_gateway_host] [-p mqtt_sn_gateway_port]] | -L URL}\n"));

  log_str(logger, PSTR("                        {[[-gP gateway_network_protocol] [-gA gateway_network_bind_address]"
                       " [-gp gateway_network_bind_port]] | -gL URL}\n"));
  log_str(logger, PSTR("                        {[[-cP client_network_protocol] [-cA client_network_bind_address]"
                       " [-cp client_network_bind_port]] | -cL URL}\n"));

#if defined(WITH_TCP_BROADCAST) || defined(WITH_UDP_BROADCAST)
  log_str(logger,
          PSTR(
              "                        {[[-gbP gateway_network_broadcast_protocol] [-gbA gateway_network_broadcast_address]"
              " [-gbp gateway_network_broadcast_bind_port]] | -gbL URL}\n"));
  log_str(logger,
          PSTR(
              "                        {[[-cbP client_network_broadcast_protocol] [-cbA client_network_broadcast_address]"
              " [-cbp client_network_broadcast_bind_port]] | -cbL URL}\n"));
#endif

  log_str(logger, PSTR("                        {[[-gst gateway_send_timeout] [-grt gateway_receive_timeout]]"
                       " | --gt gateway_network_timeout}\n"));
  log_str(logger, PSTR("                        {[[-cst client_send_timeout] [-crt client_receive_timeout]]"
                       " | --ct client_network_timeout}\n"));

#ifdef WITH_LINUX_PLUGIN_NETWORK
  log_str(logger, PSTR("                        [-gnp gateway_network_plugin]\n"));
  log_str(logger, PSTR("                        [-cnp client_network_plugin]\n"));
#endif

#if defined(WITH_LOGGING)
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger, PSTR("                        [[-q quiet] | [-d default] | [-v verbose] | [-db debug]]\n"));
#else
  log_str(logger, PSTR("                        [[-q quiet] | [-d default] | [-v verbose]]\n"));
#endif
#endif

#if defined(WITH_MQTT_SN_FORWADER_CONFIG_FILE)
  log_str(logger, PSTR("                        [-c --config-file]\n"));
#endif

  log_str(logger, PSTR("       cmqttsnforwarder --help\n\n"));


  // long command line description
  log_str(logger, PSTR(" -h : mqtt-sn gateway host to connect to.\n"));
  log_str(logger, PSTR("       Defaults to "));
  log_str(logger, DEFAULT_MQTT_SN_GATEWAY_HOST);
  log_str(logger, PSTR(".\n"));

  log_str(logger, PSTR(" -p : mqtt-sn gateway network port to connect to.\n"));
  log_str(logger, PSTR("      Defaults to 8888 for plain MQTT-SN.\n"));

  log_str(logger, PSTR(" -L : specify hostname, port as a URL in the form:\n"));
  log_str(logger, PSTR("      mqtt-sn(s)://host[:port]\n"));
  log_str(logger, PSTR("      mqtt-sn(s) uses the same protocol as the gateway network.\n"));

  log_str(logger, PSTR(" -gP : specify the protocol of the gateway network to use.\n"));
#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK) && defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
  log_str(logger, PSTR("       Can be udp, tcp. Defaults to udp.\n"));
#elif defined(WITH_LINUX_UDP_GATEWAY_NETWORK)
  log_str(logger,PSTR("       Can be udp. Defaults to udp.\n"));
#elif defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
  log_str(logger,PSTR("       Can be tcp. Defaults to tcp.\n"));
#endif
  log_str(logger, PSTR(" -gA : bind the gateway network to the outgoing socket to this host/ip address.\n"));
  log_str(logger,
          PSTR(
              "       Use to control which interface to communicates over or to set the address for the gateway network.\n"));
  log_str(logger, PSTR(" -gp : listening on the specific gateway network port. Defaults to 9999.\n"));
  log_str(logger,
          PSTR(" -gL : specify protocol, broadcast address, port as a URL in the form: protocol://address[:port]\n"));

  log_str(logger, PSTR(" -cP : specify the protocol of the client network to use.\n"));
#if defined(WITH_LINUX_UDP_CLIENT_NETWORK) && defined(WITH_LINUX_TCP_CLIENT_NETWORK)
  log_str(logger, PSTR("       Can be udp, tcp. Defaults to udp.\n"));
#elif defined(WITH_LINUX_UDP_CLIENT_NETWORK)
  log_str(logger,PSTR("       Can be udp. Defaults to udp.\n"));
#elif defined(WITH_LINUX_TCP_CLIENT_NETWORK)
  log_str(logger,PSTR("       Can be tcp. Defaults to tcp.\n"));
#endif
  log_str(logger, PSTR(" -cA : bind the client network to the outgoing socket to this host/ip address.\n"));
  log_str(logger,
          PSTR(
              "       Use to control which interface to communicates over or to set the address for the client network.\n"));
  log_str(logger, PSTR(" -cp : listening on the specific client network port. Defaults to 7777.\n"));
  log_str(logger,
          PSTR(" -cL : specify protocol, broadcast address, port as a URL in the form: protocol://address[:port]\n"));

#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST) || defined(WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST)
  log_str(logger, PSTR(" -gbP : specify the protocol of the gateway network broadcast to use.\n"));
  log_str(logger, PSTR("        Can be udp. Defaults to udp.\n"));
  log_str(logger, PSTR(" -gbA : specify the gateway network broadcast address.\n"));
  log_str(logger, PSTR("        Note: the gateway network broadcast binds to the gateway network bind interface.\n"));
  log_str(logger, PSTR("        Defaults to "));
  log_str(logger, DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" -gbp : listening on the specific gateway network broadcast port. Defaults to "));
  log_int32(logger, DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" -gbL : specify protocol, address, port as a URL in the form: protocol://address[:port]\n"));
#endif

#if defined(WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST) || defined(WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST)
  log_str(logger, PSTR(" -cbP : specify the protocol of the client network broadcast to use.\n"));
  log_str(logger, PSTR("        Can be udp. Defaults to udp.\n"));
  log_str(logger, PSTR(" -cbA : specify the client network broadcast address.\n"));
  log_str(logger, PSTR("        Defaults to "));
  log_str(logger, DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR("        Note: the client network broadcast binds to the client network bind interface.\n"));
  log_str(logger, PSTR(" -cbp : listening on the specific client network broadcast port. Defaults to "));
  log_int32(logger, DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" -cbL : specify protocol, address, port as a URL in the form: protocol://address[:port]\n"));
#endif

  log_str(logger, PSTR(" -gst : specify the gateway network send timeout in ms.\n"));
  log_str(logger,
          PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n"));
  log_str(logger, PSTR(" -grt : specify the gateway network receive timeout in ms.\n"));
  log_str(logger,
          PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n"));
  log_str(logger, PSTR(" -gt  : specify the gateway network send and receive timeout in ms. Defaults to 1000 ms.\n"));
  log_str(logger,
          PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n"));
  log_str(logger, PSTR(" -cst : specify the client network send timeout in ms.\n"));
  log_str(logger,
          PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n"));
  log_str(logger, PSTR(" -crt : specify the client network receive timeout in ms.\n"));
  log_str(logger,
          PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n"));
  log_str(logger, PSTR(" -ct  : specify the client network send and receive timeout in ms. Defaults to 1000 ms.\n"));
  log_str(logger,
          PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n"));

  log_str(logger, PSTR(" -V : specify the version of the MQTT-SN protocol to use.\n"));
  log_str(logger, PSTR("      Can be mqttsnv1. Defaults to mqttsnv1.\n"));
#ifdef WITH_LINUX_PLUGIN_NETWORK
  log_str(logger, PSTR(" -gnp : path to the gateway network plugin.\n"));
  log_str(logger,
          PSTR("       The gateway network protocol must match the short protocol name gained from the plugin.\n"));
  log_str(logger,
          PSTR("       A gateway bind address is saved to a device address and given to the gateway network plugin.\n"));
  log_str(logger, PSTR("       A port overwrites the two last bytes of the gateway network bind address. "
                       "You can set gateway port to -1 if you want to overwrite this behaviour\n"));
  log_str(logger, PSTR(" -cnp : absolute path to the client network plugin.\n"));
  log_str(logger,
          PSTR("       The client network protocol must match the short protocol name gained from the plugin.\n"));
  log_str(logger,
          PSTR("       A client bind address is saved to a device address and given to the gateway network plugin.\n"));
  log_str(logger, PSTR("       A client port overwrites the two last bytes of the gateway network bind address. "
                       "You can set gateway port to -1 if you want to overwrite this behaviour\n"));
#endif
#if defined(WITH_LOGGING)
  log_str(logger, PSTR(" -q : specify quiet logging. Don't print any log messages.\n"));
  log_str(logger, PSTR(" -d : specify default logging. Print network status changes, and the mqtt-sn messages: "
                       "PUBLISH, CONNECT, CONNACK, DISCONNECT.\n"));
  log_str(logger, PSTR(" -v : specify verbose logging. Print all default logging and all MQTT-SN messages.\n"));
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger,
          PSTR(" -db : specify debug logging. Print all mqtt-sn messages including payload and internal information.\n"));
#endif
#endif
#if defined(WITH_MQTT_SN_FORWADER_CONFIG_FILE)
  log_str(logger, PSTR(" -c : specify the config file.\n"));
#endif
  log_str(logger, PSTR(" --help : display this message.\n"));
  log_flush(logger);
  log_str(logger, PSTR("See "));
  log_str(logger, MANUAL_WEBSITE);
  log_str(logger, PSTR(" for more information.\n"));

  log_flush(logger);
  return log_status(logger);
}

int print_fcfg_invalid_port_given(const MqttSnLogger *logger, long invalid_port) {
  log_str(logger, PSTR("Error: Invalid port given: "));
  log_uint16(logger, invalid_port);
  log_flush(logger);
  return log_status(logger);
}

int print_invalid_timeout_given(const MqttSnLogger *logger, long timeout) {
  log_str(logger, PSTR("Error: Invalid port given: "));
  log_uint16(logger, timeout);
  log_flush(logger);
  return log_status(logger);
}

int log_could_not_read_config_file(const MqttSnLogger *logger, char *strerror) {
  log_str(logger, PSTR("Error: Could not read config file: "));
  log_str(logger, strerror);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}

int log_argument_value_not_specified(const MqttSnLogger *logger, const char *argument, const char *argument_name) {
  log_str(logger, PSTR("Error: "));
  log_str(logger, argument);
  log_str(logger, PSTR(" argument given but no "));
  log_str(logger, argument_name);
  log_str(logger, PSTR(" specified."));
  log_flush(logger);
  return log_status(logger);
}

int log_unsupported_url_scheme(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Error: unsupported URL scheme."));
  log_flush(logger);
  return log_status(logger);
}

int log_invalid_protocol_version_given(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Error: Invalid protocol version given."));
  log_flush(logger);
  return log_status(logger);
}

int log_unknown_option(const MqttSnLogger *logger, const char *unknown_option) {
  log_str(logger, PSTR("Error: Unknown option "));
  log_str(logger, unknown_option);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
