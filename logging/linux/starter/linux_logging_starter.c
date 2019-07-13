//
// Created by SomeDude on 12.07.2019.
//

#include "linux_logging_starter.h"

int32_t start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger) {
  if (!strcmp(cfg->log_target, "console")) {
    if (cfg->log_file_path != NULL) {
      if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_stdout_log_init) < 0) {
        return -1;
      }
    } else {
      if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
        return -1;
      }
    }
  } else if (!strcmp(cfg->log_target, "file")) {
    if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_log_init) < 0) {
      return -1;
    }
  }
  if (cfg->log_identifier) {
    logger->log_identifier = cfg->log_identifier;
  }
  return 0;
}