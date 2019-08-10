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

  cfg->qos = DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_QOS;
  cfg->topic_id_type = DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE;
  cfg->topic_id = DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID;
  cfg->topic_name = NULL;

  cfg->subscription_list_max_len = CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS;
  cfg->subscription_len = 0;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_subscribe_config_cleanup(client_subscribe_config *cfg) {
  /*
  if (cfg->subscription_len > 0) {
    for (uint16_t i = 0; i < cfg->subscription_len; i++) {
      client_subscription_config_cleanup(&cfg->subscription_list[i]);
    }
  }
  */
  // nothing to do here
}
void client_subscription_config_cleanup(client_subscription_config *cfg) {
  /*
  if (cfg->topic_name) {
    free(cfg->topic_name);
  }
  memset(cfg, 0, sizeof(*cfg));
  */
}
int32_t is_client_subscribe_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-st") || !strcmp(arg, "--subscribe_topic")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-stL") || !strcmp(arg, "--subscribe_topic_URL")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t client_subscribe_config_process_args(client_subscribe_config *cfg,
                                             const MqttSnLogger *logger,
                                             int argc,
                                             char **argv) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-st") || !strcmp(argv[i], "--subscribe_topic")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "topic name");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (cfg->subscription_len < cfg->subscription_list_max_len) {
          cfg->subscription_list[cfg->subscription_len].qos = 0;
          cfg->subscription_list[cfg->subscription_len].topic_name = strdup(argv[i + 1]);
          cfg->subscription_len++;
        } else {
          print_config_list_full(logger, argv[i], cfg->subscription_list_max_len);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-stL") || !strcmp(argv[i], "--subscribe_topic_URL")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "topic URL");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (cfg->subscription_len < cfg->subscription_list_max_len) {
          if (parse_client_subscription_config(logger, argv[i + 1], &cfg->subscription_list[cfg->subscription_len++])
              == MQTT_SN_PARSE_CONFIG_FAILURE) {
            print_unsupported_url_scheme(logger);
            return MQTT_SN_PARSE_CONFIG_FAILURE;
          }
        } else {
          print_config_list_full(logger, argv[i], cfg->subscription_list_max_len);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_subscribe_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("{[-sq --subscribe_qos] [-si --subscribe_topic_id] [-sn --subscribe_topic_name] "
                       "[-sc --subscribe_clear]| [-sL --subscribe_URL]}\n"));
}
void client_subscribe_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -sq : subscribe qos.\n"));
  log_str(logger, PSTR(" -si : subscription topic id.\n"));
  log_str(logger, PSTR(" -sn : subscription topic name.\n"));
  // log_str(logger, PSTR(" -sn : do not add an end of line character when printing the payload.\n"));
  // log_str(logger, PSTR(" -sr : do not print retain messages.\n"));
  // log_str(logger, PSTR(" -t : mqtt topic to subscribe to. May be repeated multiple times.\n"));
  // log_str(logger, PSTR(" -T : topic string to filter out of results. May be repeated.\n"));
  log_str(logger, PSTR(" -sc: subscribe clear can be true or false. Deletes the saved subscribe list.\n"));
  log_str(logger, PSTR(" -sL : subscribe URL schema: <topic_id_type>://<qos>:<topic_id|topic_name>.\n"));
  log_str(logger, PSTR("       <topic_id_type> can be: topicname, predefined, short, .\n"));
  log_str(logger, PSTR("       <qos> can be: 0, 1, 2.\n"));
  log_str(logger, PSTR("       <topic_id|topic_name> can be: topic_id for predefined, "
                       "short and topic_name for topicname.\n"));
}
int32_t parse_client_subscription_config(const MqttSnLogger *logger, char *url, client_subscription_config *cfg) {
  char *topic_id_type_str = strtok(url, "://");
  char *qos_str = strtok(NULL, "://");
  char *topic_name_or_id_str = strtok(NULL, ":");
  char *null_token = strtok(NULL, "://");

  if (topic_id_type_str) {
    if (!strcmp(topic_id_type_str, "topicname")) {
      cfg->topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME;
    } else if (!strcmp(topic_id_type_str, "predefined")) {
      cfg->topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED;
    } else if (!strcmp(topic_id_type_str, "short")) {
      cfg->topic_id_type = CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_SHORT;
    } else {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  } else {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (qos_str) {
    if (parse_client_publish_config_qos(logger, qos_str, &cfg->qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  } else {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (topic_name_or_id_str) {
    if (cfg->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME) {
      cfg->topic_name = topic_name_or_id_str;
    } else if (cfg->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED
        || cfg->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_SHORT) {
      if (parse_topic_id(logger, topic_name_or_id_str, &cfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
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
int32_t client_subscribe_config_copy_to_buffer(client_subscribe_config *cfg,
                                               client_subscribe_config_buffer *cfg_buffer) {
  if (cfg->topic_name) {
    if (strlen(cfg->topic_name) + 1 > MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH) {
      return -1;
    }
    strcpy(cfg_buffer->topic_name, cfg->topic_name);
    cfg->topic_name = cfg_buffer->topic_name;
  }
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

