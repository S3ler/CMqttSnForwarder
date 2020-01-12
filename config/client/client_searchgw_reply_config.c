//
// Created by SomeDude on 05.09.2019.
//
#include "client_searchgw_reply_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>
int32_t client_search_gw_reply_config_init(client_search_gw_reply_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->search_gw_reply_enabled = DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_SEARCH_GW_REPLY_ENDABLED;
    cfg->gw_info_delay_ms        = DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_GW_INFO_DELAY_MS;
    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_search_gw_reply_config_cleanup(client_search_gw_reply_config *cfg) {
    // nothing to do
}
int32_t is_client_search_gw_reply_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-sre") || !strcmp(arg, "--searchgw_reply_enabled")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-gwd") || !strcmp(arg, "--gwinfo_delay")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_search_gw_reply_config_process_args(client_search_gw_reply_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int32_t i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-sre") || !strcmp(argv[i], "--searchgw_reply_enabled")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "searchgw_reply_enabled");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_searchgw_reply_enabled_boolean(logger, argv[i + 1], &cfg->search_gw_reply_enabled) ==
                    MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-gwd") || !strcmp(argv[i], "--gwinfo_delay")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "gwinfo delay");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_timeout_uint16(logger, argv[i + 1], &cfg->gw_info_delay_ms) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
void client_search_gw_reply_config_parameter_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-sre --searchgw_reply_enabled] [-gwd --gwinfo_delay]\n"));
}
void client_search_gw_reply_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -sre : specify search reply enabled. Can be true or false. Default is "));
    DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_SEARCH_GW_REPLY_ENDABLED ? log_str(logger, PSTR("true")) : log_str(logger, PSTR("false"));
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -gwd : specify gwinfo delay in ms. Default is "));
    log_int32(logger, DEFAULT_CLIENT_SEARCHGW_REPLY_CONFIG_GW_INFO_DELAY_MS);
    log_str(logger, PSTR(" ms.\n"));
}
