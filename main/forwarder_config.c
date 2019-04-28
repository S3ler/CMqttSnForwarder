
#include "forwarder_config.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <linux/stderr/StderrLogging.h>
#include <MqttSnForwarderLoggingBasic.h>

int log_unknown_option(const MqttSnLogger *logger, const char *unknown_option);
int forwarder_config_init(forwarder_config *fcfg) {
  memset(fcfg, 0, sizeof(*fcfg));

  fcfg->logger = &fcfg->strut_logger;
  if (MqttSnLoggerInit(fcfg->logger, LOG_LEVEL_VERBOSE, stderr_log_init)) {
    return -1;
  }

  memcpy(fcfg->version, VERSION, sizeof(VERSION));
  fcfg->major = MAJOR;
  fcfg->minor = MINOR;
  fcfg->tweak = TWEAK;
  memcpy(fcfg->build_date, CMAKE_BUILD_TIMESTAMP, sizeof(CMAKE_BUILD_TIMESTAMP));

  fcfg->log_lvl = LOG_LEVEL_DEFAULT;

  fcfg->protocol_version = MQTT_SN_PROTOCOL_V1;

  memcpy(fcfg->localhost, DEFAULT_LOCALHOST, sizeof(DEFAULT_LOCALHOST));
  fcfg->mqtt_sn_gateway_host = fcfg->localhost;
  fcfg->mqtt_sn_gateway_port = 8888;
  memcpy(fcfg->udp, DEFAULT_UDP, sizeof(DEFAULT_UDP));
  fcfg->gateway_network_protocol = fcfg->udp;
  fcfg->gateway_network_bind_port = 9999;
  memcpy(fcfg->udp, DEFAULT_UDP, sizeof(DEFAULT_UDP));
  fcfg->client_network_protocol = fcfg->udp;
  fcfg->client_network_bind_port = 7777;

  fcfg->gateway_network_send_timeout = GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT;
  fcfg->gateway_network_receive_timeout = GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT;

  fcfg->client_network_send_timeout = CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT;
  fcfg->client_network_receive_timeout = CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT;

  return 0;
}

void forwarder_config_cleanup(forwarder_config *fcfg) {
  if (fcfg->mqtt_sn_gateway_host != fcfg->localhost) {
    free(fcfg->mqtt_sn_gateway_host);
  }
  if (fcfg->gateway_network_protocol != fcfg->udp) {
    free(fcfg->gateway_network_protocol);
  }
  if (fcfg->client_network_protocol != fcfg->udp) {
    free(fcfg->client_network_protocol);
  }
  MqttSnLoggerDeinit(fcfg->logger);
}

static int parse_port(const forwarder_config *fcfg, char *port_str, int *dst) {
  char *endprt;
  long int n = strtol(port_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > 65535)) {
    print_invalid_port_given(fcfg->logger, n);
    return -1;
  }
  *dst = n;
  return 0;
}

static int parse_timeout(const forwarder_config *fcfg, char *timeout_str, int *dst) {
  char *endprt;
  long int n = strtol(timeout_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > UINT32_MAX)) {
    print_invalid_timeout_given(fcfg->logger, n);
    return -1;
  }
  *dst = n;
  return 0;
}

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]) {

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config-file")) {
      if (i == argc - 1) {
        log_no_config_file_given(fcfg->logger, argv[i]);
        return 1;
      } else {
#ifdef Arduino_h
        File config_file = myFile = SD.open(argv[i + 1], FILE_READ);
        if (config_file) {
          if (config_file.size() > nByte) {
            while (config_file.available()) {
              uint16_t nByte = 1024;
              char buf[1024] = {0};

              int len = config_file.read(&buf, nByte);
              char line_copy[len];
              memcpy(line_copy, line, len);
              int argc_line = get_arc_line_len(buf);

              char *argv_line[argc_line];
              parse_argv_line(argv_line, argv, line_copy, argc_line);

              if (process_forwarder_config_line(fcfg, argc_line, argv_line)) {
                config_file.close();
                Serial.write("Error: Could not read config file.\n""");
                return 1;
              }
            }
          }
        }
        config_file.close();
#else
        FILE *config_file = fopen(argv[i + 1], "r");
        if (!config_file) {
          log_could_not_read_config_file(fcfg->logger, strerror(errno));
          return 1;
        }
        char *line;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, config_file)) != -1) {
          char line_copy[len];
          memcpy(line_copy, line, len);

          int argc_line = get_arc_line_len(line);

          char *argv_line[argc_line];
          parse_argv_line(argv_line, argv, line_copy, argc_line);

          if (process_forwarder_config_line(fcfg, argc_line, argv_line)) {
            fclose(config_file);
            log_could_not_read_config_file(fcfg->logger, strerror(errno));
            if (line) {
              free(line);
            }
            return 1;
          }
        }
        fclose(config_file);
        if (line) {
          free(line);
        }
#endif
      }
      i++;
    } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--mqtt_sn_gateway_host")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "host");
        return 1;
      } else {
        free(fcfg->mqtt_sn_gateway_host); // free default value
        fcfg->mqtt_sn_gateway_host = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--mqtt_sn_gateway_port")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "port");
        return 1;
      } else {
        if (parse_port(fcfg, argv[i + 1], &fcfg->mqtt_sn_gateway_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-L") || !strcmp(argv[i], "--url")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "URL");
        return 1;
      } else {
        char *url = argv[i + 1];

        if (!strncasecmp(url, "mqttsn://", 7 + 2)) {
          url += 7;
          fcfg->mqtt_sn_gateway_port = 8888;
        } else {
          log_unsupported_url_scheme(fcfg->logger);
          return 1;
        }
        free(fcfg->mqtt_sn_gateway_host);
        fcfg->mqtt_sn_gateway_host = url + 2;
        char *tmp = strchr(url, ':');
        if (tmp) {
          *tmp++ = 0;
          if (parse_port(fcfg, tmp, &fcfg->mqtt_sn_gateway_port)) {
            return 1;
          }
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gP") || !strcmp(argv[i], "--gateway_network_protocol")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "protocol");
        return 1;
      } else {
        fcfg->gateway_network_protocol = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-gA") || !strcmp(argv[i], "--gateway_network_bind_address")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "address");
        return 1;
      } else {
        fcfg->gateway_network_bind_address = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-gp") || !strcmp(argv[i], "--gateway_network_bind_port")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "port");
        return 1;
      } else {
        if (parse_port(fcfg, argv[i + 1], &fcfg->gateway_network_bind_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gL") || !strcmp(argv[i], "--gateway_network_url")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "URL");
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
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (addr) {
          fcfg->gateway_network_bind_address = strdup(addr);
        } else {
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (port) {
          if (parse_port(fcfg, port, &fcfg->gateway_network_bind_port)) {
            return 1;
          }
        }

        if (null_token) {
          log_unsupported_url_scheme(fcfg->logger);
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gst") || !strcmp(argv[i], "--gateway_send_timeout")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "timeout");
        return 1;
      } else {
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->gateway_network_send_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-grt") || !strcmp(argv[i], "--gateway_receive_timeout")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "timeout");
        return 1;
      } else {
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->gateway_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gt") || !strcmp(argv[i], "--gateway_network_timeout")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "timeout");
        return 1;
      } else {
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->gateway_network_send_timeout)) {
          return 1;
        }
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->gateway_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    }
#ifdef WITH_PLUGIN
    else if (!strcmp(argv[i], "-gnp") || !strcmp(argv[i], "--gateway_network_plugin")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "path");
        return 1;
      } else {
        fcfg->gateway_network_plugin_path = strdup(argv[i + 1]);
      }
      i++;
    }
#endif
    else if (!strcmp(argv[i], "-cP") || !strcmp(argv[i], "--client_network_protocol")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "protocol");
        return 1;
      } else {
        fcfg->client_network_protocol = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-cA") || !strcmp(argv[i], "--client_network_bind_address")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "address");
        return 1;
      } else {
        fcfg->client_network_bind_address = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-cp") || !strcmp(argv[i], "--client_network_bind_port")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "port");
        return 1;
      } else {
        if (parse_port(fcfg, argv[i + 1], &fcfg->client_network_bind_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cst") || !strcmp(argv[i], "--client_send_timeout")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "timeout");
        return 1;
      } else {
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->client_network_send_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-crt") || !strcmp(argv[i], "--client_receive_timeout")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "timeout");
        return 1;
      } else {
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->client_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-ct") || !strcmp(argv[i], "--client_network_timeout")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "timeout");
        return 1;
      } else {
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->client_network_send_timeout)) {
          return 1;
        }
        if (parse_timeout(fcfg, argv[i + 1], &fcfg->client_network_receive_timeout)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cL") || !strcmp(argv[i], "--client_network_url")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "URL");
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
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (addr) {
          free(fcfg->client_network_bind_address);
          fcfg->client_network_bind_address = strdup(addr);
        } else {
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (port) {
          if (parse_port(fcfg, port, &fcfg->client_network_bind_port)) {
            return 1;
          }
        }

        if (null_token) {
          log_unsupported_url_scheme(fcfg->logger);
        }
      }
      i++;
    }
#ifdef WITH_PLUGIN
    else if (!strcmp(argv[i], "-cnp") || !strcmp(argv[i], "--client_network_plugin")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "path");
        return 1;
      } else {
        fcfg->client_network_plugin_path = strdup(argv[i + 1]);
      }
      i++;
    }
#endif
    else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--protocol-version")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "version");
        return 1;
      } else {
        if (!strcmp(argv[i + 1], "mqttsnv1")) {
          fcfg->protocol_version = MQTT_SN_PROTOCOL_V1;
        } else {
          log_invalid_protocol_version_given(fcfg->logger);
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
      log_unknown_option(fcfg->logger, argv[i]);
      return 1;
    }
  }

  return 0;
}

void parse_argv_line(char **argv_line, char **argv, char *line_copy, int argc_line) {

  // parse argv_line
  int tk_count = 0;
  *argv_line[tk_count++] = *argv[0];
  for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
    argv_line[tk_count++] = tk;
  }
  // remove '\n' from tokens
  for (uint16_t i = 0; i < argc_line; i++) {
    if (argv_line[i][(strlen(argv_line[i]) - 1)] == '\n') {
      argv_line[i][(strlen(argv_line[i]) - 1)] = '\0';
    }
  }
}
int get_arc_line_len(char *line) {
  int argc_line = 1;

  // estimates argc_line
  for (char *tk = strtok(line, " "); tk != NULL; tk = strtok(NULL, " ")) {
    argc_line++;
  }
  return argc_line;
}

int print_usage(const MqttSnLogger *logger) {
  log_str(logger,"cmqttsnforwarder is a simple mqtt-sn forwarder that will forward mqtt-sn messages\n");
  log_str(logger,"                 from a client network over a gateway network to a mqtt-sn gateway.\n");
  log_str(logger,"cmqttsnforwarder version ");
  log_str(logger, VERSION);
  log_str(logger," is ");
  log_uint64(logger, MAJOR);
  log_str(logger,".");
  log_uint64(logger, MINOR);
  log_str(logger,".");
  log_uint64(logger, TWEAK);
  log_str(logger," (build date ");
  log_str(logger,CMAKE_BUILD_TIMESTAMP);
  log_str(logger,").");
  log_flush(logger);
  log_flush(logger);

  log_str(logger,"Usage: cmqttsnforwarder {[[-h mqtt_sn_gateway_host] [-p mqtt_sn_gateway_port]] | -L URL}\n");
  log_str(logger,"                        {[[-gP gateway_network_protocol] [-gA gateway_network_bind_address]"
         " [-gp gateway_network_bind_port]] | -gL URL}\n");
  log_str(logger,"                        {[[-cP client_network_protocol] [-cA client_network_bind_address]"
         " [-cp client_network_bind_port]] | -cL URL}\n");
  log_str(logger,"                        {[[-gst gateway_send_timeout] [-grt gateway_receive_timeout]]"
         " | --gt gateway_network_timeout}\n");
  log_str(logger,"                        {[[-cst client_send_timeout] [-crt client_receive_timeout]]"
         " | --ct client_network_timeout}\n");
#ifdef WITH_PLUGIN
  log_str(logger,"                        [-gnp gateway_network_plugin]\n");
  log_str(logger,"                        [-cnp client_network_plugin]\n");
#endif
#if defined(WITH_LOGGING)
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger,"                        [[-q quiet] | [-d default] | [-v verbose] | [-db debug]]\n");
#else
  log_str(logger,"                        [[-q quiet] | [-d default] | [-v verbose]]\n");
#endif
#endif
  log_str(logger,"       [-c --config-file]\n");
  log_str(logger,"       cmqttsnforwarder --help\n\n");

  log_str(logger," -h : mqtt-sn gateway host to connect to. Defaults to localhost.\n");
  log_str(logger," -p : mqtt-sn gateway network port to connect to. Defaults to 8888 for plain MQTT-SN.\n");
  log_str(logger," -L : specify hostname, port as a URL in the form:\n");
  log_str(logger,"      mqtt-sn(s)://host[:port]\n");
  log_str(logger,"      mqtt-sn(s) uses the same protocol as the gateway network.\n");

  log_str(logger," -gP : specify the protocol of the gateway network to use.\n");
  log_str(logger,"       Can be udp, tcp. Defaults to udp.\n");
  log_str(logger," -gA : bind the gateway network to the outgoing socket to this host/ip address.\n");
  log_str(logger,"       Use to control which interface the network communicates over.\n");
  log_str(logger," -gp : listening on the specific gateway network port. Defaults to 9999.\n");
  log_str(logger," -gL : specify outgoing socket, port as a URL in the form: address[:port]\n");

  log_str(logger," -cP : specify the protocol of the client network to use.\n");
  log_str(logger,"       Can be udp, tcp. Defaults to udp.\n");
  log_str(logger," -cA : bind the client network to the outgoing socket to this host/ip address.\n");
  log_str(logger,"       Use to control which interface the network communicates over.\n");
  log_str(logger," -cp : listening on the specific client network port. Defaults to 7777.\n");
  log_str(logger," -cL : specify outgoing socket, port as a URL in the form: address[:port]\n");

  log_str(logger," -gst : specify the gateway network send timeout in ms.\n");
  log_str(logger,"       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger," -grt : specify the gateway network receive timeout in ms.\n");
  log_str(logger,"       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger," -gt  : specify the gateway network send and receive timeout in ms. Defaults to 1000 ms.\n");
  log_str(logger,"       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger," -cst : specify the client network send timeout in ms.\n");
  log_str(logger,"       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger," -crt : specify the client network receive timeout in ms.\n");
  log_str(logger,"       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");
  log_str(logger," -ct  : specify the client network send and receive timeout in ms. Defaults to 1000 ms.\n");
  log_str(logger,"       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to 1000 ms.\n");

  log_str(logger," -V : specify the version of the MQTT-SN protocol to use.\n");
  log_str(logger,"      Can be mqttsnv1. Defaults to mqttsnv1.\n");
#ifdef WITH_PLUGIN
  log_str(logger," -gnp : path to the gateway network plugin.\n");
  log_str(logger,"       The gateway network protocol must match the short protocol name gained from the plugin.\n");
  log_str(logger,"       A gateway bind address is saved to a device address and given to the gateway network plugin.\n");
  log_str(logger,"       A port overwrites the two last bytes of the gateway network bind address. "
         "You can set gateway port to -1 if you want to overwrite this behaviour\n");
  log_str(logger," -cnp : absolute path to the client network plugin.\n");
  log_str(logger,"       The client network protocol must match the short protocol name gained from the plugin.\n");
  log_str(logger,"       A client bind address is saved to a device address and given to the gateway network plugin.\n");
  log_str(logger,"       A client port overwrites the two last bytes of the gateway network bind address. "
         "You can set gateway port to -1 if you want to overwrite this behaviour\n");
#endif
#if defined(WITH_LOGGING)
  log_str(logger," -q : specify quiet logging. Don't print any log messages.\n");
  log_str(logger," -d : specify default logging. Print network status changes, and the mqtt-sn messages: "
         "PUBLISH, CONNECT, CONNACK, DISCONNECT.\n");
  log_str(logger," -v : specify verbose logging. Print all default logging and all MQTT-SN messages.\n");
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger," -db : specify debug logging. Print all mqtt-sn messages including payload and internal information.\n");
#endif
#endif
  log_str(logger," -c : specify the config file.\n");
  log_str(logger," --help : display this message.\n");
  log_flush(logger);
  log_str(logger, "See ");
  log_str(logger, MANUAL_WEBSITE);
  log_str(logger, " for more information.\n");

  log_flush(logger);
  return log_status(logger);
}

int print_invalid_port_given(const MqttSnLogger *logger, long invalid_port) {
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

int log_no_config_file_given(const MqttSnLogger *logger, char *string) {
  log_str(logger, "Error: ");
  log_str(logger, string);
  log_str(logger, " argument given but no config file specified.\n");
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
  log_str(logger, "specified.\n");
  log_flush(logger);
  return log_status(logger);
}

int log_unsupported_url_scheme(const MqttSnLogger *logger) {
  log_str(logger, "\"Error: unsupported URL scheme.");
  log_flush(logger);
  return log_status(logger);
}

int log_invalid_protocol_version_given(const MqttSnLogger *logger) {
  log_str(logger, "\"Error: Invalid protocol version given.");
  log_flush(logger);
  return log_status(logger);
}

int log_unknown_option(const MqttSnLogger *logger, const char *unknown_option) {
  log_str(logger, "Error: Unknown option ");
  log_str(logger, unknown_option);
  log_str(logger, ".");
  log_flush(logger);
  return log_status(logger);
}

