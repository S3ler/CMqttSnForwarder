//
// Created by SomeDude on 28.05.2019.
//

#include <platform/platform_compatibility.h>
#include "mqtt_sn_logger_config.h"
#include "config_command_helper.h"

int32_t mqtt_sn_logger_config_init(mqtt_sn_logger_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));
  memcpy(cfg->default_logging_target,
         MQTT_SN_LOGGER_DEFAULT_LOGGING_TARGET,
         sizeof(MQTT_SN_LOGGER_DEFAULT_LOGGING_TARGET));
  cfg->log_target = cfg->default_logging_target;
  cfg->log_filepath = NULL;
  cfg->log_identifier = NULL;
  cfg->log_lvl = MQTT_SN_LOGGER_DEFAULT_LOG_LVL;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void mqtt_sn_logger_config_cleanup(mqtt_sn_logger_config *cfg) {

}
int32_t is_mqtt_sn_logger_config_command(const char *arg, int *i) {

  if (!strcmp(arg, "-li") || !strcmp(arg, "--log_identifier")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-lt") || !strcmp(arg, "--log_target")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-lfp") || !strcmp(arg, "--log_file_path")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-q") || !strcmp(arg, "--quiet")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-d") || !strcmp(arg, "--default")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-v") || !strcmp(arg, "--verbose")) {
    (*i)++;
    return 1;
  }
#if defined(WITH_DEBUG_LOGGING)
  else if (!strcmp(arg, "-db") || !strcmp(arg, "--debug")) {
    (*i)++;
    return 1;
  }
#endif
  return 0;
}
int32_t mqtt_sn_logger_config_process_args(mqtt_sn_logger_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char **argv) {
  int32_t parsed_args = 0;
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-li") || !strcmp(argv[i], "--log_identifier")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "log identifier");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->log_identifier = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    }
    if (!strcmp(argv[i], "-lt") || !strcmp(argv[i], "--log_target")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "log target");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (!strcmp(argv[i + 1], "console") || strcmp(argv[i + 1], "path")) {
          cfg->log_target = argv[i + 1];
        } else {
          print_config_parser_invalid_log_target(logger, argv[i + 1]);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-lfp") || !strcmp(argv[i], "--log_file_path")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "log file path");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->log_filepath = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
      cfg->log_lvl = LOG_LEVEL_QUIET;
      parsed_args += 1;
    } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--default")) {
      cfg->log_lvl = LOG_LEVEL_DEFAULT;
      parsed_args += 1;
    } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
      cfg->log_lvl = LOG_LEVEL_VERBOSE;
      parsed_args += 1;
    }
#if defined(WITH_DEBUG_LOGGING)
    else if (!strcmp(argv[i], "-db") || !strcmp(argv[i], "--debug")) {
      cfg->log_lvl = LOG_LEVEL_DEBUG;
      parsed_args += 1;
    }
#endif
  }

  if (!strcmp(cfg->log_target, "file") && cfg->log_filepath == NULL) {
    print_argument_value_not_specified(logger, "log target file", "log file path");
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  return parsed_args;
}
void mqtt_sn_logger_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger,
          PSTR("[[-li log_identifier] [-lt log_target] [-lfp log_file_path] [-q quiet] | [-d default] | [-v verbose]"));
#ifdef WITH_DEBUG_LOGGING
  log_str(logger, PSTR(" | [-db debug]"));
#endif
  log_str(logger, PSTR("]\n"));
}
void mqtt_sn_logger_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -li : specify the log identifier.\n"));
  log_str(logger, PSTR(" -lt : specify the log target. Can be: console, file, consolefile. Defaults to "));
  log_str(logger, PSTR(MQTT_SN_LOGGER_DEFAULT_LOGGING_TARGET));
  log_str(logger, PSTR("\n"));
  log_str(logger, PSTR("     : When using file as log target the logfile path (-lfp) must be set.\n"));
  log_str(logger, PSTR(" -lfp : specify the logfile path. Logs to the logfile additional to the logging target.\n"));
  log_str(logger, PSTR(" -q : specify quiet logging. Does not print any log messages.\n"));
  log_str(logger, PSTR(" -d : specify default logging. Enables default log messages.\n"));
  //log_str(logger, PSTR(" -d : specify default logging. Print network status changes, and the mqtt-sn messages: PUBLISH, CONNECT, CONNACK, DISCONNECT.\n"));
  log_str(logger, PSTR(" -v : specify verbose logging. Enables verbose log messages.\n"));
#if defined(WITH_DEBUG_LOGGING)
  log_str(logger, PSTR(" -db : specify debug logging. Enables debug log messages.\n"));
#endif
}
int32_t mqtt_sn_logger_config_buffer_copy_to_buffer(mqtt_sn_logger_config *cfg,
                                                    mqtt_sn_logger_config_buffer *cfg_buffer) {
  if (cfg->log_target) {
    if (strlen(cfg->log_target) + 1 > MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_TARGET_LENGTH) {
      return -1;
    }
    strcpy(cfg_buffer->log_target, cfg->log_target);
    cfg->log_target = cfg_buffer->log_target;
  }
  if (cfg->log_filepath) {
    if (strlen(cfg->log_filepath) + 1 > MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_file_path_LENGTH) {
      return -1;
    }
    strcpy(cfg_buffer->log_file_path, cfg->log_filepath);
    cfg->log_filepath = cfg_buffer->log_file_path;
  }
  if (cfg->log_identifier) {
    if (strlen(cfg->log_identifier) + 1 > MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_identifier_LENGTH) {
      return -1;
    }
    strcpy(cfg_buffer->log_identifier, cfg->log_identifier);
    cfg->log_identifier = cfg_buffer->log_identifier;
  }
  return 0;
}
void mqtt_sn_logger_config_print_usage(const MqttSnLogger *logger) {
    mqtt_sn_logger_config_print_usage_short(logger, NULL);
    mqtt_sn_logger_config_print_usage_long(logger);
}
