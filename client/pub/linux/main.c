//
// Created by SomeDude on 27.05.2019.
//

#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <client/pub/config/publish_client_config.h>
#include <platform/platform_compatibility.h>
#include <client/pub/linux/starter/publish_client_starter.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>

int32_t start_logger(const mqtt_sn_logger_config *cfg,
                     MqttSnLogger *logger);

int main(int argc, char *argv[]) {
  MqttSnLogger cfg_logger = {0};

  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  publish_client_config cfg = {0};
  if (publish_client_config_init(&cfg)) {
    return EXIT_FAILURE;
  }
  int rc = publish_client_config_process_line(&cfg, &cfg_logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      publish_client_config_print_usage(&cfg_logger);
    } else {
      publish_client_config_print_see_usage(&cfg_logger);
      publish_client_config_cleanup(&cfg);
      return EXIT_FAILURE;
    }
    publish_client_config_cleanup(&cfg);
    return EXIT_SUCCESS;
  }

  MqttSnLogger *mqtt_sn_logger = NULL;
#ifdef WITH_LOGGING
  MqttSnLogger logger = {0};
  if (start_logger(&cfg.mslcfg, &logger) < 0) {
    log_str(&cfg_logger, PSTR("Could not initialize mqtt sn logger\n"));
  }
  mqtt_sn_logger = &logger;
#endif

  MqttSnClient publish_client = {0};
  void *gatewayNetworkContext = NULL;
  rc = start_publish_client(&cfg, mqtt_sn_logger, &publish_client, gatewayNetworkContext);

  if (mqtt_sn_logger) {
    MqttSnLoggerDeinit(mqtt_sn_logger);
  }
  publish_client_config_cleanup(&cfg);

  return rc;
}

int32_t start_logger(const mqtt_sn_logger_config *cfg,
                     MqttSnLogger *logger) {
  if (!strcmp(cfg->log_target, "console")) {
    if (cfg->log_file_path != NULL) {
      if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_stdout_log_init, NULL) < 0) {
        return -1;
      }
    } else {
      if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
        return -1;
      }
    }
  } else if (!strcmp(cfg->log_target, "file")) {
    if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_log_init, NULL) < 0) {
      return -1;
    }
  }
  if (cfg->log_identifier) {
    logger->log_identifier = cfg->log_identifier;
  }
  return 0;
}
