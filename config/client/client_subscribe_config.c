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

  cfg->subscription_list_max_len = CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS;
  cfg->subscription_len = 0;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_subscribe_config_cleanup(client_subscribe_config *cfg) {

  if (cfg->subscription_len > 0) {
    for (uint16_t i = 0; i < cfg->subscription_len; i++) {
      client_subscription_config_cleanup(&cfg->subscription_list[i]);
    }
  }
  // nothing to do here
}
void client_subscription_config_cleanup(client_subscription_config *cfg) {
  if (cfg->topic_name) {
    free(cfg->topic_name);
  }
  memset(cfg, 0, sizeof(*cfg));
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
  log_str(logger, PSTR("{[-st --subscribe_topic] | [-stL --subscribe_topic_URL]}\n"));
}
void client_subscribe_config_print_usage_long(const MqttSnLogger *logger) {

  log_str(logger, PSTR(" -st : topic name.\n"));
  log_str(logger, PSTR(" -sn : do not add an end of line character when printing the payload.\n"));
  log_str(logger, PSTR(" -sr : do not print retain messages.\n"));
  log_str(logger, PSTR(" -t : mqtt topic to subscribe to. May be repeated multiple times.\n"));
  log_str(logger, PSTR(" -T : topic string to filter out of results. May be repeated.\n"));
  log_str(logger, PSTR(" -stL : subscribe topic URL schema: <topic_id_type>://<qos>:<topic_id|topic_name>.\n"));
  log_str(logger, PSTR("        <topic_id_type> can be: predefined, shorttopic, topicname.\n"));
  log_str(logger, PSTR("        <qos> can be: 0, 1, 2.\n"));
  log_str(logger,
          PSTR(
              "        <topic_id|topic_name> can be: topic_id for predefined, shorttopic and topic_name for topicname.\n"));
}
int32_t parse_client_subscription_config(const MqttSnLogger *logger, char *url, client_subscription_config *cfg) {
  char *topic_id_type_str = strtok(url, "://");
  char *qos_str = strtok(NULL, "://");
  char *topic_name_or_id_str = strtok(NULL, ":");
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
    if (parse_client_publish_config_qos(logger, qos_str, &cfg->qos) == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  } else {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (topic_name_or_id_str) {
    if (cfg->topic_id_type == 0) {
      cfg->topic_name = strdup(topic_name_or_id_str);
    } else if (cfg->topic_id_type == 1 || cfg->topic_id_type == 2) {
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

