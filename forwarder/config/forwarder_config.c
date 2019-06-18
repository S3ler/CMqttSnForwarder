//
// Created by SomeDude on 31.05.2019.
//

#include <platform/platform_compatibility.h>
#include <platform/MqttSnMessageData.h>
#include <config/common/config_command_helper.h>
#include "forwarder_config.h"

int32_t forwarder_config_init(forwarder_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  memcpy(&cfg->executable_name, MQTT_SN_FORWARDER_EXECUTABLE_NAME, sizeof(MQTT_SN_FORWARDER_EXECUTABLE_NAME));

  mqtt_sn_version_config_init(&cfg->msvcfg);
  mqtt_sn_gateway_config_init(&cfg->msgcfg);
  client_find_mqtt_sn_gateway_config_init(&cfg->cfmsngcfg);
  client_network_config_init(&cfg->cncfg);
  gateway_network_config_init(&cfg->gncfg);
  mqtt_sn_logger_config_init(&cfg->mslcfg);

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}

void forwarder_config_cleanup(forwarder_config *cfg) {
  mqtt_sn_version_config_cleanup(&cfg->msvcfg);
  mqtt_sn_gateway_config_cleanup(&cfg->msgcfg);
  client_find_mqtt_sn_gateway_config_cleanup(&cfg->cfmsngcfg);
  client_network_config_cleanup(&cfg->cncfg);
  gateway_network_config_cleanup(&cfg->gncfg);
  mqtt_sn_logger_config_cleanup(&cfg->mslcfg);
}
int32_t forwarder_config_file_process_command_callback(void *cfg, const MqttSnLogger *logger, int argc, char **argv) {
  forwarder_config *fcfg = (forwarder_config *) cfg;
  return forwarder_config_process_line(fcfg, logger, argc, argv);
}
int32_t forwarder_config_process_line(forwarder_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
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
    if (process_config_file(config_file_path, logger, cfg, forwarder_config_file_process_command_callback, argv[0])
        == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    config_file_rc = 2;
  }

  int32_t msgcfg_rc = mqtt_sn_gateway_config_process_args(&cfg->msgcfg, logger, argc, argv);
  int32_t cnfg_rc = client_network_config_process_args(&cfg->cncfg, logger, argc, argv);
  int32_t cfmsngcfg_rc = client_find_mqtt_sn_gateway_config_process_args(&cfg->cfmsngcfg, logger, argc, argv);
  int32_t gnfg_rc = gateway_network_config_process_args(&cfg->gncfg, logger, argc, argv);
  int32_t mslcfg_rc = mqtt_sn_logger_config_process_args(&cfg->mslcfg, logger, argc, argv);
  int32_t fcfg_rc = forwarder_config_process_args(cfg, logger, argc, argv);

  if (msgcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cnfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cfmsngcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || gnfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || fcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  if (config_file_rc + msgcfg_rc + cnfg_rc + cfmsngcfg_rc + gnfg_rc + mslcfg_rc + fcfg_rc != (argc - 1)) {
    // there is an unknown option
    for (int i = 1; i < argc; i++) {
      if (!is_mqtt_sn_gateway_config_command(argv[i], &i)
          && !is_client_network_config_command(argv[i], &i)
          && !is_client_find_mqtt_sn_gateway_config_command(argv[i], &i)
          && !is_gateway_network_config_command(argv[i], &i)
          && !is_mqtt_sn_logger_config_command(argv[i], &i)
          && !is_forwarder_config_command(argv[i], &i)) {
        print_unknown_option(logger, argv[i]);
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }
    }
  }

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t is_forwarder_config_command(const char *arg, int *i) {
  // nothing to do here yet
  return 0;
}
int32_t forwarder_config_process_args(forwarder_config *cfg, const MqttSnLogger *logger, int argc, char **argv) {
  // nothing to do here yet
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void forwarder_config_print_usage(const MqttSnLogger *logger) {
  log_str(logger, PSTR("cmqttsnforwarder is a simple mqtt-sn forwarder that will forward mqtt-sn messages\n"));
  log_str(logger, PSTR("                 from a client network over a gateway network to a mqtt-sn gateway.\n"));
  mqtt_sn_version_config_print_usage_short(logger);
  forwarder_config_print_usage_short(logger);
  log_flush(logger);
  log_flush(logger);
  forwarder_config_print_usage_long(logger);
}
void forwarder_config_print_usage_short(const MqttSnLogger *logger) {
  const char *indent = "                        ";
  log_str(logger, PSTR("Usage: cmqttsnforwarder "));
  mqtt_sn_gateway_config_print_usage_short(logger, NULL);
  client_find_mqtt_sn_gateway_config_print_usage_short(logger, indent);
  gateway_network_config_print_usage_short(logger, indent);
  client_network_config_print_usage_short(logger, indent);
  mqtt_sn_logger_config_print_usage_short(logger, indent);
}
void forwarder_config_print_usage_long(const MqttSnLogger *logger) {
  mqtt_sn_gateway_config_print_usage_long(logger);
  client_find_mqtt_sn_gateway_config_print_usage_long(logger);
  gateway_network_config_print_usage_long(logger);
  client_network_config_print_usage_long(logger);
  mqtt_sn_logger_config_print_usage_long(logger);
}
void forwarder_config_print_see_usage(const MqttSnLogger *logger) {
  const char *executable_name = MQTT_SN_FORWARDER_EXECUTABLE_NAME;
  print_see_usage(logger, executable_name);
}




