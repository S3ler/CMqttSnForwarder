#include "forwarder_config.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <forwarder/logging/MqttSnForwarderLoggingBasic.h>
#include <forwarder/logging/linux/stderr/StderrLogging.h>

// TODO remove strdup
int forwarder_config_init(forwarder_config *fcfg, MqttSnLogger *logger) {
  memset(fcfg, 0, sizeof(*fcfg));

  fcfg->logger = logger;

  memcpy(fcfg->version, VERSION, sizeof(VERSION));
  fcfg->major = MAJOR;
  fcfg->minor = MINOR;
  fcfg->tweak = TWEAK;
  memcpy(fcfg->build_date, CMAKE_BUILD_TIMESTAMP, sizeof(CMAKE_BUILD_TIMESTAMP));

  fcfg->log_lvl = LOG_LEVEL_DEFAULT;

  fcfg->protocol_version = MQTT_SN_PROTOCOL_V1;

  // gateway network
  memcpy(fcfg->localhost, DEFAULT_MQTT_SN_GATEWAY_HOST, sizeof(DEFAULT_MQTT_SN_GATEWAY_HOST));
  fcfg->mqtt_sn_gateway_host = fcfg->localhost;
  fcfg->mqtt_sn_gateway_port = 8888;
  memcpy(fcfg->udp, DEFAULT_UDP, sizeof(DEFAULT_UDP));
  fcfg->gateway_network_protocol = fcfg->udp;
  fcfg->gateway_network_bind_port = 9999;
  // gateway network broadcast
  fcfg->gateway_network_broadcast_protocol = fcfg->udp;
  memcpy(fcfg->gateway_network_default_broadcast_address,
         DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS,
         sizeof(DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS));
  fcfg->gateway_network_broadcast_address = fcfg->gateway_network_default_broadcast_address;
  fcfg->gateway_network_broadcast_bind_port = DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT;

  // client network
  memcpy(fcfg->udp, DEFAULT_UDP, sizeof(DEFAULT_UDP));
  fcfg->client_network_protocol = fcfg->udp;
  fcfg->client_network_bind_port = 7777;
  // client network broadcast
  fcfg->client_network_broadcast_protocol = fcfg->udp;
  memcpy(fcfg->client_network_default_broadcast_address,
         DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS,
         sizeof(DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS));
  fcfg->client_network_broadcast_address = fcfg->client_network_default_broadcast_address;
  fcfg->client_network_broadcast_bind_port = DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT;

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
    print_fcfg_invalid_port_given(fcfg->logger, n);
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

int process_forwarder_config_str(forwarder_config *fcfg, char *line, size_t len, char *argv_o) {
  if (line == NULL) {
    return -1;
  }
  if (len == 0) {
    return -1;
  }

  if (strlen(line) + 1 != len) {
    return -1;
  }

  if (strlen(line) == 0) {
    return -1;
  }

  int tk_count = 0;
  {
    char line_copy[len];
    memcpy(line_copy, line, len);
    // estimates argc
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      tk_count++;
    }
  }
  char *argv[tk_count];
  int argc = 0;
  char line_copy[len];
  {
    memcpy(line_copy, line, len);
    argv[argc++] = argv_o;
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      argv[argc++] = tk;
    }
  }

  // remove '\n' from tokens
  for (uint16_t i = 1; i < argc; i++) {
    if (argv[i][(strlen(argv[i]) - 1)] == '\n') {
      argv[i][(strlen(argv[i]) - 1)] = '\0';
    }
  }

#ifdef WITH_DEBUG_LOGGING
  log_str(fcfg->logger, "argc: ");
  log_uint64(fcfg->logger, argc);
  log_str(fcfg->logger, " argv: ");
  for (uint16_t i = 0; i < argc; i++) {
    log_str(fcfg->logger, argv[i]);
    if (i + 1 < argc) {
      log_str(fcfg->logger, ", ");
    }
  }
  log_flush(fcfg->logger);
#endif
  return process_forwarder_config_line(fcfg, argc, argv);
}

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
#if defined(WITH_CONFIG_FILE)
    if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config-file")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "config file");
        return 1;
      } else {
#if defined(Arduino_h) && defined(__SD_H__)
        File config_file = SD.open(argv[i + 1], FILE_READ);
        if (!config_file) {
          log_could_not_read_config_file(fcfg->logger, "could not open file");
          config_file.close();
          return 1;
        }
        uint16_t len = 1024;
        if (config_file.size() > len) {
          log_could_not_read_config_file(fcfg->logger, "line too long");
          config_file.close();
          return 1;
        }
        while (config_file.available()) {
          char line[1024] = {0};
          int len = config_file.read(&line, len);
          if (process_forwarder_config_str(fcfg, line, len, argv[0])) {
            config_file.close();
            log_could_not_read_config_file(fcfg->logger, "see above");
            return 1;
          }
        }
        config_file.close();
#else
        FILE *config_file = fopen(argv[i + 1], "r");
        if (!config_file) {
          log_could_not_read_config_file(fcfg->logger, strerror(errno));
          fclose(config_file);
          return 1;
        }
        char *line;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, config_file)) != -1) {
          if (process_forwarder_config_str(fcfg, line, len, argv[0])) {
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
    } else
#endif
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--mqtt_sn_gateway_host")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "host");
        return 1;
      } else {
        // TODO strdup
        if (fcfg->mqtt_sn_gateway_host != fcfg->localhost) {
          free(fcfg->mqtt_sn_gateway_host);
        }
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
        // TODO strdup
        if (fcfg->mqtt_sn_gateway_host != fcfg->localhost) {
          free(fcfg->mqtt_sn_gateway_host);
        }
        //TODO check if next line works
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
        // TODO strdup
        if (fcfg->gateway_network_protocol != fcfg->udp) {
          free(fcfg->gateway_network_protocol);
        }
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
          // TODO strdup
          if (fcfg->gateway_network_protocol != fcfg->udp) {
            free(fcfg->gateway_network_protocol);
          }
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
        // TODO strdup
        if (fcfg->client_network_protocol != fcfg->udp) {
          free(fcfg->client_network_protocol);
        }
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
          // TODO strdup
          if (fcfg->client_network_protocol != fcfg->udp) {
            free(fcfg->client_network_protocol);
          }
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
#if defined(WITH_TCP_BROADCAST) || defined(WITH_UDP_BROADCAST)
    // gateway network broadcast
    else if (!strcmp(argv[i], "-gbP") || !strcmp(argv[i], "--gateway_network_broadcast_protocol")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "protocol");
        return 1;
      } else {
        fcfg->gateway_network_broadcast_protocol = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "-gbA") || !strcmp(argv[i], "--gateway_network_broadcast_address")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "address");
        return 1;
      } else {
        fcfg->gateway_network_broadcast_address = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "-gbp") || !strcmp(argv[i], "--gateway_network_broadcast_bind_port")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "port");
        return 1;
      } else {
        if (parse_port(fcfg, argv[i + 1], &fcfg->gateway_network_broadcast_bind_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gbL") || !strcmp(argv[i], "--gateway_network_broadcast_url")) {
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
          fcfg->gateway_network_broadcast_protocol = prot;
        } else {
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (addr) {
          fcfg->gateway_network_broadcast_address = addr;
        } else {
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (port) {
          if (parse_port(fcfg, port, &fcfg->gateway_network_broadcast_bind_port)) {
            return 1;
          }
        }

        if (null_token) {
          log_unsupported_url_scheme(fcfg->logger);
        }
      }
      i++;
    }
    // client network broadcast
    else if (!strcmp(argv[i], "-cbP") || !strcmp(argv[i], "--client_network_broadcast_protocol")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "protocol");
        return 1;
      } else {
        fcfg->client_network_broadcast_protocol = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "-cbA") || !strcmp(argv[i], "--client_network_broadcast_address")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "address");
        return 1;
      } else {
        fcfg->client_network_broadcast_address = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "-cbp") || !strcmp(argv[i], "--client_network_broadcast_bind_port")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(fcfg->logger, argv[i], "port");
        return 1;
      } else {
        if (parse_port(fcfg, argv[i + 1], &fcfg->client_network_broadcast_bind_port)) {
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cbL") || !strcmp(argv[i], "--client_network_broadcast_url")) {
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
          fcfg->client_network_broadcast_protocol = prot;
        } else {
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (addr) {
          fcfg->client_network_broadcast_address = addr;
        } else {
          log_unsupported_url_scheme(fcfg->logger);
        }

        if (port) {
          if (parse_port(fcfg, port, &fcfg->client_network_broadcast_bind_port)) {
            return 1;
          }
        }

        if (null_token) {
          log_unsupported_url_scheme(fcfg->logger);
        }
      }
      i++;
    }
#endif
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
      return FORWARDER_CONFIG_HELP;
    } else {
      log_unknown_option(fcfg->logger, argv[i]);
      return 1;
    }
  }

  return 0;
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

#ifdef WITH_PLUGIN
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

#if defined(WITH_CONFIG_FILE)
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
  log_str(logger, "       Can be udp, tcp. Defaults to udp.\n");
  log_str(logger, " -gA : bind the gateway network to the outgoing socket to this host/ip address.\n");
  log_str(logger, "       Use to control which interface the network communicates over.\n");
  log_str(logger, " -gp : listening on the specific gateway network port. Defaults to 9999.\n");
  log_str(logger, " -gL : specify protocol, broadcast address, port as a URL in the form: protocol://address[:port]\n");

  log_str(logger, " -cP : specify the protocol of the client network to use.\n");
  log_str(logger, "       Can be udp, tcp. Defaults to udp.\n");
  log_str(logger, " -cA : bind the client network to the outgoing socket to this host/ip address.\n");
  log_str(logger, "       Use to control which interface the network communicates over.\n");
  log_str(logger, " -cp : listening on the specific client network port. Defaults to 7777.\n");
  log_str(logger, " -cL : specify protocol, broadcast address, port as a URL in the form: protocol://address[:port]\n");

#if defined(WITH_TCP_BROADCAST) || defined(WITH_UDP_BROADCAST)
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
#ifdef WITH_PLUGIN
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
#if defined(WITH_CONFIG_FILE)
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

