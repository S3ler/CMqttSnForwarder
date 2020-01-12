//
// Created by SomeDude on 28.05.2019.
//

#include "client_subscribe_config.h"
#include "config/common/config_command_helper.h"
#include <platform/platform_compatibility.h>
#include <string.h>
#include <stdlib.h>

int32_t client_subscribe_config_init(client_subscribe_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));

    cfg->qos                       = DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_QOS;
    cfg->subscription_list_max_len = CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS;
    cfg->subscription_len          = 0;

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_subscribe_config_cleanup(client_subscribe_config *cfg) {
    // nothing to do here
}
int32_t is_client_subscribe_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-sq") || !strcmp(arg, "--subscribe_qos")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-stn") || !strcmp(arg, "--subscribe_topic_name")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-stp") || !strcmp(arg, "--subscribe_predefined_topic_id")) {
        (*i)++;
        return 1;
    /*} else if (!strcmp(arg, "-sti") || !strcmp(arg, "--subscribe_topic_id")) {
        (*i)++;
        return 1;*/
    } else if (!strcmp(arg, "-stL") || !strcmp(arg, "--subscribe_topic_URL")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-sc") || !strcmp(arg, "--subscribe_clear")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_subscribe_config_process_args(client_subscribe_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-sq") || !strcmp(argv[i], "--subscribe_qos")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "subscribe qos");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_config_qos(logger, argv[i + 1], &cfg->qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-stn") || !strcmp(argv[i], "--subscribe_topic_name")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "subscribe topic name");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->subscription_len < cfg->subscription_list_max_len) {
                    cfg->subscription_list[cfg->subscription_len].topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME;
                    cfg->subscription_list[cfg->subscription_len].qos           = cfg->qos;
                    cfg->subscription_list[cfg->subscription_len].topic_id      = 0;
                    cfg->subscription_list[cfg->subscription_len].topic_name    = argv[i + 1];
                    cfg->subscription_len++;
                } else {
                    print_config_list_full(logger, argv[i], cfg->subscription_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-stp") || !strcmp(argv[i], "--subscribe_predefined_topic_id")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "subscribe predefined topic id");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->subscription_len < cfg->subscription_list_max_len) {
                    cfg->subscription_list[cfg->subscription_len].topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED;
                    cfg->subscription_list[cfg->subscription_len].qos           = cfg->qos;
                    if (parse_topic_id(logger, argv[i + 1], &cfg->subscription_list[cfg->subscription_len].topic_id) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                    cfg->subscription_list[cfg->subscription_len].topic_name = NULL;
                    cfg->subscription_len++;
                } else {
                    print_config_list_full(logger, argv[i], cfg->subscription_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
            /*} else if (!strcmp(argv[i], "-sti") || !strcmp(argv[i], "--subscribe_topic_id")) {
                if (i == argc - 1) {
                    print_argument_value_not_specified(logger, argv[i], "subscribe topic id");
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                } else {
                    if (cfg->subscription_len < cfg->subscription_list_max_len) {
                        cfg->subscription_list[cfg->subscription_len].topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_SHORT;
                        cfg->subscription_list[cfg->subscription_len].qos           = cfg->qos;
                        if (parse_topic_id(logger, argv[i + 1], &cfg->subscription_list[cfg->subscription_len].topic_id) ==
                            MQTT_SN_PARSE_CONFIG_FAILURE) {
                            return MQTT_SN_PARSE_CONFIG_FAILURE;
                        }
                        cfg->subscription_list[cfg->subscription_len].topic_name = NULL;
                        cfg->subscription_len++;
                    } else {
                        print_config_list_full(logger, argv[i], cfg->subscription_list_max_len);
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                }
                i++;
                parsed_args += 2;*/
        } else if (!strcmp(argv[i], "-stL") || !strcmp(argv[i], "--subscribe_topic_URL")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "subscribe topic URL");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (cfg->subscription_len < cfg->subscription_list_max_len) {
                    if (parse_client_subscription_config(logger, argv[i + 1], &cfg->subscription_list[cfg->subscription_len++]) ==
                        MQTT_SN_PARSE_CONFIG_FAILURE) {
                        print_unsupported_url_scheme(logger);
                        return MQTT_SN_PARSE_CONFIG_FAILURE;
                    }
                } else {
                    print_config_list_full(logger, argv[i], cfg->subscription_list_max_len);
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-sc") || !strcmp(argv[i], "--subscribe_clear")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "subscribe clear");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_clear_boolean(logger, argv[i + 1], &cfg->subscribe_clear) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
                if (cfg->subscribe_clear) {
                    cfg->subscription_len = 0;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_subscribe_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    /*log_str(logger, PSTR("{[-sq --subscribe_qos] [-stn --subscribe_topic_name] [-stp --subscribe_predefined_topic_id] "
                         "[-sti --subscribe_topic_id] [-sL --subscribe_URL] [-sc --subscribe_clear]}\n"));*/
    log_str(logger, PSTR("{[-sq --subscribe_qos] [-stn --subscribe_topic_name] [-stp --subscribe_predefined_topic_id] "
                         "[-stL --subscribe_topic_URL] [-sc --subscribe_clear]}\n"));
}
void client_subscribe_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -sq : specify subscribe qos to use for the added subscriptions. Defaults to "));
    log_uint64(logger, DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_QOS);
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -stn : add a subscription by topic name. Sets topic id type to topicname.\n"));
    log_str(logger, PSTR(" -stp : add a subscription by predefined topic id. Sets topic id type to predefined.\n"));
    //log_str(logger, PSTR(" -sti : add a subscription by short topic id. Sets topic id type to short.\n"));

    log_str(logger, PSTR(" -stL : subscribe topic URL schema: <topic_id_type>://<qos>:<topic_id|topic_name>.\n"));
    //log_str(logger, PSTR("       <topic_id_type> can be: topicname, predefined, or short.\n"));
    log_str(logger, PSTR("       <topic_id_type> can be: topicname, predefined.\n"));
    log_str(logger, PSTR("       <qos> can be: 0, 1, 2.\n"));
    //log_str(logger, PSTR("       <topic_id|topic_name> can be: string for topic_name, topic_id for predefined and short.\n"));
    log_str(logger, PSTR("       <topic_id|topic_name> can be: string for topic_name, topic_id for predefined.\n"));

    log_str(logger, PSTR(" -sc: subscribe clear can be true or false. If true the subscribe list is cleared.\n"));
}
int32_t parse_client_subscription_config(const MqttSnLogger *logger, char *url, client_subscription_topic *scfg) {
    char *topic_id_type_str    = strtok(url, "://");
    char *qos_str              = strtok(NULL, "://");
    char *topic_name_or_id_str = strtok(NULL, ":");
    char *null_token           = strtok(NULL, "://");

    if (topic_id_type_str) {
        if (!strcmp(topic_id_type_str, "topicname")) {
            scfg->topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME;
        } else if (!strcmp(topic_id_type_str, "predefined")) {
            scfg->topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED;
            } else if (!strcmp(topic_id_type_str, "topicid")) {
            scfg->topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICID;
        } else {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (qos_str) {
        if (parse_client_config_qos(logger, qos_str, &scfg->qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
    } else {
        return MQTT_SN_PARSE_CONFIG_FAILURE;
    }

    if (topic_name_or_id_str) {
        if (scfg->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME) {
            scfg->topic_name = topic_name_or_id_str;
        } else if (scfg->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED
            || scfg->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICID) {
            if (parse_topic_id(logger, topic_name_or_id_str, &scfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
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
int32_t client_subscribe_config_copy_to_buffer(client_subscribe_config *cfg, client_subscribe_config_buffer *cfg_buffer) {
    for (uint32_t i = 0; i < cfg->subscription_list_max_len; i++) {
        if (cfg->subscription_list[i].topic_name) {
            if (strlen(cfg->subscription_list[i].topic_name) + 1 > MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH) {
                return -1;
            }
            strcpy(cfg_buffer->subscription_list[i].topic_name, cfg->subscription_list[i].topic_name);
            cfg->subscription_list[i].topic_name = cfg_buffer->subscription_list[i].topic_name;
        }
    }
    return 0;
}
