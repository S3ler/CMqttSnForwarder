//
// Created by SomeDude on 09.04.2019.
//

#include <time.h>
#include "MqttSnForwarderLogging.h"
#include <string.h>
#include <stdint.h>
#include <logging/linux/stdout/StdoutLogging.h>

int log_uint8(const MqttSnLogger *logger, uint8_t n) {
  const char *uint8_max_str = "255";
  int uint8_max_str_length = strlen(uint8_max_str);
  char n_str[uint8_max_str_length + 1];
  snprintf((char *) &n_str, uint8_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uint32(const MqttSnLogger *logger, uint32_t n) {
  const char *uint32_max_str = "4294967295"; // uint32_t 4294967295
  int uint32_max_str_length = strlen(uint32_max_str);
  char n_str[uint32_max_str_length + 1];
  snprintf((char *) &n_str, uint32_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uintmax(const MqttSnLogger *logger, uintmax_t n) {
  const char *uintmax_str = "18446744073709551615"; // uintmax_t 18446744073709551615
  int uintmax_str_length = strlen(uintmax_str);
  char t_str[uintmax_str_length + 1];
  snprintf((char *) &t_str, uintmax_str_length + 1, "%ju", n);
  return log_str(logger, t_str);
}

int get_timestamp(uint64_t *t) {
  time_t result = time(NULL);
  if (result == -1) {
    return -1;
  }
  *t = result;
  return 0;
}

int log_current_time(const MqttSnLogger *logger) {
  uintmax_t t;
  if (get_timestamp(&t)) {
    return -1;
  }
  return log_uintmax(logger, t);
}

int log_device_address(const MqttSnLogger *logger, const device_address *address) {
  const char *dot = ".";
  for (size_t i = 0; i < sizeof(device_address); ++i) {
    if (log_uint8(logger, address->bytes[i])) {
      return -1;
    }
    if (i + 1 < sizeof(device_address)) {
      if (log_str(logger, dot)) {
        return -1;
      }
    }
  }
  return 0;
}

int log_forwarder_started(const MqttSnLogger *logger,
                          int level,
                          const char *version,
                          int major,
                          int minor,
                          int tweak,
                          const char *build_date) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *is = " is ";
  const char *dot = ".";
  const char *build_date_str = " (build date ";
  const char *action = ") started.";

  return (log_current_time(logger) ||
      log_str(logger, name_version) ||
      log_str(logger, version) ||
      log_str(logger, is) ||
      log_uint32(logger, major) ||
      log_str(logger, dot) ||
      log_uint32(logger, minor) ||
      log_str(logger, dot) ||
      log_uintmax(logger, tweak) ||
      log_str(logger, build_date_str) ||
      log_str(logger, build_date) ||
      log_str(logger, action) ||
      log_flush(logger) != 0);
}
int log_forwarder_terminated(const MqttSnLogger *logger,
                             int level,
                             const char *version,
                             uint32_t major,
                             uint32_t minor,
                             uintmax_t tweak) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *is = " is ";
  const char *dot = ".";
  const char *action = " terminated.";

  return (log_current_time(logger) ||
      log_str(logger, name_version) ||
      log_str(logger, version) ||
      log_str(logger, is) ||
      log_uint32(logger, major) ||
      log_str(logger, dot) ||
      log_uint32(logger, minor) ||
      log_str(logger, dot) ||
      log_uintmax(logger, tweak) ||
      log_str(logger, action) ||
      log_flush(logger) != 0);
}
int MqttSnLoggerInit(MqttSnLogger *logger) {
  logger->log_init = stdout_log_init;
  logger->log_deinit = stdout_log_deinit;
  logger->log_flush = stdout_log_flush;
  logger->log_str = stdout_log_str;
  // TODO exchange with fcfg->log_level
  logger->log_level = LOG_LEVEL_DEFAULT;
  return logger->log_init(logger);
}

void MqttSnLoggerDeinit(MqttSnLogger *logger) {
  logger->log_deinit(logger);
}

int log_flush(const MqttSnLogger *logger) {
  return logger->log_flush(logger);
}

int log_str(const MqttSnLogger *logger, const char *str) {
  return logger->log_str(str);
}
