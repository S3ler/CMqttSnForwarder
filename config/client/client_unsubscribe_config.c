//
// Created by SomeDude on 06.08.2019.
//

#include "client_unsubscribe_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>

int32_t parse_client_subscribe_config(const MqttSnLogger *logger, char *url, client_unsubscription_topic *cfg);

int32_t client_unsubscribe_config_init(client_unsubscribe_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->unsubscribe_list_max_len = CLIENT_MQTT_SN_MAX_UNSUBSCRIBE;
    cfg->unsubscribe_list_len     = 0;
    return 0;
}
void client_unsubscribe_config_cleanup(client_unsubscribe_config *cfg) {
    // nothing to do here
}
int32_t is_client_unsubscribe_config_command(char *arg, int *i) {
    if (!strcmp(arg, "-utn") || !strcmp(arg, "--unsubscribe_topic_name")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-utp") || !strcmp(arg, "--unsubscribe_predefined_topic_id")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-uti") || !strcmp(arg, "--unsubscribe_topic_id")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-uL") || !strcmp(arg, "--unsubscribe_URL")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-uc") || !strcmp(arg, "--unsubscribe_clear")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_unsubscribe_config_process_args(client_unsubscribe_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-utn") || !strcmp(argv[i], "--unsubscribe_topic_name")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "unsubscribe topic name");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->unsubscribe_list_len < cfg->unsubscribe_list_max_len) {
                    cfg->unsubscribe_list[cfg->unsubscribe_list_len].topic_id_type = CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME;
                    cfg->unsubscribe_list[cfg->unsubscribe_list_len].topic_name    = argv[i + 1];
                    cfg->unsubscribe_list_len += 1;
                } else {
                    print_config_list_full(logger, argv[i], cfg->unsubscribe_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-utp") || !strcmp(argv[i], "--unsubscribe_predefined_topic_id")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "unsubscribe predefined topic id");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->unsubscribe_list_len < cfg->unsubscribe_list_max_len) {
                    cfg->unsubscribe_list[cfg->unsubscribe_list_len].topic_id_type = CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED;
                    if (parse_topic_id(logger, argv[i + 1], &cfg->unsubscribe_list[cfg->unsubscribe_list_len].topic_id) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                    cfg->unsubscribe_list_len += 1;
                } else {
                    print_config_list_full(logger, argv[i], cfg->unsubscribe_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-uti") || !strcmp(argv[i], "--unsubscribe_topic_id")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "unsubscribe topic id");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->unsubscribe_list_len < cfg->unsubscribe_list_max_len) {
                    cfg->unsubscribe_list[cfg->unsubscribe_list_len].topic_id_type = CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICID;
                    if (parse_topic_id(logger, argv[i + 1], &cfg->unsubscribe_list[cfg->unsubscribe_list_len].topic_id) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                    cfg->unsubscribe_list_len += 1;
                } else {
                    print_config_list_full(logger, argv[i], cfg->unsubscribe_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-uL") || !strcmp(argv[i], "--unsubscribe_URL")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "unsubscribe URL");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->unsubscribe_list_len < cfg->unsubscribe_list_max_len) {
                    if (parse_client_subscribe_config(logger, argv[i + 1], &cfg->unsubscribe_list[cfg->unsubscribe_list_len++]) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        print_unsupported_url_scheme(logger);
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], cfg->unsubscribe_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-uc") || !strcmp(argv[i], "--unsubscribe_clear")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "unsubscribe clear");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_clear_boolean(logger, argv[i + 1], &cfg->unsubscribe_clear) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                if (cfg->unsubscribe_clear) {
                    cfg->unsubscribe_list_len = 0;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
int32_t parse_client_subscribe_config(const MqttSnLogger *logger, char *url, client_unsubscription_topic *cfg) {
    char *topic_id_type_str    = strtok(url, "://");
    char *topic_name_or_id_str = strtok(NULL, ":");
    char *null_token           = strtok(NULL, "://");

    if (topic_id_type_str) {
        if (!strcmp(topic_id_type_str, "topicname")) {
            cfg->topic_id_type = CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME;
        } else if (!strcmp(topic_id_type_str, "predefined")) {
            cfg->topic_id_type = CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED;
        } else if (!strcmp(topic_id_type_str, "topicid")) {
            cfg->topic_id_type = CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICID;
        } else {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (topic_name_or_id_str) {
        if (cfg->topic_id_type == CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME) {
            cfg->topic_name = topic_name_or_id_str;
        } else if (cfg->topic_id_type == CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED ||
                   cfg->topic_id_type == CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICID) {
            char *topic_name_id_str = strtok(topic_name_or_id_str, "//");
            if (parse_topic_id(logger, topic_name_id_str, &cfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            }
        } else {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (null_token) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_unsubscribe_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("{[-utn --unsubscribe_topic_name] [-utp ---unsubscribe_predefined_topic_id]"
                         " [-uti --unsubscribe_topic_id] [-uL --unsubscribe_URL] [-uc --unsubscribe_clear]}\n"));
    //log_str(logger, PSTR("{[-utn --unsubscribe_topic_name] [-utp ---unsubscribe_predefined_topic_id] "
    //                     "[-uL --unsubscribe_URL] [-uc --unsubscribe_clear]}\n"));
}
void client_unsubscribe_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -utn : add a unsubscription by topic name. Sets the topic id type to topicname.\n"));
    log_str(logger, PSTR(" -utp : add a unsubscription by predefined topic id. Sets the topic id type to predefined.\n"));
    log_str(logger, PSTR(" -uti : add a unsubscription by topic id. Sets the topic id type to short.\n"));

    log_str(logger, PSTR(" -uL : unsubscribe URL schema: <topic_id_type>://<topic_id|topic_name>.\n"));
    log_str(logger, PSTR("       <topic_id_type> can be: topicname, predefined, or short.\n"));
    log_str(logger, PSTR("       <topic_id|topic_name> can be: string for topic_name, topic_id for predefined and short.\n"));

    log_str(logger, PSTR(" -uc : unsubscribe clear can be true or false. If true the unsubscribe list is cleared.\n"));
}
int32_t client_unsubscribe_config_copy_to_buffer(client_unsubscribe_config *cfg, client_unsubscribe_config_buffer *cfg_buffer) {
    for (uint32_t i = 0; i < cfg->unsubscribe_list_max_len; i++) {
        if (cfg->unsubscribe_list[i].topic_name) {
            if (strlen(cfg->unsubscribe_list[i].topic_name) + 1 > CLIENT_MQTT_SN_UNSUBSCRIBE_TOPIC_NAME_LENGTH) {
                return -1;
            }
            strcpy(cfg_buffer->unsubscribe_list[i].topic_name, cfg->unsubscribe_list[i].topic_name);
            cfg->unsubscribe_list[i].topic_name = cfg_buffer->unsubscribe_list[i].topic_name;
        }
    }
    return 0;
}
