//
// Created by SomeDude on 27.05.2019.
//

#include "config_command_helper.h"
#include <logging/MqttSnLoggingInterface.h>
#include <platform/platform_compatibility.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

int32_t contains_config_argument_help(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "--help")) {
      return 1;
    }
  }
  return 0;
}
int32_t get_config_argument_file_index(int argc, char **argv, const MqttSnLogger *logger) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config-file")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "config file");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        return i;

      }
    }
  }
  return 0;
}
int32_t process_config_file(const char *config_file_path,
                            const MqttSnLogger *logger,
                            void *cfg,
                            int32_t (*callback)(void *, const MqttSnLogger *, int, char **),
                            const char *argv_0) {
#if defined(Arduino_h) && defined(__SD_H__)
  File config_file = SD.open(config_file_path, FILE_READ);
  if (!config_file) {
    print_could_not_read_config_file(logger, "could not open file");
    config_file.close();
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  uint16_t len = 1024;
  if (config_file.size() > len) {
    print_could_not_read_config_file(fcfg->logger, "line too long");
    config_file.close();
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  while (config_file.available()) {
    char line[1024] = {0};
    int len = config_file.read(&line, len);
    if (process_config_file_line(logger, line, len, argv_0, cfg, callback)) {
      config_file.close();
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  }
  config_file.close();
#elif defined(linux)
  FILE *config_file = fopen(config_file_path, "r");
  if (!config_file) {
    print_could_not_read_config_file(logger, strerror(errno));
    fclose(config_file);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  char *line;
  size_t len = 0;
  ssize_t read;
  // ESP8266   while ((read = getline(&line, &len, config_file)) != -1) {
  // while ((read = __getline(&line, &len, config_file)) != -1) {
  while ((read = getline(&line, &len, config_file)) != -1) {
    log_str(logger, line);
    log_flush(logger);
    log_uint16(logger, len);
    log_flush(logger);

    if (process_config_file_line(logger, line, len, argv_0, cfg, callback) == MQTT_SN_PARSE_CONFIG_FAILURE) {
      fclose(config_file);
      print_could_not_read_config_file(logger, strerror(errno));
      if (line) {
        free(line);
      }
      return MQTT_SN_PARSE_CONFIG_FAILURE;
    }
  }
  fclose(config_file);
  if (line) {
    free(line);
  }
#endif
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t process_config_file_line(const MqttSnLogger *logger,
                                 char *line,
                                 size_t len,
                                 const char *argv_0,
                                 void *cfg,
                                 int32_t (*callback)(void *, const MqttSnLogger *, int, char **)) {
  if (line == NULL) {
    assert(!(line == NULL));
    return -1;
  }
  if (len == 0) {
    assert(!(len == 0));
    return -1;
  }

  if (strlen(line) + 1 != len) {
    assert(!(strlen(line) + 1 != len));
    return -1;
  }

  if (strlen(line) == 0) {
    assert(!(strlen(line) == 0));
    return -1;
  }

  int tk_count = 0;
  {
    char line_copy[len];
    memcpy(line_copy, line, len);
    // estimates argc
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      tk_count++;
    }
  }
  char *argv[tk_count];
  int argc = 0;
  char line_copy[len];
  {
    memcpy(line_copy, line, len);
    argv[argc++] = (char *) argv_0;
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      argv[argc++] = tk;
    }
  }

  // remove '\n' from tokens
  for (uint16_t i = 1; i < argc; i++) {
    if (argv[i][(strlen(argv[i]) - 1)] == '\n') {
      argv[i][(strlen(argv[i]) - 1)] = '\0';
    }
  }

#ifdef WITH_DEBUG_LOGGING
  print_argc_argv(logger, argc, argv);
#endif

  return callback(cfg, logger, argc, argv);
}
int32_t parse_port(const MqttSnLogger *logger, char *port_str, int32_t *dst) {
  char *endprt;
  long int n = strtol(port_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > UINT16_MAX)) {
    print_config_parser_invalid_port_given(logger, n);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  *dst = n;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t parse_duration(const MqttSnLogger *logger, char *duration_str, uint16_t *duration_dst) {
  char *endprt;
  long int n = strtol(duration_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < 0 || n > UINT16_MAX)) {
    print_invalid_duration_given(logger, duration_str);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  *duration_dst = n;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t parse_radius(const MqttSnLogger *logger, char *radius_str, uint8_t *radius_dst) {
  char *endprt;
  long int n = strtol(radius_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < 0 || n > UINT8_MAX)) {
    print_invalid_radius_given(logger, radius_str);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  *radius_dst = n;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t parse_gateway_id(const MqttSnLogger *logger, char *gateway_id_str, uint8_t *gateway_id_dst) {
  char *endprt;
  long int n = strtol(gateway_id_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < 0 || n > UINT8_MAX)) {
    print_invalid_gateway_id_given(logger, gateway_id_str);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  *gateway_id_dst = n;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t parse_timeout(const MqttSnLogger *logger, char *timeout_str, int32_t *dst) {
  char *endprt;
  long int n = strtol(timeout_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > INT32_MAX)) {
    print_invalid_timeout_given(logger, n);
    return -1;
  }
  *dst = n;
  return 0;
}
int32_t parse_topic_id(const MqttSnLogger *logger, char *topic_id_str, uint16_t *dst) {
  char *endprt;
  long int n = strtol(topic_id_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < 0 || n > UINT16_MAX)) {
    print_invalid_topic_id_given(logger, topic_id_str);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  *dst = n;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t print_invalid_retain_given(const MqttSnLogger *logger, const char *given_str) {
  log_str(logger, PSTR("Error: Invalid retain given: "));
  log_str(logger, given_str);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_invalid_clean_session_given(const MqttSnLogger *logger, const char *given_str) {
  log_str(logger, PSTR("Error: Invalid clean session given: "));
  log_str(logger, given_str);
  log_flush(logger);
  return log_status(logger);
}
int32_t parse_retain(const MqttSnLogger *logger, char *retain_str, uint8_t *dst) {
  if (!strcmp(retain_str, "false")) {
    (*dst) = 0;
  } else if (!strcmp(retain_str, "true")) {
    (*dst) = 1;
  } else {
    print_invalid_retain_given(logger, retain_str);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t parse_clean_session(const MqttSnLogger *logger, char *clean_session_str, uint8_t *dst) {
  if (!strcmp(clean_session_str, "false")) {
    (*dst) = 0;
  } else if (!strcmp(clean_session_str, "true")) {
    (*dst) = 1;
  } else {
    print_invalid_clean_session_given(logger, clean_session_str);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t print_invalid_topic_id_given(const MqttSnLogger *logger, const char *given_str) {
  log_str(logger, PSTR("Error: Invalid topic id given: "));
  log_str(logger, given_str);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_invalid_duration_given(const MqttSnLogger *logger, const char *given_str) {
  log_str(logger, PSTR("Error: Invalid duration given: "));
  log_str(logger, given_str);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_invalid_radius_given(const MqttSnLogger *logger, const char *given_str) {
  log_str(logger, PSTR("Error: Invalid radius given: "));
  log_str(logger, given_str);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_invalid_gateway_id_given(const MqttSnLogger *logger, const char *given_str) {
  log_str(logger, PSTR("Error: Invalid gateway id given: "));
  log_str(logger, given_str);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_config_parser_invalid_port_given(const MqttSnLogger *logger, long invalid_port) {
  log_str(logger, PSTR("Error: Invalid port given: "));
  log_uint16(logger, invalid_port);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_config_parser_invalid_qos_given(const MqttSnLogger *logger, uint64_t given_qos) {
  log_str(logger, PSTR("Error: Invalid qos given: "));
  log_uint64(logger, given_qos);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_argument_value_not_specified(const MqttSnLogger *logger,
                                           const char *argument,
                                           const char *argument_name) {
  log_str(logger, PSTR("Error: "));
  log_str(logger, argument);
  log_str(logger, PSTR(" argument given but no "));
  log_str(logger, argument_name);
  log_str(logger, PSTR(" specified."));
  log_flush(logger);
  return log_status(logger);
}
int32_t print_unsupported_url_scheme(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Error: unsupported URL scheme."));
  log_flush(logger);
  return log_status(logger);
}
int32_t print_invalid_timeout_given(const MqttSnLogger *logger, long timeout) {
  log_str(logger, PSTR("Error: Invalid port given: "));
  log_uint16(logger, timeout);
  log_flush(logger);
  return log_status(logger);
}
int32_t print_invalid_protocol_version_given(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Error: Invalid protocol id version given."));
  log_flush(logger);
  return log_status(logger);
}
int32_t print_argc_argv(const MqttSnLogger *logger, int argc, char *argv[]) {
  log_str(logger, PSTR("argc: "));
  log_uint64(logger, argc);
  log_str(logger, PSTR(" argv: "));
  for (uint16_t i = 0; i < argc; i++) {
    log_str(logger, argv[i]);
    if (i + 1 < argc) {
      log_str(logger, PSTR(", "));
    }
  }
  log_flush(logger);
  return log_status(logger);
}
int32_t print_show_help(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" --help : display this message.\n"));
  return log_status(logger);
}
int32_t print_unknown_option(const MqttSnLogger *logger, const char *unknown_option) {
  log_str(logger, PSTR("Error: Unknown option "));
  log_str(logger, unknown_option);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int32_t print_could_not_read_config_file(const MqttSnLogger *logger, char *strerror) {
  log_str(logger, PSTR("Error: Could not read config file: "));
  log_str(logger, strerror);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int32_t print_config_list_full(const MqttSnLogger *logger, char *arg_list, uint16_t max_arg_list_len) {
  log_str(logger, PSTR("Error: Cannot save more than "));
  log_uint16(logger, max_arg_list_len);
  log_str(logger, PSTR(" "));
  log_str(logger, arg_list);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int32_t parse_client_subscription_config_qos(const MqttSnLogger *logger, char *qos_str, int8_t *qos_dst) {
  char *endprt;
  long int n = strtol(qos_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > 2)) {
    print_config_parser_invalid_qos_given(logger, n);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  *qos_dst = n;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t parse_client_publish_config_qos(const MqttSnLogger *logger, char *qos_str, int8_t *qos_dst) {
  char *endprt;
  long int n = strtol(qos_str, &endprt, 10);
  if ((errno == EOVERFLOW) || (*endprt != '\0') || (n < -1 || n > 2)) {
    print_config_parser_invalid_qos_given(logger, n);
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  *qos_dst = n;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
int32_t print_see_usage(const MqttSnLogger *logger, const char *executable_name) {
  log_str(logger, PSTR("Use "));
  log_str(logger, executable_name);
  log_str(logger, PSTR(" --help' to see usage."));
  return log_status(logger);
}

