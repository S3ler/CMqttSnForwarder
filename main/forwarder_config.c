
#include "forwarder_config.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void forwarder_config_init(forwarder_config *fcfg) {
  memset(fcfg, 0, sizeof(*fcfg));
  fcfg->version = strdup(VERSION);
  fcfg->major = MAJOR;
  fcfg->minor = MINOR;
  fcfg->tweak = TWEAK;
  fcfg->build_date = strdup(CMAKE_BUILD_TIMESTAMP);

  fcfg->log_lvl = LOG_LEVEL_DEFAULT;

  fcfg->protocol_version = MQTT_SN_PROTOCOL_V1;

  fcfg->mqtt_sn_gateway_host = strdup("localhost");
  fcfg->mqtt_sn_gateway_port = 8888;
  fcfg->gateway_network_protocol = strdup("udp");
  fcfg->gateway_network_bind_port = 9999;
  fcfg->client_network_protocol = strdup("udp");
  fcfg->client_network_bind_port = 7777;

  fcfg->gateway_network_send_timeout = GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT;
  fcfg->gateway_network_receive_timeout = GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT;

  fcfg->client_network_send_timeout = CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT;
  fcfg->client_network_receive_timeout = CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT;
}

void forwarder_config_cleanup(forwarder_config *fcfg) {
  free(fcfg->version);
  free(fcfg->build_date);
  free(fcfg->mqtt_sn_gateway_host);
  free(fcfg->gateway_network_protocol);
  free(fcfg->client_network_protocol);
}

static int parse_port(char *port_str, int *dst) {
  char *endprt;
  long int n = strtol(port_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > 65535)) {
    fprintf(stderr, "Error: Invalid port given: %ld\n", n);
    return -1;
  }
  *dst = n;
  return 0;
}

static int parse_timeout(char *timeout_str, int *dst) {
  char *endprt;
  long int n = strtol(timeout_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > UINT32_MAX)) {
    fprintf(stderr, "Error: Invalid timeout given: %ld\n", n);
    return -1;
  }
  *dst = n;
  return 0;
}

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]) {

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--mqtt_sn_gateway_host")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no host specified.\n\n", argv[i]);
        return 1;
      } else {
        free(fcfg->mqtt_sn_gateway_host); // free default value
        fcfg->mqtt_sn_gateway_host = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--mqtt_sn_gateway_port")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no port specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_port(argv[i + 1], &fcfg->mqtt_sn_gateway_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-L") || !strcmp(argv[i], "--url")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no URL specified.\n\n", argv[i]);
        return 1;
      } else {
        char *url = argv[i + 1];

        if (!strncasecmp(url, "mqttsn://", 7 + 2)) {
          url += 7;
          fcfg->mqtt_sn_gateway_port = 8888;
        } else {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
          return 1;
        }
        free(fcfg->mqtt_sn_gateway_host);
        fcfg->mqtt_sn_gateway_host = url + 2;
        char *tmp = strchr(url, ':');
        if (tmp) {
          *tmp++ = 0;
          if (parse_port(tmp, &fcfg->mqtt_sn_gateway_port)) {
            return 1;
          }
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gP") || !strcmp(argv[i], "--gateway_network_protocol")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no protocol specified.\n\n", argv[i]);
        return 1;
      } else {
        free(fcfg->gateway_network_protocol);
        fcfg->gateway_network_protocol = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-gA") || !strcmp(argv[i], "--gateway_network_bind_address")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no address specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->gateway_network_bind_address = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-gp") || !strcmp(argv[i], "--gateway_network_bind_port")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no port specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_port(argv[i + 1], &fcfg->gateway_network_bind_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gL") || !strcmp(argv[i], "--gateway_network_url")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no URL specified.\n\n", argv[i]);
        return 1;
      } else {
        char *url = argv[i + 1];

        char *prot = strtok(url, "://");
        char *addr = strtok(NULL, "://");
        char *port = strtok(NULL, ":");
        char *null_token = strtok(NULL, "://");

        if (prot) {
          free(fcfg->gateway_network_protocol);
          fcfg->gateway_network_protocol = strdup(prot);
        } else {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
        }

        if (addr) {
          fcfg->gateway_network_bind_address = strdup(addr);
        } else {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
        }

        if (port) {
          if (parse_port(port, &fcfg->gateway_network_bind_port)) {
            return 1;
          }
        }

        if (null_token) {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gst") || !strcmp(argv[i], "--gateway_send_timeout")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no timeout specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_timeout(argv[i + 1], &fcfg->gateway_network_send_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-grt") || !strcmp(argv[i], "--gateway_receive_timeout")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no timeout specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_timeout(argv[i + 1], &fcfg->gateway_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gt") || !strcmp(argv[i], "--gateway_network_timeout")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no timeout specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_timeout(argv[i + 1], &fcfg->gateway_network_send_timeout)) {
          return 1;
        }
        if (parse_timeout(argv[i + 1], &fcfg->gateway_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    }
#ifdef WITH_PLUGIN
    else if (!strcmp(argv[i], "-gnp") || !strcmp(argv[i], "--gateway_network_plugin")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no path specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->gateway_network_plugin_path = strdup(argv[i + 1]);
      }
      i++;
    }
#endif
    else if (!strcmp(argv[i], "-cP") || !strcmp(argv[i], "--client_network_protocol")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no protocol specified.\n\n", argv[i]);
        return 1;
      } else {
        free(fcfg->client_network_protocol);
        fcfg->client_network_protocol = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-cA") || !strcmp(argv[i], "--client_network_bind_address")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no address specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->client_network_bind_address = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-cp") || !strcmp(argv[i], "--client_network_bind_port")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no port specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_port(argv[i + 1], &fcfg->client_network_bind_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cst") || !strcmp(argv[i], "--client_send_timeout")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no timeout specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_timeout(argv[i + 1], &fcfg->client_network_send_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-crt") || !strcmp(argv[i], "--client_receive_timeout")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no timeout specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_timeout(argv[i + 1], &fcfg->client_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-ct") || !strcmp(argv[i], "--client_network_timeout")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no timeout specified.\n\n", argv[i]);
        return 1;
      } else {
        if (parse_timeout(argv[i + 1], &fcfg->client_network_send_timeout)) {
          return 1;
        }
        if (parse_timeout(argv[i + 1], &fcfg->client_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cL") || !strcmp(argv[i], "--client_network_url")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no URL specified.\n\n", argv[i]);
        return 1;
      } else {
        char *url = argv[i + 1];

        char *prot = strtok(url, "://");
        char *addr = strtok(NULL, "://");
        char *port = strtok(NULL, ":");
        char *null_token = strtok(NULL, "://");

        if (prot) {
          free(fcfg->client_network_protocol);
          fcfg->client_network_protocol = strdup(prot);
        } else {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
        }

        if (addr) {
          free(fcfg->client_network_bind_address);
          fcfg->client_network_bind_address = strdup(addr);
        } else {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
        }

        if (port) {
          if (parse_port(port, &fcfg->client_network_bind_port)) {
            return 1;
          }
        }

        if (null_token) {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
        }
      }
      i++;
    }
#ifdef WITH_PLUGIN
    else if (!strcmp(argv[i], "-cnp") || !strcmp(argv[i], "--client_network_plugin")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no path specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->client_network_plugin_path = strdup(argv[i + 1]);
      }
      i++;
    }
#endif
    else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--protocol-version")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no version specified.\n\n", argv[i]);
        return 1;
      } else {
        if (!strcmp(argv[i + 1], "mqttsnv1")) {
          fcfg->protocol_version = MQTT_SN_PROTOCOL_V1;
        } else {
          fprintf(stderr, "Error: Invalid protocol version given.\n\n");
          return 1;
        }
        i++;
      }
    }
#if defined(WITH_LOGGING)
    else if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
      fcfg->log_lvl = LOG_LEVEL_QUIET;
    } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--default")) {
      fcfg->log_lvl = LOG_LEVEL_DEFAULT;
    } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
      fcfg->log_lvl = LOG_LEVEL_VERBOSE;
    }
#if defined(WITH_DEBUG_LOGGING)
    else if (!strcmp(argv[i], "-db") || !strcmp(argv[i], "--debug")) {
      fcfg->log_lvl = LOG_LEVEL_DEBUG;
    }
#endif
#endif
    else if (!strcmp(argv[i], "--help")) {
      return 2;
    } else {
      fprintf(stderr, "Error: Unknown option '%s'.\n", argv[i]);
      return 1;
    }
  }

  return 0;
}

void print_usage(void) {
  printf("cmqttsnforwarder is a simple mqtt-sn forwarder that will forward mqtt-sn messages\n");
  printf("                 from a client network over a gateway network to a mqtt-sn gateway.\n");
  printf("cmqttsnforwarder version %s is %d.%d.%d (build date %s).\n\n",
         VERSION,
         MAJOR,
         MINOR,
         TWEAK,
         CMAKE_BUILD_TIMESTAMP);
  printf("Usage: cmqttsnforwarder {[[-h mqtt_sn_gateway_host] [-p mqtt_sn_gateway_port]] | -L URL}\n");
  printf("                        {[[-gP gateway_network_protocol] [-gA gateway_network_bind_address]"
         " [-gp gateway_network_bind_port]] | -gL URL}\n");
  printf("                        {[[-cP client_network_protocol] [-cA client_network_bind_address]"
         " [-cp client_network_bind_port]] | -cL URL}\n");
  printf("                        {[[-gst gateway_send_timeout] [-grt gateway_receive_timeout]]"
         " | --gt gateway_network_timeout}\n");
  printf("                        {[[-cst client_send_timeout] [-crt client_receive_timeout]]"
         " | --ct client_network_timeout}\n");
#ifdef WITH_PLUGIN
  printf("                        [-gnp gateway_network_plugin]\n");
  printf("                        [-cnp client_network_plugin]\n");
#endif
#if defined(WITH_LOGGING)
#if defined(WITH_DEBUG_LOGGING)
  printf("                        [[-q quiet] | [-d default] | [-v verbose] | [-db debug]]\n");
#else
  printf("                        [[-q quiet] | [-d default] | [-v verbose]]\n");
#endif
#endif
  printf("       cmqttsnforwarder --help\n\n");

  printf(" -h : mqtt-sn gateway host to connect to. Defaults to localhost.\n");
  printf(" -p : mqtt-sn gateway network port to connect to. Defaults to 8888 for plain MQTT-SN.\n");
  printf(" -L : specify hostname, port as a URL in the form:\n");
  printf("      mqtt-sn(s)://host[:port]\n");
  printf("      mqtt-sn(s) uses the same protocol as the gateway network.\n");

  printf(" -gP : specify the protocol of the gateway network to use.\n");
  printf("       Can be udp, tcp. Defaults to udp.\n");
  printf(" -gA : bind the gateway network to the outgoing socket to this host/ip address.\n");
  printf("       Use to control which interface the network communicates over.\n");
  printf(" -gp : listening on the specific gateway network port. Defaults to 9999.\n");
  printf(" -gL : specify outgoing socket, port as a URL in the form: address[:port]\n");

  printf(" -cP : specify the protocol of the client network to use.\n");
  printf("       Can be udp, tcp. Defaults to udp.\n");
  printf(" -cA : bind the client network to the outgoing socket to this host/ip address.\n");
  printf("       Use to control which interface the network communicates over.\n");
  printf(" -cp : listening on the specific client network port. Defaults to 7777.\n");
  printf(" -cL : specify outgoing socket, port as a URL in the form: address[:port]\n");

  printf(" -gst : specify the gateway network send timeout in ms.\n");
  printf("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  printf(" -grt : specify the gateway network receive timeout in ms.\n");
  printf("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  printf(" -gt  : specify the gateway network send and receive timeout in ms. Defaults to 1000 ms.\n");
  printf("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  printf(" -cst : specify the client network send timeout in ms.\n");
  printf("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  printf(" -crt : specify the client network receive timeout in ms.\n");
  printf("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  printf(" -ct  : specify the client network send and receive timeout in ms. Defaults to 1000 ms.\n");
  printf("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");

  printf(" -V : specify the version of the MQTT-SN protocol to use.\n");
  printf("      Can be mqttsnv1. Defaults to mqttsnv1.\n");
#ifdef WITH_PLUGIN
  printf(" -gnp : path to the gateway network plugin.\n");
  printf("       The gateway network protocol must match the short protocol name gained from the plugin.\n");
  printf("       A gateway bind address is saved to a device address and given to the gateway network plugin.\n");
  printf("       A port overwrites the two last bytes of the gateway network bind address. "
         "You can set gateway port to -1 if you want to overwrite this behaviour\n");
  printf(" -cnp : absolute path to the client network plugin.\n");
  printf("       The client network protocol must match the short protocol name gained from the plugin.\n");
  printf("       A client bind address is saved to a device address and given to the gateway network plugin.\n");
  printf("       A client port overwrites the two last bytes of the gateway network bind address. "
         "You can set gateway port to -1 if you want to overwrite this behaviour\n");
#endif
#if defined(WITH_LOGGING)
  printf(" -q : specify quiet logging. Don't print any log messages.\n");
  printf(" -d : specify default logging. Print network status changes, and the mqtt-sn messages: "
         "PUBLISH, CONNECT, CONNACK, DISCONNECT.\n");
  printf(" -v : specify verbose logging. Print all default logging and all MQTT-SN messages.\n");
#if defined(WITH_DEBUG_LOGGING)
  printf(" -db : specify debug logging. Print all mqtt-sn messages including payload and internal information.\n");
#endif
#endif
  printf(" --help : display this message.\n");
  printf("\nSee %s for more information.\n\n", MANUAL_WEBSITE);
}
