
#include "MqttSnForwarderLogging.h"
#include <platform/platform_compatibility.h>

int log_forwarder_started(const MqttSnLogger *logger,
                          const char *version,
                          int major,
                          int minor,
                          int tweak,
                          const char *build_date) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  log_msg_start(logger);
  log_str(logger, PSTR("cmqttsnforwarder version "));
  log_str(logger, version);
  log_str(logger, PSTR(" is "));
  log_uint32(logger, major);
  log_str(logger, PSTR("."));
  log_uint32(logger, minor);
  log_str(logger, PSTR("."));
  log_uintmax(logger, tweak);
  log_str(logger, PSTR(" (build date "));
  log_str(logger, build_date);
  log_str(logger, PSTR(") started."));
  log_flush(logger);
  return log_status(logger);
}

int log_forwarder_terminated(const MqttSnLogger *logger,
                             const char *version,
                             uint32_t major,
                             uint32_t minor,
                             uintmax_t tweak) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  log_msg_start(logger);
  log_str(logger, PSTR("cmqttsnforwarder version "));
  log_str(logger, version);
  log_str(logger, PSTR(" is "));
  log_uint32(logger, major);
  log_str(logger, PSTR("."));
  log_uint32(logger, minor);
  log_str(logger, PSTR("."));
  log_uintmax(logger, tweak);
  log_str(logger, PSTR(" terminated."));
  log_flush(logger);
  return log_status(logger);
}
