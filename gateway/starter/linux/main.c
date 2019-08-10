#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <gateway/config/gateway_config.h>
#include <platform/platform_compatibility.h>
#include <gateway/MqttSnGateway.h>
#include "gateway_starter.h"
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>

int32_t start_logger(const mqtt_sn_logger_config *cfg,
                     MqttSnLogger *logger);

int main(int argc, char *argv[]) {
  MqttSnLogger cfg_logger = {0};

  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  mqtt_sn_gateway__config egwcfg = {0};
  if (mqtt_sn_gateway__config_init(&egwcfg)) {
    return EXIT_FAILURE;
  }
  int rc = process_mqtt_sn_gateway__config_line(&egwcfg, &cfg_logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      mqtt_sn_gateway__config_print_usage(&cfg_logger);
    } else {
      mqtt_sn_gateway__config_print_see_usage(&cfg_logger);
      mqtt_sn_gateway__config_cleanup(&egwcfg);
      return EXIT_FAILURE;
    }
    mqtt_sn_gateway__config_cleanup(&egwcfg);
    return EXIT_SUCCESS;
  }

  MqttSnLogger *mqtt_sn_logger = NULL;
#ifdef WITH_LOGGING
  MqttSnLogger logger = {0};
  if (start_logger(&egwcfg.mslcfg, &logger) < 0) {
    log_str(&cfg_logger, PSTR("Could not initialize mqtt sn logger\n"));
  }
  mqtt_sn_logger = &logger;
#endif

  MqttSnGateway egw = {0};
  void *clientNetworkContext = NULL;
  rc = start_mqtt_sn_gateway(&egwcfg, mqtt_sn_logger, &egw, clientNetworkContext);
  mqtt_sn_gateway__config_cleanup(&egwcfg);

  if (mqtt_sn_logger) {
    MqttSnLoggerDeinit(mqtt_sn_logger);
  }

  mqtt_sn_gateway__config_cleanup(&egwcfg);
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
