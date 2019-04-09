//
// Created by SomeDude on 09.04.2019.
//

#include <time.h>
#include "MqttSnForwarderLogging.h"
#include <string.h>
#include <stdint.h>

int log_flush() {
  if (log_str("\n") || fflush(stdout) == EOF) {
    return -1;
  }
  return 0;
}

int log_str(const char *str) {
  if (fputs(str, stdout) == EOF) {
    return -1;
  }
  return 0;
}

int log_uint8(uint8_t n) {
  const char *uint8_max_str = "255";
  int uint8_max_str_length = strlen(uint8_max_str);
  char n_str[uint8_max_str_length + 1];
  snprintf((char *) &n_str, uint8_max_str_length + 1, "%u", n);
  return log_str(n_str);
}

int log_uint32(uint32_t n) {
  const char *uint32_max_str = "4294967295"; // uint32_t 4294967295
  int uint32_max_str_length = strlen(uint32_max_str);
  char n_str[uint32_max_str_length + 1];
  snprintf((char *) &n_str, uint32_max_str_length + 1, "%u", n);
  return log_str(n_str);
}

int log_uintmax(uintmax_t n) {
  const char *uintmax_str = "18446744073709551615"; // uintmax_t 18446744073709551615
  int uintmax_str_length = strlen(uintmax_str);
  char t_str[uintmax_str_length + 1];
  snprintf((char *) &t_str, uintmax_str_length + 1, "%ju", n);
  return log_str(t_str);
}

int get_timestamp(uint64_t *t) {
  time_t result = time(NULL);
  if (result == -1) {
    return -1;
  }
  *t = result;
  return 0;
}

int log_current_time() {
  uintmax_t t;
  if (get_timestamp(&t)) {
    return -1;
  }
  return log_uintmax(t);
}

int log_device_address(const device_address *address) {
  const char *dot = ".";
  for (size_t i = 0; i < sizeof(device_address); ++i) {
    if (log_uint8(address->bytes[i])) {
      return -1;
    }
    if (i + 1 < sizeof(device_address)) {
      if (log_str(dot)) {
        return -1;
      }
    }
  }
  return 0;
}

int log_forwarder_started(int level, const char *VERSION, int major, int minor, int revision) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *dot = ".";
  const char *build_date = " (build date ";
  const char *action = ") starting.";

  return (log_current_time() ||
      log_str(name_version) ||
      log_str(VERSION) ||
      log_str(dot) ||
      log_uint32(major) ||
      log_str(dot) ||
      log_uint32(minor) ||
      log_str(build_date) ||
      log_uintmax(revision) ||
      log_str(action) ||
      log_flush() != 0);
}
int log_forwarder_terminated(int level, const char *VERSION, uint32_t major, uint32_t minor, uintmax_t revision) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *dot = ".";
  const char *action = " terminated.";

  return (log_current_time() ||
      log_str(name_version) ||
      log_str(VERSION) ||
      log_str(dot) ||
      log_uint32(major) ||
      log_str(dot) ||
      log_uint32(minor) ||
      log_str(action) ||
      log_flush() != 0);
}
