//
// Created by SomeDude on 28.05.2019.
//

#include "client_find_mqtt_sn_gateway_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <stdlib.h>

int32_t client_find_mqtt_sn_gateway_config_init(client_find_mqtt_sn_gateway_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));

    cfg->find_pattern_type        = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN;
    cfg->advertise_wait_timeout_s = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_TIMEOUT;

    cfg->search_gateway_wait_timeout_ms = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_WAIT_TIMEOUT;
    cfg->search_gateway_radius          = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_RADIUS;
    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}

void client_find_mqtt_sn_gateway_config_cleanup(client_find_mqtt_sn_gateway_config *cfg) {
    // nothing to do yet
}

int32_t is_client_find_mqtt_sn_gateway_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-gfp") || !strcmp(arg, "--gateway_find_pattern")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-awt") || !strcmp(arg, "--advertise_wait_timeout")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-sgt") || !strcmp(arg, "--search_gateway_timeout")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-sgr") || !strcmp(arg, "--search_gateway_radius")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_find_mqtt_sn_gateway_config_process_args(client_find_mqtt_sn_gateway_config *cfg, const MqttSnLogger *logger, int argc,
                                                        char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-gfp") || !strcmp(argv[i], "--gateway_find_pattern")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "find pattern");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (!strcmp(argv[i + 1], "any")) {
                    cfg->find_pattern_type = ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
                } else if (!strcmp(argv[i + 1], "advertise")) {
                    cfg->find_pattern_type = ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
                } else if (!strcmp(argv[i + 1], "search")) {
                    cfg->find_pattern_type = SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
                } else if (!strcmp(argv[i + 1], "connect")) {
                    cfg->find_pattern_type = CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
                } else {
                    print_argument_value_not_specified(logger, argv[i], "valid pattern type");
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-awt") || !strcmp(argv[i], "--advertise_wait_timeout")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "advertise wait timeout");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                parse_timeout(logger, argv[i + 1], &cfg->advertise_wait_timeout_s);
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-sgt") || !strcmp(argv[i], "--search_gateway_timeout")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "search gateway timeout");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                parse_timeout(logger, argv[i + 1], &cfg->search_gateway_wait_timeout_ms);
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-sgr") || !strcmp(argv[i], "--search_gateway_radius")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "search gateway radius");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                parse_radius(logger, argv[i + 1], &cfg->search_gateway_radius);
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
void client_find_mqtt_sn_gateway_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-gfp --gateway_find_pattern] [-awt --advertise_wait_timeout] [-sgt --search_gateway_timeout} [-sgr --search_gateway_radius]\n"));
}
void client_find_mqtt_sn_gateway_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -gfp : specify the mqtt-sn gateway find pattern.\n"));
    log_str(logger, PSTR("        Can be any, advertise, search, connect. Default is "));
    if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN == ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
        log_str(logger, PSTR("advertise"));
    } else if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN == SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
        log_str(logger, PSTR("search"));
    } else if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN == CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
        log_str(logger, PSTR("connect"));
    } else if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN == ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
        log_str(logger, PSTR("any"));
    } else {
        log_str(logger, PSTR("undefined"));
    }
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR(" -awt : specify advertise wait timeout in s. Default is "));
    log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_TIMEOUT);
    log_str(logger, PSTR(" s.\n"));
    log_str(logger, PSTR(" -sgt : specify SearchGw timeout in ms. Default is "));
    log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_WAIT_TIMEOUT);
    log_str(logger, PSTR(" ms.\n"));
    log_str(logger, PSTR(" -sgr : specify radius of SearchGw message between 0 and 255. Default is "));
    log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_RADIUS);
    log_str(logger, PSTR(".\n"));
}
