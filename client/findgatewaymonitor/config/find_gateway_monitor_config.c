//
// Created by SomeDude on 03.06.2019.
//

#include "find_gateway_monitor_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>

int32_t find_gateway_monitor_config_init(find_gateway_monitor_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  memcpy(cfg->executable_name, FINDGATEWAYMONITOR_EXECUTABLE_NAME, sizeof(FINDGATEWAYMONITOR_EXECUTABLE_NAME));
  mqtt_sn_version_config_init(&cfg->msvcfg);
  mqtt_sn_logger_config_init(&cfg->mslcfg);
  gateway_network_config_init(&cfg->gncfg);
  client_find_mqtt_sn_gateway_config_init(&cfg->cfmqsgcfg);

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void find_gateway_monitor_config_cleanup(find_gateway_monitor_config *cfg) {
  mqtt_sn_version_config_cleanup(&cfg->msvcfg);
  mqtt_sn_logger_config_cleanup(&cfg->mslcfg);
  gateway_network_config_cleanup(&cfg->gncfg);
  client_find_mqtt_sn_gateway_config_cleanup(&cfg->cfmqsgcfg);
}
int32_t is_find_gateway_monitor_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-mi") || !strcmp(arg, "--monitor_interactive")) {
    return 1;
  }
  return 0;
}
int32_t find_gateway_monitor_config_file_process_command_callback(void *cfg,
                                                                  const MqttSnLogger *logger,
                                                                  int argc,
                                                                  char *argv[]) {
  find_gateway_monitor_config *fgmcfg = cfg;
  return find_gateway_monitor_config_process_line(fgmcfg, logger, argc, argv);
}
int32_t find_gateway_monitor_config_process_line(find_gateway_monitor_config *cfg,
                                                 const MqttSnLogger *logger,
                                                 int argc,
                                                 char **argv) {
  if (contains_config_argument_help(argc, argv)) {
    return MQTT_SN_PARSE_CONFIG_HELP;
  }

  int32_t client_file_rc = 0;
  int32_t config_file_index = get_config_argument_file_index(argc, argv, logger);
  if (config_file_index == MQTT_SN_PARSE_CONFIG_FAILURE) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  if (config_file_index) {
    const char *config_file_path = argv[config_file_index + 1];
    if (process_config_file(config_file_path,
                            logger,
                            cfg,
                            find_gateway_monitor_config_file_process_command_callback,
                            argv[0])
        == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    client_file_rc = 2;
  }

  int32_t mslcfg_rc = mqtt_sn_logger_config_process_args(&cfg->mslcfg, logger, argc, argv);
  int32_t gncfg_rc = gateway_network_config_process_args(&cfg->gncfg, logger, argc, argv);
  int32_t cfmsgcfg_rc = client_find_mqtt_sn_gateway_config_process_args(&cfg->cfmqsgcfg, logger, argc, argv);
  int32_t fgmcfg_rc = find_gateway_monitor_config_process_args(cfg, logger, argc, argv);

  if (mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || gncfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cfmsgcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || fgmcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  if (client_file_rc + mslcfg_rc + gncfg_rc + cfmsgcfg_rc + fgmcfg_rc != (argc - 1)) {
    // there is an unknown option
    for (int i = 1; i < argc; i++) {
      if (!is_mqtt_sn_logger_config_command(argv[i], &i)
          && !is_gateway_network_config_command(argv[i], &i)
          && !is_client_find_mqtt_sn_gateway_config_command(argv[i], &i)
          && !is_find_gateway_monitor_config_command(argv[i], &i)) {
        print_unknown_option(logger, argv[i]);
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t find_gateway_monitor_config_process_args(find_gateway_monitor_config *cfg,
                                                 const MqttSnLogger *logger,
                                                 int argc,
                                                 char **argv) {
  int32_t parsed_args = 0;
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-mi") || !strcmp(argv[i], "--monitor_interactive")) {
      cfg->interactive = 1;
      parsed_args += 1;
    }
  }
  return parsed_args;
}
void find_gateway_monitor_config_print_usage(const MqttSnLogger *logger) {
  log_str(logger, PSTR(FINDGATEWAYMONITOR_EXECUTABLE_NAME));
  log_str(logger, PSTR(" is a programm to search mqtt-sn gateway and monitor advertise and gwinfo messages.\n"));
  mqtt_sn_version_config_print_usage_short(logger);
  find_gateway_monitor_config_print_usage_short(logger);
  log_flush(logger);
  log_flush(logger);
  find_gateway_monitor_config_print_usage_long(logger);
  print_show_help(logger);
  log_flush(logger);
}
void find_gateway_monitor_config_print_usage_short(const MqttSnLogger *logger) {
  const char *indent = "                         ";
  log_str(logger, PSTR("Usage: "));
  log_str(logger, PSTR(FINDGATEWAYMONITOR_EXECUTABLE_NAME));
  log_str(logger, PSTR(" "));
  log_str(logger, "[-mi --monitor_interactive]\n");

  client_find_mqtt_sn_gateway_config_print_usage_short(logger, indent);
  gateway_network_config_print_usage_short(logger, indent);
  mqtt_sn_logger_config_print_usage_short(logger, indent);

}
void find_gateway_monitor_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -mi : enables interactive mode.\n"));
  client_find_mqtt_sn_gateway_config_print_usage_long(logger);
  gateway_network_config_print_usage_long(logger);
  mqtt_sn_logger_config_print_usage_long(logger);
}
void find_gateway_monitor_config_print_see_usage(const MqttSnLogger *logger) {
  print_see_usage(logger, PSTR(FINDGATEWAYMONITOR_EXECUTABLE_NAME));
}
