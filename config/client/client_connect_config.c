//
// Created by SomeDude on 28.05.2019.
//

#include "client_connect_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>

int32_t client_connect_config_init(client_connect_config *cfg) {
    memset(cfg, 0, sizeof(*cfg));

    srand(time(NULL));
    char default_client_id[] = DEFAULT_CLIENT_CONNECT_MQTT_SN_GATEWAY_CONFIG_CLIENT_ID;
    memcpy(cfg->default_client_id, default_client_id, sizeof(default_client_id));
    cfg->protocol_id            = DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_PROTOCOLID;
    cfg->client_id              = default_client_id;
    cfg->default_will_msg[0]    = '\0';
    cfg->will_topic             = NULL;
    cfg->will_qos               = DEFAULT_MQTT_SN_CLIENT_CONNECT_WILL_TOPIC_DEFAULT_QOS;
    cfg->will_retain            = DEFAULT_MQTT_SN_CLIENT_CONNECT_WILL_TOPIC_DEFAULT_RETAIN;
    cfg->will_msg               = cfg->default_will_msg;
    cfg->clean_session          = DEFAULT_MQTT_SN_CLIENT_CONNECT_CLEAN_SESSION;
    cfg->connect_duration       = DEFAULT_MQTT_SN_CLIENT_CONNECT_DURATION;
    cfg->auto_reconnect         = DEFAULT_MQTT_SN_CLIENT_AUTO_RECONNECT_STATUS;
    cfg->connection_timeout_offset = DEFAULT_MQTT_SN_CLIENT_PING_REQUEST_OFFSET_TIME_DEFAULT_VALUE;

    return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_connect_config_cleanup(client_connect_config *cfg) {
    // nothing to do
}
int32_t is_client_connect_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-ci") || !strcmp(arg, "--client_id")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-cd") || !strcmp(arg, "--connect_duration")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-ar") || !strcmp(arg, "--auto_reconnect")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-wt") || !strcmp(arg, "--will_topic")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-wm") || !strcmp(arg, "--will_message")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-wq") || !strcmp(arg, "--will_qos")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-wr") || !strcmp(arg, "--will_retain")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-cs") || !strcmp(arg, "--clean_session")) {
        (*i)++;
        return 1;
    } else if (!strcmp(arg, "-cto") || !strcmp(arg, "--client_timeout_offset")) {
        (*i)++;
        return 1;
    }
    return 0;
}
int32_t client_connect_config_process_args(client_connect_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
    int32_t parsed_args = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-ci") || !strcmp(argv[i], "--client_id")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "client id");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                cfg->client_id = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-cd") || !strcmp(argv[i], "--connect_duration")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "connect duration");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_duration(logger, argv[i + 1], &cfg->connect_duration) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-ar") || !strcmp(argv[i], "--auto_reconnect")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "auto reconnect");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_auto_reconnect_boolean(logger, argv[i + 1], &cfg->auto_reconnect) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-wt") || !strcmp(argv[i], "--will_topic")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "will topic");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                cfg->will_topic = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-wm") || !strcmp(argv[i], "--will_message")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "will message");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                cfg->will_msg = argv[i + 1];
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-wq") || !strcmp(argv[i], "--will_qos")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "will qos");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_client_config_qos(logger, argv[i + 1], &cfg->will_qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-wr") || !strcmp(argv[i], "--will_retain")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "will retain");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_retain(logger, argv[i + 1], &cfg->will_retain) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-cs") || !strcmp(argv[i], "--clean_session")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "clean session");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_clean_session(logger, argv[i + 1], &cfg->clean_session) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        } else if (!strcmp(argv[i], "-cto") || !strcmp(argv[i], "--client_timeout_offset")) {
            if (i == argc - 1) {
                print_argument_value_not_specified(logger, argv[i], "client timeout timeout");
                return MQTT_SN_PARSE_CONFIG_FAILURE;
            } else {
                if (parse_timeout_offset(logger, argv[i + 1], &cfg->connection_timeout_offset) == MQTT_SN_PARSE_CONFIG_FAILURE) {
                    return MQTT_SN_PARSE_CONFIG_FAILURE;
                }
            }
            i++;
            parsed_args += 2;
        }
    }
    return parsed_args;
}
void client_connect_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
    if (indent) {
        log_str(logger, indent);
    }
    log_str(logger, PSTR("[-ci --client_id] [-cd --connect_duration] [-ar --auto_reconnect] "
                         "[-wt --will_topic] [-wm --will_msg] [-cs clean_session] [-cto --client_connect_offset]\n"));
}
void client_connect_config_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" -ci : specify the client id. Default "));
    log_str(logger, "are six random characters");
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -cd : connect duration in s. Default is "));
    log_uint64(logger, DEFAULT_MQTT_SN_CLIENT_CONNECT_DURATION);
    log_str(logger, PSTR(" s.\n"));

    log_str(logger, PSTR(" -ar : specify auto reconnect. Can be true or false. Default is "));
    DEFAULT_MQTT_SN_CLIENT_AUTO_RECONNECT_STATUS ? log_str(logger, PSTR("true")) : log_str(logger, PSTR("false"));
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -wt : specify will topic. Default is "));
#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_DEFAULT_WILL_TOPIC
    log_str(logger, PSTR("N/D"));
#else
    log_str(logger, PSTR(DEFAULT_MQTT_SN_CLIENT_CONNECT_DEFAULT_WILL_TOPIC));
#endif
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -wm : specify will message. Default is "));
#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_DEFAULT_WILL_MESSAGE
    log_str(logger, PSTR("N/D"));
#else
    log_str(logger, PSTR(DEFAULT_MQTT_SN_CLIENT_CONNECT_DEFAULT_WILL_MESSAGE));
#endif
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -wq : specify will qos. Can be -1, 0, 1, 2. Default is "));
    log_uint64(logger, DEFAULT_MQTT_SN_CLIENT_CONNECT_WILL_TOPIC_DEFAULT_QOS);
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -wr : specify will retain. Can be true or false. Default is "));
    DEFAULT_MQTT_SN_CLIENT_CONNECT_WILL_TOPIC_DEFAULT_RETAIN ? log_str(logger, PSTR("true")) : log_str(logger, PSTR("false"));
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -cs : specify clean session. Can be true or false. Default is "));
    DEFAULT_MQTT_SN_CLIENT_CONNECT_CLEAN_SESSION ? log_str(logger, PSTR("true")) : log_str(logger, PSTR("false"));
    log_str(logger, PSTR(".\n"));

    log_str(logger, PSTR(" -cto : specify client timeout offset in s. Sends ping requests relative to the connect duration. Default is "));
    log_int64(logger, DEFAULT_MQTT_SN_CLIENT_PING_REQUEST_OFFSET_TIME_DEFAULT_VALUE);
    log_str(logger, PSTR(" s.\n"));
}
int32_t client_connect_config_copy_to_buffer(client_connect_config *cfg, client_connect_config_buffer *cfg_buffer) {
    if (strlen(cfg->client_id) + 1 > DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_CLIENT_IT_LENGTH) {
        return -1;
    }
    strcpy(cfg_buffer->client_id, cfg->client_id);
    cfg->client_id = cfg_buffer->client_id;

    if (cfg->will_topic) {
        if (strlen(cfg->will_topic) + 1 > DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_TOPIC_LENGTH) {
            return -1;
        }
        stpcpy(cfg_buffer->will_topic, cfg->will_topic);
        cfg->will_topic = cfg_buffer->will_topic;
    }

    if (cfg->will_msg) {
        if (strlen(cfg->will_msg) + 1 > DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_MSG_LENGTH) {
            return -1;
        }
        stpcpy(cfg_buffer->will_msg, cfg->will_msg);
        cfg->will_msg = cfg_buffer->will_msg;
    }
    return 0;
}
