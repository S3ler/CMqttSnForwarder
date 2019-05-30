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
  cfg->registration_list_len = 0;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_register_config_cleanup(client_register_config *cfg) {
  // nothing to do here
}
int32_t is_client_register_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-rt") || !strcmp(arg, "--register_topic")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t client_register_config_process_args(client_register_config *cfg,
                                            const MqttSnLogger *logger,
                                            int argc,
                                            char **argv) {
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
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_register_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("[-rt --register_topic]\n"));

}
void client_register_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -rt : TODO .\n"));
  log_str(logger, PSTR(" -rf : TODO registers the contents of a file as topics.\n"));
  log_str(logger, PSTR(" -rc : TODO reads one topic name from console and send registration.\n"));
}


