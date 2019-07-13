#include <stdio.h>
#include <stdlib.h>
#include "forwarder_starter.h"
#include <platform/platform_compatibility.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>

int32_t start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger);

int main(int argc, char *argv[]) {
  MqttSnLogger cfg_logger = {0};

  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  forwarder_config fcfg = {0};
  if (forwarder_config_init(&fcfg)) {
    return EXIT_FAILURE;
  }
  int rc = forwarder_config_process_line(&fcfg, &cfg_logger, argc, argv);
  if (rc) {
    if (rc == 2) {
      forwarder_config_print_usage(&cfg_logger);
    } else {
      forwarder_config_print_see_usage(&cfg_logger);
      forwarder_config_cleanup(&fcfg);
      return EXIT_FAILURE;
    }
    forwarder_config_cleanup(&fcfg);
    return EXIT_SUCCESS;
  }

  MqttSnLogger *mqtt_sn_logger = NULL;
#ifdef WITH_LOGGING
  MqttSnLogger logger = {0};
  if (start_logger(&fcfg.mslcfg, &logger) < 0) {
    log_str(&cfg_logger, PSTR("Could not initialize mqtt sn logger\n"));
  }
  mqtt_sn_logger = &logger;
#endif

  MqttSnForwarder mqttSnForwarder = {0};
  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;
  rc = start_forwarder(&fcfg, mqtt_sn_logger, &mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
  forwarder_config_cleanup(&fcfg);

  if (mqtt_sn_logger) {
    MqttSnLoggerDeinit(mqtt_sn_logger);
  }

  return rc;
}

int32_t start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger) {
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
