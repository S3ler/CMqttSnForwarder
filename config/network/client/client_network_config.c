//
// Created by SomeDude on 28.05.2019.
//

#include "client_network_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>

int32_t client_network_config_init(client_network_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  memcpy(cfg->default_client_network_protocol,
         DEFAULT_MQTT_SN_CLIENT_PROTOCOL,
         sizeof(DEFAULT_MQTT_SN_CLIENT_PROTOCOL));
  cfg->client_network_protocol = cfg->default_client_network_protocol;
  cfg->client_network_bind_port = DEFAULT_MQTT_SN_CLIENT_BIND_PORT;
#ifdef WITH_CLIENT_NETWORK_BROADCAST
  cfg->client_network_broadcast_protocol = cfg->default_client_network_protocol;
#endif
  memcpy(cfg->default_client_network_broadcast_address,
         DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS,
         sizeof(DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS));
  cfg->client_network_broadcast_address = cfg->default_client_network_broadcast_address;
  cfg->client_network_broadcast_bind_port = DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT;

  cfg->client_network_send_timeout = CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT;
  cfg->client_network_receive_timeout = CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_network_config_cleanup(client_network_config *cfg) {
  // nothing to do here
}
int32_t is_client_network_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-cP") || !strcmp(arg, "--client_network_protocol")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cA") || !strcmp(arg, "--client_network_bind_address")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cp") || !strcmp(arg, "--client_network_bind_port")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cst") || !strcmp(arg, "--client_send_timeout")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-crt") || !strcmp(arg, "--client_receive_timeout")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-ct") || !strcmp(arg, "--client_network_timeout")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cL") || !strcmp(arg, "--client_network_url")) {
    (*i)++;
    return 1;
  }
#if defined(WITH_CLIENT_NETWORK_BROADCAST)
  else if (!strcmp(arg, "-cbP") || !strcmp(arg, "--client_network_broadcast_protocol")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cbA") || !strcmp(arg, "--client_network_broadcast_address")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cbp") || !strcmp(arg, "--client_network_broadcast_bind_port")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cbL") || !strcmp(arg, "--client_network_broadcast_url")) {
    (*i)++;
    return 1;
  }
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
  else if (!strcmp(arg, "-cnp") || !strcmp(arg, "--client_network_plugin")) {
    (*i)++;
    return 1;
  }
#endif
  return 0;
}
int32_t client_network_config_process_args(client_network_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char **argv) {
  int32_t parsed_args = 0;
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-cP") || !strcmp(argv[i], "--client_network_protocol")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "protocol");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->client_network_protocol = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-cA") || !strcmp(argv[i], "--client_network_bind_address")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "address");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->client_network_bind_address = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-cp") || !strcmp(argv[i], "--client_network_bind_port")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "port");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_port(logger, argv[i + 1], &cfg->client_network_bind_port)) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-cst") || !strcmp(argv[i], "--client_send_timeout")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "timeout");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_timeout(logger, argv[i + 1], &cfg->client_network_send_timeout)) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-crt") || !strcmp(argv[i], "--client_receive_timeout")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "timeout");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_timeout(logger, argv[i + 1], &cfg->client_network_receive_timeout)) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-ct") || !strcmp(argv[i], "--client_network_timeout")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "timeout");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_timeout(logger, argv[i + 1], &cfg->client_network_send_timeout)) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
        if (parse_timeout(logger, argv[i + 1], &cfg->client_network_receive_timeout)) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-cL") || !strcmp(argv[i], "--client_network_url")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "URL");
        return 1;
      } else {
        char *url = argv[i + 1];

        char *prot = strtok(url, "://");
        char *addr = strtok(NULL, "://");
        char *port = strtok(NULL, ":");
        char *null_token = strtok(NULL, "://");

        if (prot) {
          cfg->client_network_protocol = prot;
        } else {
          print_unsupported_url_scheme(logger);
        }

        if (addr) {
          cfg->client_network_bind_address = addr;
        } else {
          print_unsupported_url_scheme(logger);
        }

        if (port) {
          if (parse_port(logger, port, &cfg->client_network_bind_port)) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
          }
        }

        if (null_token) {
          print_unsupported_url_scheme(logger);
        }
      }
      i++;
      parsed_args += 2;
    }
#if defined(WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST) || defined(WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST)
      // client network broadcast
    else if (!strcmp(argv[i], "-cbP") || !strcmp(argv[i], "--client_network_broadcast_protocol")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "protocol");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->client_network_broadcast_protocol = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-cbA") || !strcmp(argv[i], "--client_network_broadcast_address")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "address");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->client_network_broadcast_address = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-cbp") || !strcmp(argv[i], "--client_network_broadcast_bind_port")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "port");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_port(logger, argv[i + 1], &cfg->client_network_broadcast_bind_port)) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cbL") || !strcmp(argv[i], "--client_network_broadcast_url")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "URL");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        char *url = argv[i + 1];

        char *prot = strtok(url, "://");
        char *addr = strtok(NULL, "://");
        char *port = strtok(NULL, ":");
        char *null_token = strtok(NULL, "://");

        if (prot) {
          cfg->client_network_broadcast_protocol = prot;
        } else {
          print_unsupported_url_scheme(logger);
        }

        if (addr) {
          cfg->client_network_broadcast_address = addr;
        } else {
          print_unsupported_url_scheme(logger);
        }

        if (port) {
          if (parse_port(logger, port, &cfg->client_network_broadcast_bind_port)) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
          }
        }

        if (null_token) {
          print_unsupported_url_scheme(logger);
        }
      }
      i++;
      parsed_args += 2;
    }
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
    else if (!strcmp(argv[i], "-cnp") || !strcmp(argv[i], "--client_network_plugin")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "path");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->client_network_plugin_path = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    }
#endif
  }
  return parsed_args;
}

void client_network_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("{[[-cP client_network_protocol] [-cA client_network_bind_address]"
                       " [-cp client_network_bind_port]] | -cL URL}\n"));
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("{[[-cst client_send_timeout] [-crt client_receive_timeout]]"
                       " | --ct client_network_timeout}\n"));
#if defined(WITH_UDP_BROADCAST)
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("{[[-cbP client_network_broadcast_protocol] [-cbA client_network_broadcast_address]"
                       " [-cbp client_network_broadcast_bind_port]] | -cbL URL}\n"));
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("[-cnp client_network_plugin]\n"));
#endif
}
void client_network_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -cP : specify the protocol of the client network to use.\n"));
  log_str(logger, PSTR("       Can be "));
#if defined(WITH_LINUX_UDP_CLIENT_NETWORK) && defined(WITH_LINUX_TCP_CLIENT_NETWORK)
  log_str(logger, PSTR("udp, tcp"));
#elif defined(WITH_LINUX_UDP_CLIENT_NETWORK)
  log_str(logger, PSTR("udp"));
#endif
  log_str(logger, PSTR(". Defaults to "));
  log_str(logger, PSTR(DEFAULT_MQTT_SN_CLIENT_PROTOCOL));
  log_str(logger, PSTR(".\n"));

  log_str(logger, PSTR(" -cp : listening on the specific client network port. Defaults to "));
  log_int32(logger, DEFAULT_MQTT_SN_CLIENT_BIND_PORT);
  log_str(logger, PSTR(".\n"));

  log_str(logger,
          PSTR(" -cL : specify protocol, broadcast address, port as a URL in the form: protocol://address[:port]\n"));

  log_str(logger, PSTR(" -cst : specify the client network send timeout in ms.\n"));
  log_str(logger, PSTR("        On 0 returns immediately, on -1 waits indefinitely for a message. Defaults to "));
  log_int32(logger, CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT);
  log_str(logger, PSTR(" ms.\n"));

  log_str(logger, PSTR(" -crt : specify the client network receive timeout in ms.\n"));
  log_str(logger, PSTR("        On 0 returns immediately, on -1 waits indefinitely for a message. Defaults to "));
  log_int32(logger, CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT);
  log_str(logger, PSTR(" ms.\n"));

  log_str(logger, PSTR(" -ct  : specify the client network send and receive timeout in ms.\n"));

#if defined(WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST) || defined(WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST)
  log_str(logger, PSTR(" -cbP : specify the protocol of the client network broadcast to use.\n"));
  log_str(logger, PSTR("        Can be udp. Defaults to udp.\n"));
  log_str(logger, PSTR(" -cbA : specify the client network broadcast address. Defaults to "));
  log_str(logger, DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR("        Note: the client network broadcast binds to the client network bind interface.\n"));
  log_str(logger, PSTR(" -cbp : listening on the specific client network broadcast port. Defaults to "));
  log_int32(logger, DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" -cbL : specify protocol, address, port as a URL in the form: protocol://address[:port]\n"));
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
  log_str(logger, PSTR(" -cnp : absolute path to the client network plugin.\n"));
  log_str(logger,
          PSTR("        The client network protocol must match the short protocol name gained from the plugin.\n"));
  log_str(logger,
          PSTR("        A client bind address is saved to a device address and given to the gateway network plugin.\n"));
  log_str(logger, PSTR("        A client port overwrites the two last bytes of the gateway network bind address. \n"));
  log_str(logger, PSTR("        You can set client port to -1 if you want to overwrite this behaviour.\n"));
#endif
}
int32_t client_network_config_file_process_command_callback(void *cfg,
                                                            const MqttSnLogger *logger,
                                                            int argc,
                                                            char **argv) {
  client_network_config *cncfg = (client_network_config *) cfg;
  return client_network_config_process_args(cncfg, logger, argc, argv);
}
