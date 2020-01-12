//
// Created by SomeDude on 27.05.2019.
//

#include "gateway_network_config.h"
#include <platform/platform_compatibility.h>
#include <string.h>
#include <stdlib.h>
#include <config/common/config_command_helper.h>

int32_t gateway_network_config_init(gateway_network_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    memcpy(cfg->default_gateway_network_protocol, DEFAULT_MQTT_SN_GATEWAY_PROTOCOL, sizeof(DEFAULT_MQTT_SN_GATEWAY_PROTOCOL));
    cfg->gateway_network_protocol  = cfg->default_gateway_network_protocol;
    cfg->gateway_network_bind_port = DEFAULT_MQTT_SN_GATEWAY_BIND_PORT;
#ifdef WITH_GATEWAY_NETWORK_BROADCAST
    cfg->gateway_network_broadcast_protocol = cfg->default_gateway_network_protocol;
#endif
    memcpy(cfg->default_gateway_network_broadcast_address, DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS,
           sizeof(DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS));
    cfg->gateway_network_broadcast_address   = cfg->default_gateway_network_broadcast_address;
    cfg->gateway_network_broadcast_bind_port = DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT;

#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    cfg->gw_id_whitelist_len   = 0;
    cfg->gw_addr_whitelist_len = 0;
    cfg->gw_clear_whitelist    = 0;
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
    cfg->gw_id_blacklist_len     = 0;
    cfg->gw_addr_blacklist_len     = 0;
    cfg->gw_clear_blacklist        = 0;
#endif
    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}

void gateway_network_config_cleanup(gateway_network_config *cfg) {
    // nothing to do here
}
int32_t gateway_network_config_process_args(gateway_network_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-gP") || !strcmp(argv[i], "--gateway_network_protocol")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "protocol");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                cfg->gateway_network_protocol = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gA") || !strcmp(argv[i], "--gateway_network_bind_address")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "address");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                cfg->gateway_network_bind_address = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gp") || !strcmp(argv[i], "--gateway_network_bind_port")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "port");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_port(logger, argv[i + 1], &cfg->gateway_network_bind_port)) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gL") || !strcmp(argv[i], "--gateway_network_url")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "URL");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                char *url = argv[i + 1];

                char *prot       = strtok(url, "://");
                char *addr       = strtok(NULL, "://");
                char *port       = strtok(NULL, ":");
                char *null_token = strtok(NULL, "://");

                if (prot) {
                    cfg->gateway_network_protocol = prot;
                } else {
                    print_unsupported_url_scheme(logger);
                }

                if (addr) {
                    cfg->gateway_network_bind_address = addr;
                } else {
                    print_unsupported_url_scheme(logger);
                }

                if (port) {
                    if (parse_port(logger, port, &cfg->gateway_network_bind_port)) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                }

                if (null_token) {
                    print_unsupported_url_scheme(logger);
                }
            }
            i++;
            parsed_args += 2;
            // timeout
        } else if (!strcmp(argv[i], "-gst") || !strcmp(argv[i], "--gateway_send_timeout")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "timeout");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_timeout(logger, argv[i + 1], &cfg->gateway_network_send_timeout)) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-grt") || !strcmp(argv[i], "--gateway_receive_timeout")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "timeout");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_timeout(logger, argv[i + 1], &cfg->gateway_network_receive_timeout)) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gt") || !strcmp(argv[i], "--gateway_network_timeout")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "timeout");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_timeout(logger, argv[i + 1], &cfg->gateway_network_send_timeout)) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                if (parse_timeout(logger, argv[i + 1], &cfg->gateway_network_receive_timeout)) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        }
        // broadcast
#ifdef WITH_GATEWAY_NETWORK_BROADCAST
        else if (!strcmp(argv[i], "-gbP") || !strcmp(argv[i], "--gateway_network_broadcast_protocol")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "protocol");
                return 1;
            } else {
                cfg->gateway_network_broadcast_protocol = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gbA") || !strcmp(argv[i], "--gateway_network_broadcast_address")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "address");
                return 1;
            } else {
                cfg->gateway_network_broadcast_address = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gbp") || !strcmp(argv[i], "--gateway_network_broadcast_bind_port")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "port");
                return 1;
            } else {
                if (parse_port(logger, argv[i + 1], &cfg->gateway_network_broadcast_bind_port)) {
                    return 1;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gbL") || !strcmp(argv[i], "--gateway_network_broadcast_url")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "URL");
                return 1;
            } else {
                char *url = argv[i + 1];

                char *prot       = strtok(url, "://");
                char *addr       = strtok(NULL, "://");
                char *port       = strtok(NULL, ":");
                char *null_token = strtok(NULL, "://");

                if (prot) {
                    cfg->gateway_network_broadcast_protocol = prot;
                } else {
                    print_unsupported_url_scheme(logger);
                }

                if (addr) {
                    cfg->gateway_network_broadcast_address = addr;
                } else {
                    print_unsupported_url_scheme(logger);
                }

                if (port) {
                    if (parse_port(logger, port, &cfg->gateway_network_broadcast_bind_port)) {
                        return 1;
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
        // plugin
#ifdef WITH_LINUX_PLUGIN_NETWORK
        else if (!strcmp(argv[i], "-gnp") || !strcmp(argv[i], "--gateway_network_plugin")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "path");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                cfg->gateway_network_plugin_path = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        }
#endif
        // whitelist
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
        else if (!strcmp(argv[i], "-giw") || !strcmp(argv[i], "--gw_id_whitelist")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "gw_id whitelist");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->gw_id_whitelist_len < WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_WHITELIST_CONFIG_LENGTH) {
                    if (parse_gateway_id(logger, argv[i + 1], &cfg->gw_id_whitelist[cfg->gw_id_whitelist_len++]) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_WHITELIST_CONFIG_LENGTH);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gaw") || !strcmp(argv[i], "--gw_addr_whitelist")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "gw_add whitelist");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->gw_addr_whitelist_len < WITH_MQTT_SN_GATEWAY_NETWORK_ADD_WHITELIST_CONFIG_LENGTH) {
                    if (parse_device_address_hex(logger, argv[i + 1], &cfg->gw_addr_whitelist[cfg->gw_addr_whitelist_len++]) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], WITH_MQTT_SN_GATEWAY_NETWORK_ADD_WHITELIST_CONFIG_LENGTH);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gcw") || !strcmp(argv[i], "--gw_clear_whitelist")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "gw clear whitelist");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_clear_boolean(logger, argv[i + 1], &cfg->gw_clear_whitelist) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                if (cfg->gw_clear_whitelist) {
                    cfg->gw_id_whitelist_len   = 0;
                    cfg->gw_addr_whitelist_len = 0;
                }
            }
            i++;
            parsed_args += 2;
        }
#endif
        // blacklist
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
        else if (!strcmp(argv[i], "-gib") || !strcmp(argv[i], "--gw_id_blacklist")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "gw_id blacklist");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->gw_id_blacklist_len < WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_BLACKLIST_CONFIG_LENGTH) {
                    if (parse_gateway_id(logger, argv[i + 1], &cfg->gw_id_blacklist[cfg->gw_id_blacklist_len++]) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_BLACKLIST_CONFIG_LENGTH);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gab") || !strcmp(argv[i], "--gw_addr_blacklist")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "gw_add blacklist");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->gw_addr_blacklist_len < WITH_MQTT_SN_GATEWAY_NETWORK_ADD_BLACKLIST_CONFIG_LENGTH) {
                    if (parse_device_address_hex(logger, argv[i + 1], &cfg->gw_addr_blacklist[cfg->gw_addr_blacklist_len++]) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], WITH_MQTT_SN_GATEWAY_NETWORK_ADD_BLACKLIST_CONFIG_LENGTH);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gcb") || !strcmp(argv[i], "--gw_clear_blacklist")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "gw clear blacklist");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_clear_boolean(logger, argv[i + 1], &cfg->gw_clear_blacklist) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                if (cfg->gw_clear_blacklist) {
                    cfg->gw_id_blacklist_len   = 0;
                    cfg->gw_addr_blacklist_len = 0;
                }
            }
            i++;
            parsed_args += 2;
        }
#endif
    }
    return parsed_args;
}
void gateway_network_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("{[[-gP gateway_network_protocol] [-gA gateway_network_bind_address]"
                         " [-gp gateway_network_bind_port]] | -gL URL}\n"));
#ifdef WITH_GATEWAY_NETWORK_BROADCAST
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("{[[-gbP gateway_network_broadcast_protocol] [-gbA gateway_network_broadcast_address]"
                         " [-gbp gateway_network_broadcast_bind_port]] | -gbL URL}\n"));
#endif
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("{[[-gst gateway_send_timeout] [-grt gateway_receive_timeout]]"
                         " | --gt gateway_network_timeout}\n"));
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-gnp gateway_network_plugin]\n"));
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-giw --gw_id_whitelist] [-gaw --gw_addr_whitelist] [-gcw --gw_clear_whitelist]\n"));
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-gib --gw_id_blacklist] [-gab --gw_addr_blacklist] [-gcb --gw_clear_blacklist]\n"));
#endif
}
void gateway_network_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -gP : specify the protocol of the gateway network to use.\n"));
#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK) && defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
    log_str(logger, PSTR("       Can be udp, tcp. Defaults to udp.\n"));
#elif defined(WITH_LINUX_UDP_GATEWAY_NETWORK)
    log_str(logger, PSTR("       Can be udp. Defaults to udp.\n"));
#elif defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
    log_str(logger, PSTR("       Can be tcp. Defaults to tcp.\n"));
#endif
    log_str(logger, PSTR(" -gA : bind the gateway network to the outgoing socket to this host/ip address.\n"));
    log_str(logger, PSTR("       Use to control which interface to communicates over or to set the address for the gateway network.\n"));
    log_str(logger, PSTR(" -gp : listening on the specific gateway network port. Defaults to "));
    log_uint16(logger, DEFAULT_MQTT_SN_GATEWAY_BIND_PORT);
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR("       A port overwrites the two last bytes of the gateway network bind address. "
                         "You can set gateway port to -1 if you want to overwrite this behaviour\n"));
    log_str(logger, PSTR(" -gL : specify protocol, address, port as a URL in the form: protocol://address[:port]\n"));

#ifdef WITH_GATEWAY_NETWORK_BROADCAST
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

    log_str(logger, PSTR(" -gst : specify the gateway network send timeout in ms.\n"));
    log_str(logger, PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to "));
    log_int32(logger, GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT);
    log_str(logger, PSTR(" ms.\n"));

    log_str(logger, PSTR(" -grt : specify the gateway network receive timeout in ms.\n"));
    log_str(logger, PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message. Defaults to "));
    log_int32(logger, GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT);
    log_str(logger, PSTR(" ms.\n"));

    log_str(logger, PSTR(" -gt  : specify the gateway network send and receive timeout in ms.\n"));
    log_str(logger, PSTR("       on 0 returns immediately, on -1 waits indefinitely for a message.\n"));

#ifdef WITH_LINUX_PLUGIN_NETWORK
    log_str(logger, PSTR(" -gnp : path to the gateway network plugin.\n"));
    log_str(logger, PSTR("       The gateway network protocol must match the short protocol name gained from the plugin.\n"));
    log_str(logger, PSTR("       A gateway bind address is saved to a device address and given to the gateway network plugin.\n"));
    log_str(logger, PSTR("       A port overwrites the two last bytes of the gateway network bind address. "
                         "You can set gateway port to -1 if you want to overwrite this behaviour\n"));
#endif

#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    log_str(logger, PSTR(" -giw : specify gateway id for the whitelist. Can be used repeatedly."));
    log_str(logger, PSTR(" Maximum entry count is "));
    log_uint64(logger, WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_WHITELIST_CONFIG_LENGTH);
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR(" -gaw : specify hex encoded (delimiter : ) network address for the whitelist."));
    log_str(logger, PSTR(" Can be used repeatedly. Maximum entry count is "));
    log_uint64(logger, WITH_MQTT_SN_GATEWAY_NETWORK_ADD_WHITELIST_CONFIG_LENGTH);
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR(" -gcw : gateway clear whitelist can be true or false. If true the gateway whitelist is cleared.\n"));
#endif

#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    log_str(logger, PSTR(" -gib : specify gateway id for the blacklist. Can be used repeatedly."));
    log_str(logger, PSTR(" Maximum entry count is "));
    log_uint64(logger, WITH_MQTT_SN_GATEWAY_NETWORK_GW_ID_BLACKLIST_CONFIG_LENGTH);
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR(" -gab : specify hex encoded (delimiter : ) network address for the blacklist."));
    log_str(logger, PSTR(" Can be used repeatedly. Maximum entry count is "));
    log_uint64(logger, WITH_MQTT_SN_GATEWAY_NETWORK_ADD_BLACKLIST_CONFIG_LENGTH);
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR(" -gcb : gateway clear blacklist can be true or false. If true the gateway blacklist is cleared.\n"));
#endif
}
int32_t is_gateway_network_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-gP") || !strcmp(arg, "--gateway_network_protocol")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gA") || !strcmp(arg, "--gateway_network_bind_address")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gp") || !strcmp(arg, "--gateway_network_bind_port")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gL") || !strcmp(arg, "--gateway_network_url")) {
        (*i)++;
        return 1;
        // timeout
    } else if (!strcmp(arg, "-gst") || !strcmp(arg, "--gateway_send_timeout")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-grt") || !strcmp(arg, "--gateway_receive_timeout")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gt") || !strcmp(arg, "--gateway_network_timeout")) {
        (*i)++;
        return 1;
    }
    // broadcast
#ifdef WITH_GATEWAY_NETWORK_BROADCAST
    else if (!strcmp(arg, "-gbP") || !strcmp(arg, "--gateway_network_broadcast_protocol")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gbA") || !strcmp(arg, "--gateway_network_broadcast_address")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gbp") || !strcmp(arg, "--gateway_network_broadcast_bind_port")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gbL") || !strcmp(arg, "--gateway_network_broadcast_url")) {
        (*i)++;
        return 1;
    }
#endif
    // plugin
#ifdef WITH_LINUX_PLUGIN_NETWORK
    else if (!strcmp(arg, "-gnp") || !strcmp(arg, "--gateway_network_plugin")) {
        (*i)++;
        return 1;
    }
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    else if (!strcmp(arg, "-giw") || !strcmp(arg, "--gw_id_whitelist")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gaw") || !strcmp(arg, "--gw_addr_whitelist")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gcw") || !strcmp(arg, "--gw_clear_whitelist")) {
        (*i)++;
        return 1;
    }
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
    else if (!strcmp(arg, "-gib") || !strcmp(arg, "--gw_id_blacklist")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gab") || !strcmp(arg, "--gw_addr_blacklist")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gcb") || !strcmp(arg, "--gw_clear_blacklist")) {
        (*i)++;
        return 1;
    }
#endif
    return 0;
}
int32_t gateway_network_config_copy_to_buffer(gateway_network_config *cfg, gateway_network_config_container *cfg_buffer) {
    if (cfg->gateway_network_protocol) {
        if (strlen(cfg->gateway_network_protocol) + 1 > DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_PROTOCOL_LENGTH) {
            return -1;
        }
        strcpy(cfg_buffer->gateway_network_protocol, cfg->gateway_network_protocol);
        cfg->gateway_network_protocol = cfg_buffer->gateway_network_protocol;
    }
    if (cfg->gateway_network_bind_address) {
        if (strlen(cfg->gateway_network_bind_address) + 1 > DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_NETWORK_BIND_ADDRESS_LENGTH) {
            return -1;
        }
        strcpy(cfg_buffer->gateway_network_bind_address, cfg->gateway_network_bind_address);
        cfg->gateway_network_bind_address = cfg_buffer->gateway_network_bind_address;
    }
#ifdef WITH_GATEWAY_NETWORK_BROADCAST
    if (cfg->gateway_network_broadcast_protocol) {
        if (strlen(cfg->gateway_network_broadcast_protocol) + 1 > DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_PROTOCOL_LENGTH) {
            return -1;
        }
        strcpy(cfg_buffer->gateway_network_broadcast_protocol, cfg->gateway_network_broadcast_protocol);
        cfg->gateway_network_broadcast_protocol = cfg_buffer->gateway_network_broadcast_protocol;
    }
    if (cfg->gateway_network_broadcast_address) {
        if (strlen(cfg->gateway_network_broadcast_address) + 1 > DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_BROADCAST_ADDRESS_LENGTH) {
            return -1;
        }
        strcpy(cfg_buffer->gateway_network_broadcast_address, cfg->gateway_network_broadcast_address);
        cfg->gateway_network_broadcast_address = cfg_buffer->gateway_network_broadcast_address;
    }
#endif
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (cfg->gateway_network_plugin_path) {
        if (strlen(cfg->gateway_network_plugin_path) + 1 > DEFAULT_MQTT_SN_GATEWAY_NETWORK_CONFIG_PLUGIN_PATH_LENGTH) {
            return -1;
        }
        strcpy(cfg_buffer->gateway_network_plugin_path, cfg->gateway_network_plugin_path);
        cfg->gateway_network_plugin_path = cfg_buffer->gateway_network_plugin_path;
    }
#endif
    return 0;
}
