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
  }else if (!strcmp(arg, "-wm") || !strcmp(arg, "--will_msg")) {
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
      }else{
        cfg->client_id = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "-wt") || !strcmp(argv[i], "--will_topic")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "will topic");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }else{
        cfg->will_topic = argv[i + 1];
      }
      i++;
    }else if (!strcmp(argv[i], "-wm") || !strcmp(argv[i], "--will_msg")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "will message");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }else{
        cfg->will_msg = argv[i + 1];
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
  log_str(logger, PSTR("[-ci --client_id] [-wt --will_topic] [-wm --will_msg]\n"));
}
void client_connect_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -ci : TODO .\n"));
  log_str(logger, PSTR(" -cc : TODO disable clean session.\n"));
  log_str(logger, PSTR(" -wt : TODO .\n"));
  log_str(logger, PSTR(" -wm : TODO .\n"));
}
