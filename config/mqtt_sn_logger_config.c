//
// Created by SomeDude on 28.05.2019.
//

#include <platform/platform_compatibility.h>
#include "mqtt_sn_logger_config.h"

int32_t mqtt_sn_logger_config_init(mqtt_sn_logger_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));
  cfg->log_lvl = MQTT_SN_LOGGER_DEFAULT_LOG_LVL;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void mqtt_sn_logger_config_cleanup(mqtt_sn_logger_config *cfg) {

}
int32_t is_mqtt_sn_logger_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-q") || !strcmp(arg, "--quiet")) {
    return 1;
  } else if (!strcmp(arg, "-d") || !strcmp(arg, "--default")) {
    return 1;
  } else if (!strcmp(arg, "-v") || !strcmp(arg, "--verbose")) {
    return 1;
  }
#if defined(WITH_DEBUG_LOGGING)
  else if (!strcmp(arg, "-db") || !strcmp(arg, "--debug")) {
    return 1;
  }
#endif

  return 0;
}
int32_t mqtt_sn_logger_config_process_args(mqtt_sn_logger_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char **argv) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
      cfg->log_lvl = LOG_LEVEL_QUIET;
    } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--default")) {
      cfg->log_lvl = LOG_LEVEL_DEFAULT;
    } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
      cfg->log_lvl = LOG_LEVEL_VERBOSE;
    }
#if defined(WITH_DEBUG_LOGGING)
    else if (!strcmp(argv[i], "-db") || !strcmp(argv[i], "--debug")) {
      cfg->log_lvl = LOG_LEVEL_DEBUG;
    }
#endif
  }

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void mqtt_sn_logger_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
#ifndef WITH_DEBUG_LOGGING
  log_str(logger, PSTR("[[-q quiet] | [-d default] | [-v verbose]]\n"));
#else
  log_str(logger, PSTR("[[-q quiet] | [-d default] | [-v verbose] | [-db debug]]\n"));
#endif
}
void mqtt_sn_logger_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -q : specify quiet logging. Don't print any log messages.\n"));
  log_str(logger, PSTR(" -d : specify default logging. Enables default log messages.\n"));
  //log_str(logger, PSTR(" -d : specify default logging. Print network status changes, and the mqtt-sn messages: PUBLISH, CONNECT, CONNACK, DISCONNECT.\n"));
  log_str(logger, PSTR(" -v : specify verbose logging. Enables verbose log messages.\n"));
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger, PSTR(" -db : specify debug logging. Enables debug log messages.\n"));
#endif
}

