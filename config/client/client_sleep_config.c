//
// Created by SomeDude on 06.08.2019.
//

#include "client_sleep_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>

int32_t client_sleep_config_init(client_sleep_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));
  cfg->sleep_duration = DEFAULT_CLIENT_MQTT_SN_SLEEP_DURATION;
  return 0;
}
void client_sleep_config_cleanup(client_sleep_config *cfg) {
  // nothing to do here
}
int32_t is_client_sleep_config_command(char *arg, int *i) {
  if (!strcmp(arg, "-sd") || !strcmp(arg, "--sleep_duration")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t client_sleep_config_process_args(client_sleep_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-sd") || !strcmp(argv[i], "--sleep_duration")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "sleep duration");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_duration(logger, argv[i + 1], &cfg->sleep_duration) == MQTT_SN_PARSE_CONFIG_FAILURE) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_sleep_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("{[-sd --sleep_duration]}\n"));
}
void client_sleep_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -sd : sleep duration of client.\n"));
}
