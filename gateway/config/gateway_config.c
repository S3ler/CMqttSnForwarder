//
// Created by SomeDude on 18.06.2019.
//

#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>
#include "gateway_config.h"

int32_t mqtt_sn_gateway__config_init(mqtt_sn_gateway__config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  memcpy(&cfg->executable_name, MQTT_SN_GATEWAY_EXECUTABLE_NAME, sizeof(MQTT_SN_GATEWAY_EXECUTABLE_NAME));

  mqtt_sn_version_config_init(&cfg->msvcfg);
  client_network_config_init(&cfg->cncfg);
  gateway_advertisement_config_init(&cfg->gacfg);
  gateway_client_connection_config_init(&cfg->gccccfg);
  mqtt_sn_logger_config_init(&cfg->mslcfg);

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void mqtt_sn_gateway__config_cleanup(mqtt_sn_gateway__config *cfg) {
  mqtt_sn_version_config_cleanup(&cfg->msvcfg);
  client_network_config_cleanup(&cfg->cncfg);
  gateway_advertisement_config_cleanup(&cfg->gacfg);
  gateway_client_connection_config_cleanup(&cfg->gccccfg);
  mqtt_sn_logger_config_cleanup(&cfg->mslcfg);
}
void mqtt_sn_gateway__config_print_usage(const MqttSnLogger *logger) {
  log_str(logger, PSTR(MQTT_SN_GATEWAY_EXECUTABLE_NAME));
  log_str(logger, PSTR(" is a simple mqtt-sn gateway that will send mqtt-sn messages back to the receiver\n"));
  mqtt_sn_version_config_print_usage_short(logger);
  mqtt_sn_gateway__config_print_usage_short(logger);
  log_flush(logger);
  log_flush(logger);
  mqtt_sn_gateway__config_print_usage_long(logger);
}
void mqtt_sn_gateway__config_print_usage_short(const MqttSnLogger *logger) {
  const char *indent = "                       ";
  log_str(logger, PSTR("Usage: "));
  log_str(logger, PSTR(MQTT_SN_GATEWAY_EXECUTABLE_NAME));
  log_str(logger, PSTR(" "));
  gateway_advertisement_config_print_usage_short(logger, NULL);
  gateway_client_connection_config_print_usage_short(logger, indent);
  client_network_config_print_usage_short(logger, indent);
  mqtt_sn_logger_config_print_usage_short(logger, indent);
}
void mqtt_sn_gateway__config_print_usage_long(const MqttSnLogger *logger) {
  gateway_advertisement_config_print_usage_long(logger);
  gateway_client_connection_config_print_usage_long(logger);
  client_network_config_print_usage_long(logger);
  mqtt_sn_logger_config_print_usage_long(logger);
}
void mqtt_sn_gateway__config_print_see_usage(const MqttSnLogger *logger) {
  print_see_usage(logger, PSTR(MQTT_SN_GATEWAY_EXECUTABLE_NAME));
}
int32_t mqtt_sn_gateway__config_file_process_command_callback(void *cfg,
                                                              const MqttSnLogger *logger,
                                                              int argc,
                                                              char *argv[]) {
  mqtt_sn_gateway__config *egcfg = cfg;
  return process_mqtt_sn_gateway__config_line(egcfg, logger, argc, argv);
}
int32_t process_mqtt_sn_gateway__config_line(mqtt_sn_gateway__config *cfg,
                                             const MqttSnLogger *logger,
                                             int argc,
                                             char *argv[]) {
  if (contains_config_argument_help(argc, argv)) {
    return MQTT_SN_PARSE_CONFIG_HELP;
  }

  int32_t config_file_rc = 0;
  int32_t config_file_index = get_config_argument_file_index(argc, argv, logger);
  if (config_file_index == MQTT_SN_PARSE_CONFIG_FAILURE) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  if (config_file_index) {
    const char *config_file_path = argv[config_file_index + 1];
    if (process_config_file(config_file_path,
                            logger,
                            cfg,
                            mqtt_sn_gateway__config_file_process_command_callback,
                            argv[0]) == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    config_file_rc = 2;
  }

  int32_t gacfg_rc = gateway_advertisement_config_process_args(&cfg->gacfg, logger, argc, argv);
  int32_t gccccfg = gateway_client_connection_config_process_args(&cfg->gccccfg, logger, argc, argv);
  int32_t cnfg_rc = client_network_config_process_args(&cfg->cncfg, logger, argc, argv);
  int32_t mslcfg_rc = mqtt_sn_logger_config_process_args(&cfg->mslcfg, logger, argc, argv);

  if (gacfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || gccccfg == MQTT_SN_PARSE_CONFIG_FAILURE
      || cnfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (config_file_rc + gccccfg + gacfg_rc + cnfg_rc + mslcfg_rc != (argc - 1)) {
    // there is an unknown option
    for (int i = 1; i < argc; i++) {
      if (!is_gateway_advertisement_config_command(argv[i], &i)
          && !is_gateway_client_connection_config_command(argv[i], &i)
          && !is_client_network_config_command(argv[i], &i)
          && !is_mqtt_sn_logger_config_command(argv[i], &i)) {
        print_unknown_option(logger, argv[i]);
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }
    }
  }

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
