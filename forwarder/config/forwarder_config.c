//
// Created by SomeDude on 31.05.2019.
//

#include <platform/platform_compatibility.h>
#include <platform/MqttSnMessageData.h>
#include <config/common/config_command_helper.h>
#include "forwarder_config.h"

int32_t forwarder_config_init(forwarder_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));

    memcpy(&cfg->executable_name, MQTT_SN_FORWARDER_EXECUTABLE_NAME, sizeof(MQTT_SN_FORWARDER_EXECUTABLE_NAME));

    mqtt_sn_version_config_init(&cfg->msvcfg);
    mqtt_sn_gateway_config_init(&cfg->msgcfg);
    client_network_config_init(&cfg->cncfg);
    gateway_network_config_init(&cfg->gncfg);
    mqtt_sn_logger_config_init(&cfg->mslcfg);
    cfg->connection_state = MQTT_SN_FORWARDER_START_CONNECTION_STATE_DEFAULT_STATE;
    cfg->pin_connection   = MQTT_SN_FORWARDER_PIN_CONNECTION_DEFAULT_VALUE;

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}

void forwarder_config_cleanup(forwarder_config *cfg) {
    mqtt_sn_version_config_cleanup(&cfg->msvcfg);
    mqtt_sn_gateway_config_cleanup(&cfg->msgcfg);
    client_network_config_cleanup(&cfg->cncfg);
    gateway_network_config_cleanup(&cfg->gncfg);
    mqtt_sn_logger_config_cleanup(&cfg->mslcfg);
}

int32_t forwarder_config_process_line(forwarder_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    if (contains_config_argument_help(argc, argv)) {
        return MQTT_SN_PARSE_CONFIG_HELP;
    }

    int32_t config_file_rc    = 0;
    int32_t config_file_index = get_config_argument_file_index(argc, argv, logger);
    if (config_file_index == MQTT_SN_PARSE_CONFIG_FAILURE) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    if (config_file_index) {
        const char *config_file_path = argv[config_file_index + 1];
        if (process_config_file(config_file_path, logger, cfg, forwarder_config_file_process_command_callback, argv[0]) ==
            MQTT_SN_PARSE_CONFIG_FAILURE) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
        config_file_rc = 2;
    }

    int32_t msgcfg_rc = mqtt_sn_gateway_config_process_args(&cfg->msgcfg, logger, argc, argv);
    int32_t cnfg_rc   = client_network_config_process_args(&cfg->cncfg, logger, argc, argv);
    int32_t gnfg_rc   = gateway_network_config_process_args(&cfg->gncfg, logger, argc, argv);
    int32_t mslcfg_rc = mqtt_sn_logger_config_process_args(&cfg->mslcfg, logger, argc, argv);
    int32_t fcfg_rc   = forwarder_config_process_args(cfg, logger, argc, argv);

    if (msgcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE || cnfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE || gnfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE ||
        mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE || fcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (config_file_rc + msgcfg_rc + cnfg_rc + gnfg_rc + mslcfg_rc + fcfg_rc != (argc - 1)) {
        // there is an unknown option
        for (int i = 1; i < argc; i++) {
            if (!is_mqtt_sn_gateway_config_command(argv[i], &i) && !is_client_network_config_command(argv[i], &i) &&
                !is_gateway_network_config_command(argv[i], &i) && !is_mqtt_sn_logger_config_command(argv[i], &i) &&
                !is_forwarder_config_command(argv[i], &i)) {
                print_unknown_option(logger, argv[i]);
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            }
        }
    }

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t is_forwarder_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-fis") || !strcmp(arg, "--forwarder_init_state")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-fpc") || !strcmp(arg, "--forwarder_pin_connection")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t forwarder_config_process_args(forwarder_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-fis") || !strcmp(argv[i], "--forwarder_init_state")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "forwarder init state");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (!strcmp(argv[i + 1], "connected")) {
                    cfg->connection_state = CONNECTED_CONFIG_MQTT_SN_FORWARDER_START_CONNECTION_STATE;
                } else if (!strcmp(argv[i + 1], "disconnected")) {
                    cfg->connection_state = DISCONNECTED_CONFIG_MQTT_SN_FORWARDER_START_CONNECTION_STATE;
                } else {
                    print_argument_value_not_specified(logger, argv[i], "valid forwarder init state");
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-fpc") || !strcmp(argv[i], "--forwarder_pin_connection")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "forwarder pin connection");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_forwarder_pin_connection_boolean(logger, argv[i + 1], &cfg->pin_connection) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
void forwarder_config_print_usage(const MqttSnLogger *logger) {
    log_str(logger, PSTR(MQTT_SN_FORWARDER_EXECUTABLE_NAME));
    log_str(logger, PSTR(" is a simple mqtt-sn forwarder that will forward mqtt-sn messages\n"));
    log_str(logger, PSTR("                 from a client network over a gateway network to a mqtt-sn gateway.\n"));
    mqtt_sn_version_config_print_usage_short(logger);
    forwarder_config_print_usage_short(logger);
    log_flush(logger);
    log_flush(logger);
    forwarder_config_print_usage_long(logger);
    print_show_help(logger);
}
void forwarder_config_print_usage_short(const MqttSnLogger *logger) {
    const char *indent = "                        ";
    log_str(logger, PSTR("Usage: cmqttsnforwarder "));
    mqtt_sn_gateway_config_print_usage_short(logger, NULL);
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("{[-fis --forwarder_init_state] [-fpc --forwarder_pin_connection]}\n"));
    gateway_network_config_print_usage_short(logger, indent);
    client_network_config_print_usage_short(logger, indent);
    mqtt_sn_logger_config_print_usage_short(logger, indent);
}
void forwarder_config_print_usage_long(const MqttSnLogger *logger) {
    mqtt_sn_gateway_config_print_usage_long(logger);
    log_str(logger, PSTR(" -fis : specify the mqtt-sn forwarder init state.\n"));
    log_str(logger, PSTR("        Can be connected or disconnect. Default is "));
    if (MQTT_SN_FORWARDER_START_CONNECTION_STATE_DEFAULT_STATE == CONNECTED_CONFIG_MQTT_SN_FORWARDER_START_CONNECTION_STATE) {
        log_str(logger, PSTR("connected"));
    } else if (MQTT_SN_FORWARDER_START_CONNECTION_STATE_DEFAULT_STATE == DISCONNECTED_CONFIG_MQTT_SN_FORWARDER_START_CONNECTION_STATE) {
        log_str(logger, PSTR("disconnect"));
    } else {
        log_str(logger, PSTR("undefined"));
    }
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR(" -fpc : specify if the mqtt-sn gateway address is fix. Can be true or false.\n"));
    log_str(logger, PSTR("        Default is "));
    MQTT_SN_FORWARDER_PIN_CONNECTION_DEFAULT_VALUE ? log_str(logger, PSTR("true")) : log_str(logger, PSTR("false"));
    log_str(logger, PSTR(".\n"));

    gateway_network_config_print_usage_long(logger);
    client_network_config_print_usage_long(logger);
    mqtt_sn_logger_config_print_usage_long(logger);
}
void forwarder_config_print_see_usage(const MqttSnLogger *logger) {
    const char *executable_name = MQTT_SN_FORWARDER_EXECUTABLE_NAME;
    print_see_usage(logger, executable_name);
}
#ifdef WITH_MQTT_SN_FORWARDER_CONFIG_CONTAINER
int32_t forwarder_config_copy_to_container(forwarder_config *fcfg) {

    if (mqtt_sn_gateway_config_buffer_copy_to_buffer(&fcfg->msgcfg, &fcfg->msgcfgb) < 0) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    if (gateway_network_config_copy_to_buffer(&fcfg->gncfg, &fcfg->gncfgb) < 0) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    if (client_network_config_copy_to_buffer(&fcfg->cncfg, &fcfg->cncfgb) < 0) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    if (mqtt_sn_logger_config_buffer_copy_to_buffer(&fcfg->mslcfg, &fcfg->mslcfgb) < 0) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t forwarder_config_file_process_command_callback(void *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    forwarder_config *fcfg = (forwarder_config *)cfg;
    int32_t           rc   = forwarder_config_process_line(fcfg, logger, argc, argv);
    if (rc == MQTT_SN_PARSE_CONFIG_SUCCESS) {
        rc = forwarder_config_copy_to_container(fcfg);
    }
    return rc;
}
#else
int32_t forwarder_config_file_process_command_callback(void *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    forwarder_config *fcfg = (forwarder_config *)cfg;
    return forwarder_config_process_line(fcfg, logger, argc, argv);
}
#endif
