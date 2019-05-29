//
// Created by SomeDude on 28.05.2019.
//

#include "echogateway_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/config_command_helper.h>

int32_t echogateway_config_init(echogateway_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));
  mqtt_sn_version_config_init(&cfg->msvcfg);
  client_network_config_init(&cfg->cncfg);
  mqtt_sn_logger_config_init(&cfg->mslcfg);
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void echogateway_config_cleanup(echogateway_config *cfg) {
  client_network_config_cleanup(&cfg->cncfg);
  mqtt_sn_logger_config_cleanup(&cfg->mslcfg);
}
void echogateway_config_print_usage(const MqttSnLogger *logger) {
  log_str(logger,
          PSTR("echogateway is a simple mqtt-sn gateway that will send mqtt-sn messages back to the receiver\n"));
  mqtt_sn_version_config_print_usage_short(logger);
  echogateway_config_print_usage_short(logger);
  log_flush(logger);
  log_flush(logger);
  echogateway_config_print_usage_long(logger);
}
void echogateway_config_print_usage_short(const MqttSnLogger *logger) {
  const char *indent = "                   ";
  log_str(logger, PSTR("Usage: echogateway "));
  client_network_config_print_usage_short(logger, indent);
  mqtt_sn_logger_config_print_usage_short(logger, indent);
  if (indent) {
    log_str(logger, indent);
  }
  // TODO implement me
}
void echogateway_config_print_usage_long(const MqttSnLogger *logger) {
  client_network_config_print_usage_long(logger);
  // TODO implement me
  mqtt_sn_logger_config_print_usage_long(logger);
}
void echogateway_config_print_see_usage(const MqttSnLogger *logger) {
  log_str(logger, "Use 'echogateway --help' to see usage.");
}

int32_t process_echogateway_config_line(echogateway_config *cfg, const MqttSnLogger *logger, int argc, char *argv[]) {
  if (contains_config_argument_help(argc, argv)) {
    return MQTT_SN_PARSE_CONFIG_HELP;
  } else if (client_network_config_process_line(&cfg->cncfg, logger, argc, argv) < 0) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  } else if (mqtt_sn_logger_config_process_args(&cfg->mslcfg, logger, argc, argv) < 0) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  } else {
    for (int i = 0; i < argc; i++) {
// TODO implement me own parsing
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
