//
// Created by SomeDude on 10.04.2019.
//

#include "MqttSnPluginLogger.h"

int log_dlerror(const MqttSnLogger *logger, int level, const char *error) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }
  const char *d = ": ";
  if (log_current_time(logger)) {
    return -1;
  }
  if (log_str(logger, d)) {
    return -1;
  }
  if (log_str(logger, error)) {
    return -1;
  }
  if (log_flush(logger)) {
    return -1;
  }

  return 0;
}

int log_max_msg_length_mismatch(const MqttSnLogger *logger, int level, uint16_t gw_max, uint16_t plugin_max) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }
  const char *mismatch = ": maximum message length mismatch - forwarder: ";
  const char *plugin_max_str = " plugin: ";
  const char *dot = ".";

  if (log_current_time(logger)) {
    return -1;
  }
  if (log_str(logger, mismatch)) {
    return -1;
  }
  if (log_uint16(logger, gw_max)) {
    return -1;
  }
  if (log_str(logger, plugin_max_str)) {
    return -1;
  }
  if (log_uint16(logger, plugin_max)) {
    return -1;
  }
  if (log_str(logger, dot)) {
    return -1;
  }
  return 0;
}
