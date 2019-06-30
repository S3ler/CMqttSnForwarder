//
// Created by SomeDude on 28.05.2019.
//

#include "client_connect_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>

int32_t client_connect_config_init(client_connect_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  char default_client_id[] = DEFAULT_CLIENT_CONNECT_MQTT_SN_GATEWAY_CONFIG_CLIENT_ID;
  memcpy(cfg->default_client_id, default_client_id, sizeof(default_client_id));
  cfg->client_id = default_client_id;

  cfg->connect_duration = DEFAULT_MQTT_SN_CLIENT_CONNECT_DURATION;
  cfg->client_connect_timeout_ms = DEFAULT_MQTT_SN_CLIENT_CONNECT_TIMEOUT;
  cfg->connect_timeout_offset = DEFAULT_MQTT_SN_CLIENT_PING_REQUEST_OFFSET_TIME_DEFAULT_VALUE;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_connect_config_cleanup(client_connect_config *cfg) {
  // nothing to do
}
int32_t is_client_connect_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-ci") || !strcmp(arg, "--client_id")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-wt") || !strcmp(arg, "--will_topic")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-wm") || !strcmp(arg, "--will_msg")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cs") || !strcmp(arg, "--clean_session")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cct") || !strcmp(arg, "--client_connect_timeout")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cto") || !strcmp(arg, "--client_timeout_offset")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t client_connect_config_process_args(client_connect_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char **argv) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-ci") || !strcmp(argv[i], "--client_id")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "client id");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->client_id = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "-wt") || !strcmp(argv[i], "--will_topic")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "will topic");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->will_topic = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "-wm") || !strcmp(argv[i], "--will_msg")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "will message");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->will_msg = argv[i + 1];
      }
      i++;
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
    } else if (!strcmp(argv[i], "-cs") || !strcmp(argv[i], "--clean_session")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "client connect timeout");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_timeout(logger, argv[i + 1], &cfg->client_connect_timeout_ms) == MQTT_SN_PARSE_CONFIG_FAILURE) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    }else if (!strcmp(argv[i], "-cto") || !strcmp(argv[i], "--client_timeout_offset")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "client timeout timeout");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_timeout_offset(logger, argv[i + 1], &cfg->connect_timeout_offset) == MQTT_SN_PARSE_CONFIG_FAILURE) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_connect_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("[-ci --client_id] [-cd --connect_duration] [-cct client_connect_timeout] " \
                       "[-wt --will_topic] [-wm --will_msg] [-cs clean_session] "\
                       "[-cct ] [-cto ]\n"));
}
void client_connect_config_print_usage_long(const MqttSnLogger *logger) {
  // TODO überarbeiten
  log_str(logger, PSTR(" -ci : client_id .\n"));
  log_str(logger, PSTR(" -cd : connect_duration .\n"));
  log_str(logger, PSTR(" -cct : specify client connect timeout in ms. Default is 30000.\n"));
  log_str(logger, PSTR(" -wt : will_topic .\n"));
  log_str(logger, PSTR(" -wm : will_msg .\n"));
  log_str(logger, PSTR(" -cs : specify clean session. Can be true or false. Default is true.\n"));
  log_str(logger, PSTR(" -cct : specify client connect timeout in ms. Default is 30000.\n"));
  log_str(logger, PSTR(" -cto : client timeout offset in s. Default is -10.\n"));
}
