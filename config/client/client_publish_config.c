//
// Created by SomeDude on 28.05.2019.
//

#include "client_publish_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <stdlib.h>
int     parse_client_publish_single_client_publish_config(const MqttSnLogger *logger, char *url, single_client_publish_config *scfg,
                                                          uint8_t is_hex);
int     parse_client_publish_publish_topic_id_type(const MqttSnLogger *logger, const char *topic_id_type_str, uint8_t *topic_id_type);
int32_t client_publish_config_init(client_publish_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));

    cfg->qos                  = CLIENT_MQTT_SN_PUBLISH_DEFAULT_QOS;
    cfg->topic_id_type        = CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID_TYPE;
    cfg->topic_id             = CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID;
    cfg->retain               = CLIENT_MQTT_SN_PUBLISH_DEFAULT_RETAIN;
    char default_topic_name[] = CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_NAME;
    memcpy(cfg->default_topic_name, default_topic_name, sizeof(default_topic_name));
    cfg->topic_name           = cfg->default_topic_name;
    cfg->publish_list_max_len = CLIENT_MQTT_SN_MAX_PUBLISH;
    cfg->publish_list_len     = 0;
    cfg->publish_clear        = 0;

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_publish_config_cleanup(client_publish_config *cfg) {
    // nothing to do
}
int32_t is_client_publish_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-pq") || !strcmp(arg, "--publish_qos")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-ptit") || !strcmp(arg, "--publish_topic_id_type")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-ptn") || !strcmp(arg, "--publish_topic_name")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-ppi") || !strcmp(arg, "--publish_predefined_topic_id")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-pti") || !strcmp(arg, "--publish_topic_id")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-pri") || !strcmp(arg, "--publish_registration_id")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-pr") || !strcmp(arg, "--publish_retain")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-pm") || !strcmp(arg, "--publish_message")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-pmL") || !strcmp(arg, "--publish_message_URL")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-pmhbL") || !strcmp(arg, "--publish_message_hex_byte_URL")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-pc") || !strcmp(arg, "--publish_clear")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_publish_config_process_args(client_publish_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-pq") || !strcmp(argv[i], "--publish_qos")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish qos");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_config_qos(logger, argv[i + 1], &cfg->qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-ptit") || !strcmp(argv[i], "--publish_topic_id_type")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish topic id type");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_publish_publish_topic_id_type(logger, argv[i + 1], &cfg->topic_id_type) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-ptn") || !strcmp(argv[i], "--publish_topic_name")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish topic name");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                cfg->topic_name    = argv[i + 1];
                cfg->topic_id_type = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME;
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-ppi") || !strcmp(argv[i], "--publish_predefined_topic_id")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish predefined topic id");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_topic_id(logger, argv[i + 1], &cfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                cfg->topic_id_type = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED;
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-pti") || !strcmp(argv[i], "--publish_topic_id")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish topic id");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_topic_id(logger, argv[i + 1], &cfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                cfg->topic_id_type = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICID;
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-pri") || !strcmp(argv[i], "--publish_registration_id")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish registration id");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_topic_id(logger, argv[i + 1], &cfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                cfg->topic_id_type = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_REGISTRATIONINDEX;
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-pr") || !strcmp(argv[i], "--publish_retain")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish retain");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_retain(logger, argv[i + 1], &cfg->retain) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-pm") || !strcmp(argv[i], "--publish_message")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish message");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->publish_list_len < cfg->publish_list_max_len) {
                    cfg->publish_list[cfg->publish_list_len].qos           = cfg->qos;
                    cfg->publish_list[cfg->publish_list_len].topic_id_type = cfg->topic_id_type;
                    cfg->publish_list[cfg->publish_list_len].topic_id      = cfg->topic_id;
                    cfg->publish_list[cfg->publish_list_len].retain        = cfg->retain;
                    cfg->publish_list[cfg->publish_list_len].topic_name    = cfg->topic_name;

                    cfg->publish_list[cfg->publish_list_len].data_length = strlen(argv[i + 1]) + 1;
                    cfg->publish_list[cfg->publish_list_len].data        = (uint8_t *)argv[i + 1];

                    cfg->publish_list_len += 1;
                } else {
                    print_config_list_full(logger, argv[i], cfg->publish_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-pmL") || !strcmp(argv[i], "--publish_message_URL")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish message URL");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->publish_list_len < cfg->publish_list_max_len) {
                    if (parse_client_publish_single_client_publish_config(logger, argv[i + 1], &cfg->publish_list[cfg->publish_list_len++],
                                                                          0) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                        print_unsupported_url_scheme(logger);
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], cfg->publish_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-pmhbL") || !strcmp(argv[i], "--publish_message_hex_byte_URL")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish message hex byte URL");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->publish_list_len < cfg->publish_list_max_len) {
                    if (parse_client_publish_single_client_publish_config(logger, argv[i + 1], &cfg->publish_list[cfg->publish_list_len++],
                                                                          1) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                        print_unsupported_url_scheme(logger);
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], cfg->publish_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-pc") || !strcmp(argv[i], "--publish_clear")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "publish clear");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_clear_boolean(logger, argv[i + 1], &cfg->publish_clear) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                if (cfg->publish_clear) {
                    cfg->publish_list_len = 0;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
int parse_client_publish_single_client_publish_config(const MqttSnLogger *logger, char *url, single_client_publish_config *scfg,
                                                      uint8_t is_hex) {
    char *topic_id_type_str = strtok(url, "://");
    char *qos_str           = strtok(NULL, "://");
    char *retain_str        = strtok(NULL, ":");
    char *topic_id_str      = strtok(NULL, ":");

    char *data_str = strtok(NULL, "");

    char *null_token = strtok(NULL, ":");

    if (parse_client_publish_publish_topic_id_type(logger, topic_id_type_str, &scfg->topic_id_type) == MQTT_SN_PARSE_CONFIG_FAILURE) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (qos_str) {
        if (parse_client_config_qos(logger, qos_str, &scfg->qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (retain_str) {
        if (parse_retain(logger, retain_str, &scfg->retain) == MQTT_SN_PARSE_CONFIG_FAILURE) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (topic_id_str) {
        if (scfg->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME) {
            scfg->topic_name = topic_id_str;
        } else {
            if (parse_topic_id(logger, topic_id_str, &scfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            }
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (retain_str) {
        if (parse_retain(logger, retain_str, &scfg->retain) == MQTT_SN_PARSE_CONFIG_FAILURE) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (data_str) {
        if (!is_hex) {
            scfg->data_length = strlen(data_str) + 1;
            scfg->data        = (uint8_t *)data_str;
        } else {
            if (parse_hex_to_uint8_array(logger, data_str, strlen(data_str), (uint8_t *)data_str, &scfg->data_length, strlen(data_str)) ==
                MQTT_SN_PARSE_CONFIG_FAILURE) {
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            }
            scfg->data = (uint8_t *)data_str;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (null_token) {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int parse_client_publish_publish_topic_id_type(const MqttSnLogger *logger, const char *topic_id_type_str, uint8_t *topic_id_type) {
    if (topic_id_type_str) {
        if (!strcmp(topic_id_type_str, "topicname")) {
            (*topic_id_type) = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME;
        } else if (!strcmp(topic_id_type_str, "predefined")) {
            (*topic_id_type) = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED;
        } else if (!strcmp(topic_id_type_str, "topicid")) {
            (*topic_id_type) = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICID;
        } else if (!strcmp(topic_id_type_str, "registrationindex")) {
            (*topic_id_type) = CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_REGISTRATIONINDEX;
        } else {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_publish_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("{[-pq --publish_qos] [-ptn --publish_topic_name] [-pti --publish_topic_id] [-ppi --publish_predefined_topic_id]"
                         "[-ppi --publish_registration_id] [-pr --publish_retain] [-pm --publish_message] "
                         // "[-pmL --publish_message_URL]"
                         "[-pc --publish_clear]}\n"));
}
void client_publish_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -pq : specify publish qos to use for the added publish messages. Defaults to "));
    log_int8(logger, CLIENT_MQTT_SN_PUBLISH_DEFAULT_QOS);
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -ptit : specify publish topic id type. Can be: topicname, predefined, topicid.\n"));
    log_str(logger, PSTR("         Is automatically set by invoking -ptn, -pti or -ppi. Defaults to "));
    if (CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID_TYPE == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME) {
        log_str(logger, PSTR("topicname"));
    } else if (CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID_TYPE == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED) {
        log_str(logger, PSTR("predefined"));
    } else if (CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID_TYPE == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICID) {
        log_str(logger, PSTR("topicid"));
    } else if (CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID_TYPE == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_REGISTRATIONINDEX) {
        log_str(logger, PSTR("registrationid"));
    } else {
        log_str(logger, PSTR("N/D"));
    }
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -ptn : specify publish topic name to use for the added publish messages.\n"));
    log_str(logger, PSTR("        Sets topic id type to topicname. Defaults to "));
    log_str(logger, CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_NAME);
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -ppi : specify publish predefined topic id to use for the added publish messages.\n"));
    log_str(logger, PSTR("        Sets topic id type to predefined Defaults to "));
    log_uint64(logger, CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID);
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -pti : specify publish topic id to use for the added publish messages.\n"));
    log_str(logger, PSTR("        Sets topic id type to topicid. Defaults to "));
    log_uint64(logger, CLIENT_MQTT_SN_PUBLISH_DEFAULT_TOPIC_ID);
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -ptr : specify publish registration id use for the added publish messages.\n"));
    log_str(logger, PSTR("        First send registration equals to registration id zero. Sets topic id type to registrationid.\n"));

    log_str(logger, PSTR(" -pr : specify publish retain to use for the added publish messages. Can be true or false. Defaults to "));
    CLIENT_MQTT_SN_PUBLISH_DEFAULT_RETAIN ? log_str(logger, PSTR("true")) : log_str(logger, PSTR("false"));
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -pm : add a publish message to send as string.\n"));
    //log_str(logger, PSTR(" -pmhb : add a publish message to send hex byte encoded divided by ':'. Example 00:01:AA:FF.\n"));

    log_str(logger, PSTR(" -pmL : publish message URL schema: <topic_id_type>://<qos>:<retain>:<topic_id>:<data>.\n"));
    log_str(logger, PSTR("        <topic_id_type> can be: topicname, predefined or topicid.\n"));
    log_str(logger, PSTR("        <qos> can be: -1, 0, 1, 2.\n"));
    log_str(logger, PSTR("        <retain> can be: true or false.\n"));
    log_str(logger, PSTR("        <topic_id|topic_name> can be: string for topicname, topic_id for predefined and topicid.\n"));
    log_str(logger, PSTR("        <data> as string.\n"));

    log_str(logger, PSTR(" -pmhbL : publish message URL schema: <topic_id_type>://<qos>:<retain>:<topic_id|topic_name>:<data>.\n"));
    log_str(logger, PSTR("        <topic_id_type> can be: topicname, predefined or topicid.\n"));
    log_str(logger, PSTR("        <qos> can be: -1, 0, 1, 2.\n"));
    log_str(logger, PSTR("        <retain> can be: true or false.\n"));
    log_str(logger, PSTR("        <topic_id|topic_name> can be: string for topicname, topic_id for predefined and topicid.\n"));
    log_str(logger, PSTR("        <data> hex byte encoded divided by ':'. Example 00:01:AA:FF.\n"));

    log_str(logger, PSTR(" -pc : publish clear can be true or false. If true the publish list is cleared.\n"));
}
int32_t client_publish_config_copy_to_buffer(client_publish_config *cfg, client_publish_config_buffer *cfg_buffer) {
    if (cfg->topic_name) {
        if (strlen(cfg->topic_name) + 1 > CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME_LENGTH) {
            return -1;
        }
        strcpy(cfg_buffer->topic_name, cfg->topic_name);
        cfg->topic_name = cfg_buffer->topic_name;
    }
    for (uint32_t i = 0; i < cfg->publish_list_max_len; i++) {
        if (cfg->publish_list[i].topic_name) {
            if (strlen(cfg->publish_list[i].topic_name) + 1 > CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME_LENGTH) {
                return -1;
            }
            strcpy(cfg_buffer->publish_list[i].topic_name, cfg->publish_list[i].topic_name);
            cfg->publish_list[i].topic_name = cfg_buffer->publish_list[i].topic_name;
        }

        if (cfg->publish_list[i].data) {
            if (cfg->publish_list[i].data_length > CLIENT_MQTT_SN_PUBLISH_DATA_LENGTH) {
                return -1;
            }
            memcpy(cfg_buffer->publish_list[i].data, cfg->publish_list[i].data, cfg->publish_list[i].data_length);
            cfg->publish_list[i].data = cfg_buffer->publish_list[i].data;
        }
    }
    return 0;
}
