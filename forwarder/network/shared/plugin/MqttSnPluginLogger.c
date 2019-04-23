//
// Created by SomeDude on 10.04.2019.
//

#include <logging/MqttSnForwarderLoggingBasic.h>
#include "MqttSnPluginLogger.h"

int log_dlerror(const MqttSnLogger *logger, int level, const char *error) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  log_msg_start(logger);
  log_str(logger, error);
  log_flush(logger);
  return log_status(logger);
}

int log_max_msg_length_mismatch(const MqttSnLogger *logger, int level, uint16_t gw_max, uint16_t plugin_max) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  const char *mismatch = "maximum message length mismatch - forwarder: ";
  const char *plugin_max_str = " plugin: ";
  const char *dot = ".";

  log_msg_start(logger);
  log_str(logger, mismatch);
  log_uint16(logger, gw_max);
  log_str(logger, plugin_max_str);
  log_uint16(logger, plugin_max);
  log_str(logger, dot);
  log_flush(logger);
  return log_status(logger);

}
