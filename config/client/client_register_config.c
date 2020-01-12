//
// Created by SomeDude on 28.05.2019.
//

#include "client_register_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>

int32_t client_register_config_init(client_register_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));

    cfg->registration_list_max_len = CLIENT_MQTT_SN_MAX_REGISTRATIONS;
    cfg->registration_list_len     = 0;
    cfg->register_clear            = 0;

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_register_config_cleanup(client_register_config *cfg) {
    // nothing to do here
}
int32_t is_client_register_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-rt") || !strcmp(arg, "--register_topic")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-rc") || !strcmp(arg, "--register_clean")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_register_config_process_args(client_register_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-rt") || !strcmp(argv[i], "--register_topic")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "topic name");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->registration_list_len < cfg->registration_list_max_len) {
                    cfg->registration_list[cfg->registration_list_len++].topic_name = argv[i + 1];
                } else {
                    print_config_list_full(logger, argv[i], cfg->registration_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-rc") || !strcmp(argv[i], "--register_clear")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "register clear");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_clear_boolean(logger, argv[i + 1], &cfg->register_clear) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                if (cfg->register_clear) {
                    cfg->registration_list_len = 0;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
void client_register_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-rt --register_topic] [-rc --register_clear]\n"));
}
void client_register_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -rt : add register topic to the registration list. Can be used repeatedly. Maximum registrations "));
    log_uint64(logger, CLIENT_MQTT_SN_MAX_REGISTRATIONS);
    log_str(logger, PSTR(".\n"));
    log_str(logger, PSTR(" -rc : register clear can be true or false. If true the registration list is cleared.\n"));
}
int32_t client_register_config_copy_to_buffer(client_register_config *cfg, client_register_config_buffer *cfg_buffer) {
    for (uint32_t i = 0; i < cfg->registration_list_max_len; i++) {
        if (cfg->registration_list[i].topic_name) {
            if (strlen(cfg->registration_list[i].topic_name) + 1 > CLIENT_MQTT_SN_REGISTRATION_TOPIC_NAME_LENGTH) {
                return -1;
            }
            strcpy(cfg_buffer->registration_list[i].topic_name, cfg->registration_list[i].topic_name);
            cfg->registration_list[i].topic_name = cfg_buffer->registration_list[i].topic_name;
        }
    }
    return 0;
}
