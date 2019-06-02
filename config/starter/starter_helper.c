//
// Created by SomeDude on 31.05.2019.
//

#include <platform/platform_compatibility.h>
#include "starter_helper.h"

int32_t print_program_started(const MqttSnLogger *logger,
                              const mqtt_sn_version_config *msvcfg,
                              const char *program_name) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }

  log_msg_start(logger);
  log_str(logger, program_name);
  log_str(logger, PSTR(" version "));
  log_str(logger, msvcfg->version);
  log_str(logger, PSTR(" is "));
  log_uint32(logger, msvcfg->major);
  log_str(logger, PSTR("."));
  log_uint32(logger, msvcfg->minor);
  log_str(logger, PSTR("."));
  log_uintmax(logger, msvcfg->tweak);
  log_str(logger, PSTR(" (build date "));
  log_str(logger, msvcfg->build_date);
  log_str(logger, PSTR(") started."));
  log_flush(logger);

  return log_status(logger);
}
int32_t print_program_terminated(const MqttSnLogger *logger,
                                 const mqtt_sn_version_config *msvcfg,
                                 const char *program_name) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }

  log_msg_start(logger);
  log_str(logger, program_name);
  log_str(logger, PSTR(" version "));
  log_str(logger, msvcfg->version);
  log_str(logger, PSTR(" is "));
  log_uint32(logger, msvcfg->major);
  log_str(logger, PSTR("."));
  log_uint32(logger, msvcfg->minor);
  log_str(logger, PSTR("."));
  log_uintmax(logger, msvcfg->tweak);
  log_str(logger, PSTR(" terminated."));
  log_flush(logger);

  return log_status(logger);
}
