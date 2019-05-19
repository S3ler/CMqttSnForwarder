#include "forwarder_config.h"
#include "forwarder_config_logger.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <platform/platform_compatibility.h>

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
#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK) && defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
  fcfg->gateway_network_broadcast_protocol = fcfg->udp;
#elif defined(WITH_LINUX_UDP_GATEWAY_NETWORK)
  fcfg->gateway_network_broadcast_protocol = fcfg->udp;
#elif defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
  fcfg->gateway_network_broadcast_protocol = fcfg->tcp;
#endif
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
#if defined(WITH_LINUX_UDP_CLIENT_NETWORK) && defined(WITH_LINUX_TCP_CLIENT_NETWORK)
  fcfg->client_network_broadcast_protocol = fcfg->udp;
#elif defined(WITH_LINUX_UDP_CLIENT_NETWORK)
  fcfg->client_network_broadcast_protocol = fcfg->udp;
#elif defined(WITH_LINUX_TCP_CLIENT_NETWORK)
  fcfg->client_network_broadcast_protocol = fcfg->tcp;
#endif
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
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > INT32_MAX)) {
    print_invalid_timeout_given(fcfg->logger, n);
    return -1;
  }
  *dst = n;
  return 0;
}

int process_forwarder_config_str(forwarder_config *fcfg, char *line, size_t len, const char *argv_o) {
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
    argv[argc++] = (char *) argv_o;
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
#if defined(WITH_MQTT_SN_FORWADER_CONFIG_FILE)
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
#ifdef WITH_LINUX_PLUGIN_NETWORK
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
#if defined(WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST) || defined(WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST)
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
#endif
#if defined(WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST) || defined(WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST)
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
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
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
    else if (!strcmp_P(argv[i], "--help")) {
      return FORWARDER_CONFIG_HELP;
    } else {
      log_unknown_option(fcfg->logger, argv[i]);
      return 1;
    }
  }

  return 0;
}

