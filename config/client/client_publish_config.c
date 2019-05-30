//
// Created by SomeDude on 28.05.2019.
//

#include "client_publish_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>

int parse_client_publish_single_client_publish_config(const MqttSnLogger *logger,
                                                      char *url,
                                                      single_client_publish_config *cfg);
int32_t client_publish_config_init(client_publish_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  cfg->default_qos = -1;
  cfg->default_topic_id_type = 1; // predefined
  cfg->publish_list_max_len = CLIENT_MQTT_SN_MAX_PUBLISH;
  cfg->publish_list_len = 0;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_publish_config_cleanup(client_publish_config *cfg) {
  // nothing to do here
}
int32_t is_client_publish_config_command(const char *arg, int *i) {
  /*
  // TODO
  if (!strcmp(arg, "-pm") || !strcmp(arg, "--publish_message")) {
    (*i)++;
    return 1;
  } else
  */
  if (!strcmp(arg, "-pmL") || !strcmp(arg, "--publish_message_URL")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t client_publish_config_process_args(client_publish_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char **argv) {
  for (int i = 0; i < argc; i++) {
    /*
    // TODO
      if (!strcmp(argv[i], "-pm") || !strcmp(argv[i], "--publish_message")) {
        if (i == argc - 1) {
          print_argument_value_not_specified(logger, argv[i], "publish message");
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        } else {
          if (cfg->publish_list_len < cfg->publish_list_max_len) {
            cfg->publish_list[cfg->publish_list_len].qos = cfg->default_qos;
            cfg->publish_list[cfg->publish_list_len].topic_name = strdup(argv[i + 1]);
            cfg->publish_list_len++;
          } else {
            print_config_list_full(logger, argv[i], cfg->subscription_list_max_len);
            return MQTT_SN_PARSE_CONFIG_FAILURE;
          }
        }
        i++;
      } else
    */
    if (!strcmp(argv[i], "-pmL") || !strcmp(argv[i], "--publish_message_URL")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "publish message");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (cfg->publish_list_len < cfg->publish_list_max_len) {
          if (parse_client_publish_single_client_publish_config(logger,
                                                                argv[i + 1],
                                                                &cfg->publish_list[cfg->publish_list_len++])
              == MQTT_SN_PARSE_CONFIG_FAILURE) {
            print_unsupported_url_scheme(logger);
            return MQTT_SN_PARSE_CONFIG_FAILURE;
          }
        } else {
          print_config_list_full(logger, argv[i], cfg->publish_list_max_len);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int parse_client_publish_single_client_publish_config(const MqttSnLogger *logger,
                                                      char *url,
                                                      single_client_publish_config *cfg) {
  char *topic_id_type_str = strtok(url, "://");
  char *qos_str = strtok(NULL, "://");
  char *topic_id_str = strtok(NULL, ":");
  char *data_str = strtok(NULL, ":");
  char *null_token = strtok(NULL, "://");

  if (topic_id_type_str) {
    if (!strcmp(topic_id_type_str, "topicname")) {
      cfg->topic_id_type = 0;
    } else if (!strcmp(topic_id_type_str, "predefined")) {
      cfg->topic_id_type = 1;
    } else if (!strcmp(topic_id_type_str, "shorttopic")) {
      cfg->topic_id_type = 2;
    } else {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  } else {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (qos_str) {
    if (parse_client_subscription_config_qos(logger, qos_str, &cfg->qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  } else {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (topic_id_str) {
    if (parse_topic_id(logger, topic_id_str, &cfg->topic_id) == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  } else {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (data_str) {
    cfg->data_length = strlen(data_str);
    cfg->data = (uint8_t *) strdup(data_str);
  } else {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (null_token) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_publish_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("{[-pq --publish_qos][-pm --publish_message] | [-pmL --publish_message_URL ... ]}\n"));
}
void client_publish_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -pq : quality of service level to use for the publish messages. Defaults to -1.\n"));
  log_str(logger, PSTR(" -ptn : topic name to publish to. Will be registered."));
  log_str(logger, PSTR(" -pi : predefined topic id to publish to. Defaults to 0."));
  log_str(logger, PSTR(" -pr : message should be retained.\n"));
  log_str(logger, PSTR(" -pm : message payload to send.\n"));
  log_str(logger, PSTR(" -pf : send the contents of a file as the message.\n"));
  log_str(logger, PSTR(" -pc : read messages from stdin, sending a separate message for each line.\n"));
  log_str(logger, PSTR(" -pmL : publish message URL schema: <topic_id_type>://<qos>:<retain>:<topic_id>:<data>.\n"));
  log_str(logger, PSTR("        <topic_id_type> can be: predefined or shorttopic.\n"));
  log_str(logger, PSTR("        <qos> can be: -1, 0, 1, 2.\n"));
  //log_str(logger, PSTR("        <retain> can be: 0, 1.\n"));
  log_str(logger, PSTR("        <topic_id> can be: topic_id for predefined, shorttopic.\n"));
  log_str(logger, PSTR("        <data> as characters.\n"));
  log_str(logger, PSTR("        URL schema overwrites default and global config.\n"));
}



