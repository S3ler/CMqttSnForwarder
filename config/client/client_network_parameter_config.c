//
// Created by SomeDude on 27.08.2019.
//

#include "client_network_parameter_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>

int32_t client_network_parameter_config_init(client_network_parameter_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->retry_timeout_ms        = DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_TIMEOUT_MS;
    cfg->retry_count             = DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_COUNT;
    cfg->congestion_delay_s      = DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_CONGESTION_DELAY_S;
    cfg->default_signal_strength = DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_SIGNAL_STRENGTH;
    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_network_parameter_config_cleanup(client_network_parameter_config *cfg) {
    // nothing to do
}
int32_t is_client_network_parameter_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-nrt") || !strcmp(arg, "--network_retry_timeout")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-nrc") || !strcmp(arg, "--network_retry_count")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-ncd") || !strcmp(arg, "--network_congestion_delay")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-nss") || !strcmp(arg, "--network_signal_strength")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_network_parameter_config_process_args(client_network_parameter_config *cfg, const MqttSnLogger *logger, int argc,
                                                     char **argv) {
    int32_t parsed_args = 0;
    for (int32_t i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-nrt") || !strcmp(argv[i], "--network_retry_timeout")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "network retry timeout");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_timeout(logger, argv[i + 1], &cfg->retry_timeout_ms) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-nrc") || !strcmp(argv[i], "--network_retry_count")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "network retry count");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_retry_count(logger, argv[i + 1], &cfg->retry_count) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-ncd") || !strcmp(argv[i], "--network_congestion_delay")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "network congestion delay");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_timeout_uint64(logger, argv[i + 1], &cfg->congestion_delay_s) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-nss") || !strcmp(argv[i], "--network_signal_strength")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "network signal strength");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_default_signal_strength(logger, argv[i + 1], &cfg->default_signal_strength) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
void client_network_config_parameter_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-nrt --network_retry_timeout] [-nrc --network_retry_count] [-ncd --network_congestion_delay] "
                         "[-nss --network_signal_strength]\n"));
}
void client_network_config_parameter_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -nrt : specify network retry timeout in ms. Default is "));
    log_int32(logger, DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_TIMEOUT_MS);
    log_str(logger, PSTR(" ms.\n"));

    log_str(logger, PSTR(" -nrc : specify network retry count. Default is "));
    log_int32(logger, DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_RETRY_COUNT);
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -ncd : specify network congestion delay in s. Default is "));
    log_int32(logger, DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_CONGESTION_DELAY_S);
    log_str(logger, PSTR(" s.\n"));

    log_str(logger, PSTR(" -nss : specify network signal strength between 0 and 255. Default is "));
    log_int32(logger, DEFAULT_CLIENT_NETWORK_CONFIG_PARAMETER_SIGNAL_STRENGTH);
    log_str(logger, PSTR(".\n"));
}
