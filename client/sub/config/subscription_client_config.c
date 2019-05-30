//
// Created by SomeDude on 28.05.2019.
//

#include "subscription_client_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>

int32_t subscription_client_config_init(subscription_client_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  mqtt_sn_version_config_init(&cfg->msvcfg);
  mqtt_sn_gateway_config_init(&cfg->msgcfg);
  gateway_network_config_init(&cfg->gncfg);
  mqtt_sn_logger_config_init(&cfg->mslcfg);

  client_find_mqtt_sn_gateway_config_init(&cfg->cfmsgcfg);
  client_connect_config_init(&cfg->cccfg);
  client_register_config_init(&cfg->crcfg);
  client_subscribe_config_init(&cfg->cscfg);

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void subscription_client_config_cleanup(subscription_client_config *cfg) {
  mqtt_sn_version_config_cleanup(&cfg->msvcfg);
  mqtt_sn_gateway_config_cleanup(&cfg->msgcfg);
  gateway_network_config_cleanup(&cfg->gncfg);
  mqtt_sn_logger_config_cleanup(&cfg->mslcfg);

  client_find_mqtt_sn_gateway_config_cleanup(&cfg->cfmsgcfg);
  client_connect_config_cleanup(&cfg->cccfg);
  client_register_config_cleanup(&cfg->crcfg);
  client_subscribe_config_cleanup(&cfg->cscfg);
}
int32_t subscription_client_config_file_process_command_callback(void *cfg,
                                                                 const MqttSnLogger *logger,
                                                                 int argc,
                                                                 char *argv[]) {
  subscription_client_config *sccfg = cfg;
  return subscription_client_config_process_line(sccfg, logger, argc, argv);
}
int32_t subscription_client_config_process_line(subscription_client_config *cfg,
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
                            subscription_client_config_file_process_command_callback,
                            argv[0]) == MQTT_SN_PARSE_CONFIG_FAILURE) {
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
    client_file_rc = 2;
  }

  int32_t msgfg_rc = mqtt_sn_gateway_config_process_args(&cfg->msgcfg, logger, argc, argv);
  int32_t gnfg_rc = gateway_network_config_process_args(&cfg->gncfg, logger, argc, argv);
  int32_t mslcfg_rc = mqtt_sn_logger_config_process_args(&cfg->mslcfg, logger, argc, argv);

  int32_t cfmsgcfg_rc = client_find_mqtt_sn_gateway_config_process_args(&cfg->cfmsgcfg, logger, argc, argv);
  int32_t cccfg_rc = client_connect_config_process_args(&cfg->cccfg, logger, argc, argv);
  int32_t crcfg_rc = client_register_config_process_args(&cfg->crcfg, logger, argc, argv);
  int32_t cscfg_rc = client_subscribe_config_process_args(&cfg->cscfg, logger, argc, argv);

  int32_t cfg_rc = subscription_client_config_process_args(cfg, logger, argc, argv);

  if (msgfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || gnfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cfmsgcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cccfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || crcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cscfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  if (client_file_rc + msgfg_rc + gnfg_rc + mslcfg_rc + cfmsgcfg_rc + cccfg_rc + crcfg_rc + cscfg_rc + cfg_rc
      != (argc - 1)) {
    // there is an unknown option
    for (int i = 0; i < argc; i++) {
      if (!is_mqtt_sn_gateway_config_command(argv[i], &i)
          & !is_gateway_network_config_command(argv[i], &i)
          & !is_mqtt_sn_logger_config_command(argv[i], &i)
          & !is_client_find_mqtt_sn_gateway_config_command(argv[i], &i)
          & !is_client_connect_config_command(argv[i], &i)
          & !is_client_register_config_command(argv[i], &i)
          & !is_client_subscribe_config_command(argv[i], &i)
          & !is_subscription_client_config_process_command(argv[i], &i)) {
        print_unknown_option(logger, argv[i]);
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}

int32_t is_subscription_client_config_process_command(const char *arg, int *i) {
  return 0;
}
int32_t subscription_client_config_process_args(subscription_client_config *cpfg,
                                                const MqttSnLogger *logger,
                                                int argc,
                                                char **argv) {
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void subscription_client_config_print_usage(const MqttSnLogger *logger) {
  log_str(logger,
          PSTR("client_sub is a simple mqtt-sn client that will subscribe to  mqtt-sn topics on a mqtt-sn gateway\n"));
  mqtt_sn_version_config_print_usage_short(logger);
  subscription_client_config_print_usage_short(logger);
  log_flush(logger);
  log_flush(logger);
  subscription_client_config_print_usage_long(logger);
  print_show_help(logger);
  log_flush(logger);
}
void subscription_client_config_print_usage_short(const MqttSnLogger *logger) {
  const char *indent = "                  ";
  log_str(logger, PSTR("Usage: client_sub "));

  mqtt_sn_gateway_config_print_usage_short(logger, NULL);
  client_find_mqtt_sn_gateway_config_print_usage_short(logger, indent);

  gateway_network_config_print_usage_short(logger, indent);

  client_connect_config_print_usage_short(logger, indent);
  client_register_config_print_usage_short(logger, indent);
  client_subscribe_config_print_usage_short(logger, indent);

  mqtt_sn_logger_config_print_usage_short(logger, indent);
}
void subscription_client_config_print_usage_long(const MqttSnLogger *logger) {
  mqtt_sn_gateway_config_print_usage_long(logger);
  client_find_mqtt_sn_gateway_config_print_usage_long(logger);
  gateway_network_config_print_usage_long(logger);

  client_connect_config_print_usage_long(logger);
  client_register_config_print_usage_long(logger);
  client_subscribe_config_print_usage_long(logger);

  mqtt_sn_logger_config_print_usage_long(logger);
}

void subscription_client_config_print_see_usage(const MqttSnLogger *logger) {
  log_str(logger, "Use 'client_sub --help' to see usage.");
  log_flush(logger);
}
