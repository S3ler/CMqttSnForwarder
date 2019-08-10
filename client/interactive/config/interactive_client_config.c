//
// Created by SomeDude on 27.05.2019.
//

#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>
#include <stdbool.h>
#include "interactive_client_config.h"

int32_t interactive_client_config_init(interactive_client_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  memcpy(&cfg->executable_name,
         MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME,
         sizeof(MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME));

  mqtt_sn_version_config_init(&cfg->msvcfg);
  mqtt_sn_gateway_config_init(&cfg->msgcfg);
  gateway_network_config_init(&cfg->gncfg);
  mqtt_sn_logger_config_init(&cfg->mslcfg);

  client_find_mqtt_sn_gateway_config_init(&cfg->cfmsgcfg);
  client_connect_config_init(&cfg->cccfg);
  client_register_config_init(&cfg->crcfg);
  client_subscribe_config_init(&cfg->cscfg);
  client_unsubscribe_config_init(&cfg->cuscfg);
  client_publish_config_init(&cfg->cpcfg);
  client_sleep_config_init(&cfg->scfg);

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void interactive_client_config_cleanup(interactive_client_config *cfg) {
  mqtt_sn_version_config_cleanup(&cfg->msvcfg);
  mqtt_sn_gateway_config_cleanup(&cfg->msgcfg);
  gateway_network_config_cleanup(&cfg->gncfg);
  mqtt_sn_logger_config_cleanup(&cfg->mslcfg);

  client_find_mqtt_sn_gateway_config_cleanup(&cfg->cfmsgcfg);
  client_connect_config_cleanup(&cfg->cccfg);
  client_register_config_cleanup(&cfg->crcfg);
  client_subscribe_config_cleanup(&cfg->cscfg);
  client_unsubscribe_config_cleanup(&cfg->cuscfg);
  client_publish_config_cleanup(&cfg->cpcfg);
  client_sleep_config_cleanup(&cfg->scfg);
}

int32_t interactive_client_config_file_process_command_callback(void *cfg,
                                                                const MqttSnLogger *logger,
                                                                int argc,
                                                                char **argv) {
  interactive_client_config *pccfg = cfg;
  return interactive_client_config_process_line(pccfg, logger, argc, argv);
}
int32_t interactive_client_config_process_line(interactive_client_config *cfg,
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
                            interactive_client_config_file_process_command_callback,
                            argv[0])
        == MQTT_SN_PARSE_CONFIG_FAILURE) {
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
  int32_t cuscfg_rc = client_unsubscribe_config_process_args(&cfg->cuscfg, logger, argc, argv);
  int32_t cpcfg_rc = client_publish_config_process_args(&cfg->cpcfg, logger, argc, argv);
  int32_t scfg_rc = client_sleep_config_process_args(&cfg->scfg, logger, argc, argv);

  int32_t cfg_rc = interactive_client_config_process_args(cfg, logger, argc, argv);

  if (msgfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || gnfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cfmsgcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cccfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || crcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cscfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cuscfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cpcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || cfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      || scfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE
      ) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  if (client_file_rc + msgfg_rc + gnfg_rc + mslcfg_rc + cfmsgcfg_rc + cccfg_rc + crcfg_rc + cscfg_rc + cuscfg_rc
      + cpcfg_rc + cfg_rc + scfg_rc != (argc - 1)) {
    // there is an unknown option
    for (int i = 1; i < argc; i++) {
      if (!is_interactive_client_config_command(argv[i], &i)
          & !is_gateway_network_config_command(argv[i], &i)
          & !is_mqtt_sn_logger_config_command(argv[i], &i)
          & !is_client_find_mqtt_sn_gateway_config_command(argv[i], &i)
          & !is_client_connect_config_command(argv[i], &i)
          & !is_client_register_config_command(argv[i], &i)
          & !is_client_subscribe_config_command(argv[i], &i)
          & !is_client_unsubscribe_config_command(argv[i], &i)
          & !is_client_publish_config_command(argv[i], &i)
          & !is_mqtt_sn_gateway_config_command(argv[i], &i)
          & !is_client_sleep_config_command(argv[i], &i)
          ) {
        print_unknown_option(logger, argv[i]);
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t is_interactive_client_config_command(char *arg, int *i) {
  return 0;
}
int32_t interactive_client_config_process_args(interactive_client_config *cfg,
                                               const MqttSnLogger *logger,
                                               int argc,
                                               char **argv) {
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void interactive_client_config_print_usage(const MqttSnLogger *logger) {
  log_str(logger, PSTR(MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME));
  log_str(logger, PSTR(" is a simple mqtt-sn client that will interactive mqtt-sn messages to a mqtt-sn gateway\n"));
  mqtt_sn_version_config_print_usage_short(logger);
  interactive_client_config_print_usage_short(logger);
  log_flush(logger);
  log_flush(logger);
  interactive_client_config_print_usage_long(logger);
  print_show_help(logger);
  log_flush(logger);
}
void interactive_client_config_print_usage_short(const MqttSnLogger *logger) {
  const char *indent = "                     ";
  log_str(logger, PSTR("Usage: "));
  log_str(logger, PSTR(MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME));
  log_str(logger, PSTR(" "));

  mqtt_sn_gateway_config_print_usage_short(logger, NULL);
  client_find_mqtt_sn_gateway_config_print_usage_short(logger, indent);

  gateway_network_config_print_usage_short(logger, indent);

  client_connect_config_print_usage_short(logger, indent);
  client_register_config_print_usage_short(logger, indent);
  client_subscribe_config_print_usage_short(logger, indent);
  client_unsubscribe_config_print_usage_short(logger, indent);
  client_publish_config_print_usage_short(logger, indent);
  client_sleep_config_print_usage_short(logger, indent);

  mqtt_sn_logger_config_print_usage_short(logger, indent);

}
void interactive_client_config_print_usage_long(const MqttSnLogger *logger) {
  mqtt_sn_gateway_config_print_usage_long(logger);
  client_find_mqtt_sn_gateway_config_print_usage_long(logger);
  gateway_network_config_print_usage_long(logger);

  client_connect_config_print_usage_long(logger);
  client_register_config_print_usage_long(logger);
  client_subscribe_config_print_usage_long(logger);
  client_unsubscribe_config_print_usage_long(logger);
  client_publish_config_print_usage_long(logger);
  client_sleep_config_print_usage_long(logger);

  mqtt_sn_logger_config_print_usage_long(logger);
}
void interactive_client_config_print_see_usage(const MqttSnLogger *logger) {
  print_see_usage(logger, PSTR(MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME));
}
int32_t interactive_client_config_copy_to_buffer(interactive_client_config *cfg) {
  if (mqtt_sn_gateway_config_buffer_copy_to_buffer(&cfg->msgcfg, &cfg->msgcfgb) < 0) {
    return -1;
  }
  if (gateway_network_config_copy_to_buffer(&cfg->gncfg, &cfg->gncfgb) < 0) {
    return -1;
  }
  if (mqtt_sn_logger_config_buffer_copy_to_buffer(&cfg->mslcfg, &cfg->mslcfgb) < 0) {
    return -1;
  }
  if (client_connect_config_copy_to_buffer(&cfg->cccfg, &cfg->cccfgb) < 0) {
    return -1;
  }
  if (client_register_config_copy_to_buffer(&cfg->crcfg, &cfg->crcfgb) < 0) {
    return -1;
  }
  if (client_subscribe_config_copy_to_buffer(&cfg->cscfg, &cfg->cscfgb) < 0) {
    return -1;
  }
  if (client_unsubscribe_config_copy_to_buffer(&cfg->cuscfg, &cfg->cuscfgb) < 0) {
    return -1;
  }
  if (client_publish_config_copy_to_buffer(&cfg->cpcfg, &cfg->cpcfgb) < 0) {
    return -1;
  }
  return 0;
}
void print_interactive_client_config_print_not_copyable(const MqttSnLogger *logger) {
  log_str(logger, PSTR("cannot save/copy input to configuration - too long."));
  log_flush(logger);
}
int32_t mqtt_sn_cmd_buffer_simulate(const interactive_client_config *cfg,
                                    const MqttSnLogger *logger,
                                    int argc,
                                    char **argv) {
  interactive_client_config cfg_cp = (*cfg);
  int rc = interactive_client_config_process_line(&cfg_cp, logger, argc, argv);
  if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
    return rc;
  }
  if (rc < 0) {
    return rc;
  }
  if (interactive_client_config_copy_to_buffer(&cfg_cp) < 0) {
    return -1;
  }
  return 0;
}
