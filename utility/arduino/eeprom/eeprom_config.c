//
// Created by SomeDude on 01.05.2019.
//

#include "eeprom_config.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <forwarder/logging/MqttSnForwarderLoggingBasic.h>
#include <CMqttSnForwarderArduino.h>
#include <platform/platform_compatibility.h>

void eeprom_config_init(EEPROM_cfg **ecfg) {
  assert(*ecfg == NULL);
  EEPROM.begin(EEPROM_DATA_SIZE);
  *ecfg = (EEPROM_cfg *) EEPROM.getDataPtr();
}
void eeprom_config_cleanup(EEPROM_cfg **ecfg) {
  assert(*ecfg != NULL);
  assert(*ecfg == (EEPROM_cfg *) EEPROM.getDataPtr());
  EEPROM.end();
  *ecfg = NULL;
}
int eeprom_config_clear(EEPROM_cfg *ecfg) {
  assert(ecfg != NULL);
  assert(ecfg == (EEPROM_cfg *) EEPROM.getDataPtr());
  if (ecfg->locked) {
    return -2;
  }
  ecfg->locked = 0;
  memcpy(ecfg->fcfg_name, EEPROM_FCFG_NAME, sizeof(EEPROM_FCFG_NAME));
  memset(ecfg->fcfg_line, 0, EEPROM_LINE_SIZE);
  EEPROM.commit();
  return 0;
}
void eeprom_lock(EEPROM_cfg *ecfg) {
  assert(ecfg != NULL);
  assert(ecfg == (EEPROM_cfg *) EEPROM.getDataPtr());
  ecfg->locked = 1;
  EEPROM.commit();
}
void eeprom_unlock(EEPROM_cfg *ecfg) {
  assert(ecfg != NULL);
  assert(ecfg == (EEPROM_cfg *) EEPROM.getDataPtr());
  ecfg->locked = 0;
  EEPROM.commit();
}
int eeprom_config_set(EEPROM_cfg *ecfg, const char *fcfg_line, size_t fcfg_line_len) {
  assert(ecfg != NULL);
  assert(ecfg == (EEPROM_cfg *) EEPROM.getDataPtr());
  if (ecfg->locked) {
    return -2;
  }
  if (fcfg_line_len > EEPROM_LINE_SIZE) {
    return -1;
  }
  memcpy(ecfg->fcfg_line, fcfg_line, fcfg_line_len);
  EEPROM.commit();
  return 0;
}
int eeprom_load_forwarder_config(EEPROM_cfg *ecfg, forwarder_config *fcfg) {
  assert(ecfg != NULL);
  assert(ecfg == (EEPROM_cfg *) EEPROM.getDataPtr());
  assert(fcfg != NULL);
  assert(fcfg->logger != NULL);
#ifdef WITH_LOGGING
  print_loading_fcfg_from_eeprom(fcfg->logger);
#endif

  if (strlen(ecfg->fcfg_line) == 0) {
    return 0;
  }
  int fcfg_rc = process_forwarder_config_str(fcfg, ecfg->fcfg_line,
                                             strlen(ecfg->fcfg_line) + 1, ecfg->fcfg_name);

  if (fcfg_rc == FORWARDER_CONFIG_HELP) {
    print_usage(fcfg->logger);
  } else if (fcfg_rc == 0) {
    return fcfg_rc;
  }
  // else
#ifdef WITH_LOGGING
  print_invalid_fcfg_clearing_eeprom(fcfg->logger);
#endif
  if (eeprom_config_clear(ecfg) == -2) {
    print_failed_to_clear_eeprom_locked(fcfg->logger);
    return -2;
  }
  return fcfg_rc;
}

int eeprom_save_forwarder_config_line(EEPROM_cfg *ecfg, forwarder_config *fcfg, char *fcfg_line, size_t fcfg_line_len) {
  assert(fcfg != NULL);
  assert(fcfg->logger != NULL);

  char *argv_0 = "fcfg_line";
  char line_copy[fcfg_line_len];
  memcpy(line_copy, fcfg_line, fcfg_line_len);

  int fcfg_rc = process_forwarder_config_str(fcfg, fcfg_line, fcfg_line_len, argv_0);
  if (fcfg_rc == -1) {
    print_eeprom_line_parse_error(fcfg->logger, fcfg_line, fcfg_line_len);
  }
  if (fcfg_rc == 0) {
    print_save_fcfg_to_eeprom(fcfg->logger);
    int set_rc = eeprom_config_set(ecfg, line_copy, sizeof(line_copy));
    if (set_rc < 0) {
      if (set_rc == -1) {
        print_failed_to_save_fcfg_to_eeprom_input_too_long(fcfg->logger);
      } else if (set_rc == -2) {
        print_failed_to_save_fcfg_to_eeprom_locked(fcfg->logger);
      }
      return set_rc;
    }
  }
  if (fcfg_rc == FORWARDER_CONFIG_HELP) {
    print_usage(fcfg->logger);
  }
  if (fcfg_rc != 0 && fcfg_rc != FORWARDER_CONFIG_HELP) {
    log_str(fcfg->logger, PSTR("Use 'cmqttsnforwarder --help' to see usage."));
    log_flush(fcfg->logger);
  }
  return fcfg_rc;
}

int print_eeprom_config_usage(const MqttSnLogger *logger) {
  log_str(logger, PSTR("EEPROM Usage:     [-es eeprom_show]\n"));
  log_str(logger, PSTR("                  [-ec eeprom_clear]\n"));
  log_str(logger, PSTR("                  [-el eeprom_lock]\n"));
  log_str(logger, PSTR("                  [-eu eeprom_unlock]\n"));
  log_str(logger, PSTR("                  [-efl eeprom_force_line]\n"));
#ifdef WITH_ARDUINO_WIFI
  log_str(logger, PSTR("                  [-ews eeprom_wifi_show]\n"));
  log_str(logger, PSTR("                  [-ewc eeprom_wifi_clean]\n"));
  log_str(logger, PSTR("                  [-ewn eeprom_wifi_name]\n"));
  log_str(logger, PSTR("                  [-ewp eeprom_wifi_password]\n"));
#endif
  log_str(logger, PSTR("                  --eeprom_help\n"));
  log_flush(logger);
  log_str(logger, PSTR(" -es : show the current EEPROM configuration.\n"));
  log_str(logger, PSTR(" -ec : clear the EEPROM configuration.\n"));
  log_str(logger, PSTR(" -el : locks the EEPROM configuration to prevent clearing the EEPROM configuration.\n"));
  log_str(logger, PSTR("       Can be used to define a config-file which is used after flashing the firmware.\n"));
  log_str(logger, PSTR(" -eu : unlocks the EEPROM configuration.\n"));
  log_str(logger, PSTR(" -efl : write the input directly into the EEPROM configuration.\n"));
  log_str(logger, PSTR("        Can be used to circumvent the MQTT-SN Forwarder command line validation.\n"));
#ifdef WITH_ARDUINO_WIFI
  log_str(logger, PSTR(" -ews : show the current WiFi configuration.\n"));
  log_str(logger, PSTR(" -ewc : clear the WiFi configuration.\n"));
  log_str(logger, PSTR(" -ewn : set the WiFi ssid aka name.\n"));
  log_str(logger, PSTR(" -ewp : set the WiFi password.\n"));
#endif
  log_str(logger, PSTR(" --eeprom_help : display this message.\n"));
  log_flush(logger);
  return log_status(logger);
}

int process_eeprom_force_set_str(EEPROM_cfg *ecfg, const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len) {
  char *efl_short = "-efl";
  char *efl_long = "--eeprom_force_line ";
  char *efl = NULL;
  if (ecfg_line_len > strlen(efl_short) && strlen(ecfg_line) > strlen(efl_short)
      && StartsWith(efl_short, ecfg_line)) {
    efl = efl_short;
  } else if (ecfg_line_len > strlen(efl_long) && strlen(ecfg_line) > strlen(efl_long)
      && StartsWith(efl_long, ecfg_line)) {
    efl = efl_long;
  } else {
    return -1;
  }
  if (ecfg_line_len - strlen(efl) <= 1) {
    log_argument_value_not_specified(logger, efl, PSTR("input"));
    return -2;
  }
  print_save_fcfg_to_eeprom(logger);
  int set_rc = eeprom_config_set(ecfg, &ecfg_line[strlen(efl) + 1], ecfg_line_len - (strlen(efl)));
  if (set_rc < 0) {
    if (set_rc == -1) {
      print_failed_to_save_fcfg_to_eeprom_input_too_long(logger);
    } else if (set_rc == -2) {
      print_failed_to_save_fcfg_to_eeprom_locked(logger);
    }
    return set_rc;
  }
  return 0;
}

int process_eeprom_config_str(EEPROM_cfg *ecfg, const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len) {
  assert(ecfg != NULL);
  assert(logger != NULL);

  char *argv_0 = "ecfg_line";

  if (ecfg_line == NULL) {
    return -1;
  }
  if (ecfg_line_len == 0) {
    return -1;
  }

  if (strlen(ecfg_line) + 1 != ecfg_line_len) {
    return -1;
  }
  if (strlen(ecfg_line) == 0) {
    return -1;
  }

  int force_rc = process_eeprom_force_set_str(ecfg, logger, ecfg_line, ecfg_line_len);
  if (force_rc == -2) {
    return -1;
  } else if (force_rc == 0) {
    return 0;
  }

  size_t len = ecfg_line_len;
  int tk_count = 0;
  {
    char line_copy[len];
    memcpy(line_copy, ecfg_line, len);
    // estimates argc
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      tk_count++;
    }
  }

  char *argv[tk_count];
  int argc = 0;
  char line_copy[len];
  {
    memcpy(line_copy, ecfg_line, len);
    argv[argc++] = argv_0;
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

  // process
  int rc = process_eeprom_config_token(ecfg, logger, argc, argv);
  return rc;
}
int validate_eeprom_config_token(const MqttSnLogger *logger, int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-el") || !strcmp(argv[i], "--eeprom_lock")) {
      i++;
    } else if (!strcmp(argv[i], "-eu") || !strcmp(argv[i], "--eeprom_unlock")) {
      i++;
    } else if (!strcmp(argv[i], "-ec") || !strcmp(argv[i], "--eeprom_clear")) {
      i++;
    } else if (!strcmp(argv[i], "-es") || !strcmp(argv[i], "--eeprom_show")) {
      i++;
    }
#ifdef WITH_ARDUINO_WIFI
    else if (!strcmp(argv[i], "-ewc") || !strcmp(argv[i], "--eeprom_wifi_clean")) {
      i++;
    } else if (!strcmp(argv[i], "-ewn") || !strcmp(argv[i], "--eeprom_wifi_name")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_name"));
        return 1;
      } else {
        // validation only
      }
      i++;
    } else if (!strcmp(argv[i], "-ewp") || !strcmp(argv[i], "--eeprom_wifi_password")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_password"));
        return 1;
      } else {
        // validation only
      }
      i++;
    } else if (!strcmp(argv[i], "-ews") || !strcmp(argv[i], "--eeprom_wifi_show")) {
      i++;
    }
#endif
    else if (!strcmp(argv[i], "-eh") || !strcmp(argv[i], "--eeprom_help")) {
      return 2;
    } else {
#ifdef WITH_DEBUG_LOGGING
      print_eeprom_unknown_option(logger, argv[i]);
#endif
      return 1;
    }
  }
  return 0;
}
int process_eeprom_config_token(EEPROM_cfg *ecfg, const MqttSnLogger *logger, int argc, char *argv[]) {
  int validation_rc = validate_eeprom_config_token(logger, argc, argv);
  if (validation_rc == 2) {
    print_eeprom_config_usage(logger);
    return 2;
  }
  if (validation_rc != 0) {
    return validation_rc;
  }

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-el") || !strcmp(argv[i], "--eeprom_lock")) {
      eeprom_lock(ecfg);
      print_eeprom_lock_status(logger, ecfg);
    } else if (!strcmp(argv[i], "-eu") || !strcmp(argv[i], "--eeprom_unlock")) {
      eeprom_unlock(ecfg);
      print_eeprom_lock_status(logger, ecfg);
    } else if (!strcmp(argv[i], "-ec") || !strcmp(argv[i], "--eeprom_clear")) {
      if (eeprom_config_clear(ecfg) == -2) {
        print_failed_to_clear_eeprom_locked(logger);
      } else {
        print_eeprom_cleared(logger);
      }
    } else if (!strcmp(argv[i], "-es") || !strcmp(argv[i], "--eeprom_show")) {
      print_eeprom_ecfg(logger, ecfg);
    }
#ifdef WITH_ARDUINO_WIFI
    else if (!strcmp(argv[i], "-ewc") || !strcmp(argv[i], "--eeprom_wifi_clean")) {
      memset(ecfg->WiFi_cfg.ssid, 0, sizeof(ecfg->WiFi_cfg.ssid));
      memset(ecfg->WiFi_cfg.password, 0, sizeof(ecfg->WiFi_cfg.password));
      print_eeprom_wifi_cleared(logger);
    } else if (!strcmp(argv[i], "-ewn") || !strcmp(argv[i], "--eeprom_wifi_name")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_name"));
        return -1;
      } else {
        if (strlen(argv[i + 1]) + 1 > sizeof(ecfg->WiFi_cfg.ssid)) {
          print_eeprom_invalid_too_long_wifi_name(logger, argv[i + 1]);
        }
        memset(ecfg->WiFi_cfg.ssid, 0, sizeof(ecfg->WiFi_cfg.ssid));
        strcpy(ecfg->WiFi_cfg.ssid, argv[i + 1]);
        print_eeprom_wifi_set_ssid(logger, ecfg->WiFi_cfg.ssid);
      }
      i++;
    } else if (!strcmp(argv[i], "-ewp") || !strcmp(argv[i], "--eeprom_wifi_password")) {
      if (i == argc - 1) {
        log_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_password"));
        return -1;
      } else {
        if (strlen(argv[i + 1]) + 1 > sizeof(ecfg->WiFi_cfg.password)) {
          log_eeprom_invalid_too_long_wifi_password(logger);
        }
        memset(ecfg->WiFi_cfg.password, 0, sizeof(ecfg->WiFi_cfg.password));
        strcpy(ecfg->WiFi_cfg.password, argv[i + 1]);
#ifdef WITH_DEBUG_LOGGING
        print_eeprom_wifi_set_password(logger, ecfg->WiFi_cfg.password);
#else
        log_eeprom_wifi_set_password(logger, NULL);
#endif
      }
      i++;
    } else if (!strcmp(argv[i], "-ews") || !strcmp(argv[i], "--eeprom_wifi_show")) {
      print_eeprom_wifi(logger, ecfg);
      i++;
    }
#endif
  }
  return 0;
}

#ifdef WITH_LOGGING
int print_loading_fcfg_from_eeprom(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Loading MQTT-SN Forwarder Configuration from EEPROM."));
  log_flush(logger);
  return log_status(logger);
}
int print_invalid_fcfg_clearing_eeprom(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Invalid MQTT-SN Forwarder Configuration in EEPROM - clearing EEPROM Configuration."));
  log_flush(logger);
  return log_status(logger);
}
int print_save_fcfg_to_eeprom(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Saving MQTT-SN Forwarder Configuration to EEPROM."));
  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_line_parse_error(const MqttSnLogger *logger, const char *line, size_t line_len) {
  log_str(logger, PSTR("Cannot tokenize: len: "));
  log_uint64(logger, line_len);
  log_str(logger, PSTR(", line: "));
  log_str(logger, line);
  log_flush(logger);
  return log_status(logger);
}
int print_failed_to_save_fcfg_to_eeprom_input_too_long(const MqttSnLogger *logger) {
  return print_failed_to_save_fcfg_to_eeprom(logger, PSTR("input is too long"));
}
int print_failed_to_save_fcfg_to_eeprom_locked(const MqttSnLogger *logger) {
  return print_failed_to_save_fcfg_to_eeprom(logger, PSTR("EEPROM is locked"));
}
int print_failed_to_save_fcfg_to_eeprom(const MqttSnLogger *logger, const char *reason) {
  log_str(logger, PSTR("Failed to save MQTT-SN Forwarder Configuration to EEPROM - "));
  log_str(logger, reason);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int print_failed_to_clear_eeprom_locked(const MqttSnLogger *logger) {
  print_failed_to_clear_eeprom(logger, PSTR("EEPROM locked."));
}
int print_failed_to_clear_eeprom(const MqttSnLogger *logger, const char *reason) {
  log_str(logger, PSTR("Failed to clear EEPROM - "));
  log_str(logger, reason);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_ecfg(const MqttSnLogger *logger, const EEPROM_cfg *ecfg) {
  log_str(logger, ecfg->fcfg_name);
  log_str(logger, PSTR(" EEPROM Configuration: "));

  log_str(logger, PSTR("locked: "));
  if (ecfg->locked) {
    log_str(logger, PSTR("true"));
  } else {
    log_str(logger, PSTR("false"));
  }

  log_str(logger, PSTR(", line: "));
  log_str(logger, ecfg->fcfg_line);

  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_unknown_option(const MqttSnLogger *logger, const char *unknown_option) {
  log_str(logger, PSTR("Error: Unknown EEPROM option "));
  log_str(logger, unknown_option);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_cleared(const MqttSnLogger *logger) {
  log_str(logger, PSTR("EPPROM configuration cleared."));
  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_lock_status(const MqttSnLogger *logger, const EEPROM_cfg *ecfg) {
  log_str(logger, PSTR("EPPROM configuration locked: "));
  if (ecfg->locked) {
    log_str(logger, PSTR("true"));
  } else {
    log_str(logger, PSTR("false"));
  }
  log_flush(logger);
  return log_status(logger);
}

#ifdef WITH_ARDUINO_WIFI
int print_eeprom_wifi(const MqttSnLogger *logger, const EEPROM_cfg *ecfg) {
  log_str(logger, ecfg->fcfg_name);
  log_str(logger, PSTR(" in EEPROM WiFi Configuration: "));

  log_str(logger, PSTR("ssid: "));
  log_str(logger, ecfg->WiFi_cfg.ssid);
  log_str(logger, PSTR(", password: "));
#ifdef WITH_DEBUG_LOGGING
  log_str(logger, ecfg->WiFi_cfg.password);
#else
  log_str(logger, PSTR("****"));
#endif

  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_invalid_too_long_wifi_name(const MqttSnLogger *logger, const char *wifi_name) {
  log_str(logger, PSTR("Error: WiFi SSID "));
  log_str(logger, wifi_name);
  log_str(logger, PSTR(" is too long - maximum is 32 characters."));
  log_flush(logger);
  return log_status(logger);
}
int log_eeprom_invalid_too_long_wifi_password(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Error: WiFi password is too long - maximum is 63 characters."));
  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_wifi_cleared(const MqttSnLogger *logger) {
  log_str(logger, PSTR("WiFi configuration cleared."));
  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_wifi_set_ssid(const MqttSnLogger *logger, const char *ssid) {
  log_str(logger, PSTR("WiFi SSID set to: "));
  log_str(logger, ssid);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int print_eeprom_wifi_set_password(const MqttSnLogger *logger, const char *password) {
  log_str(logger, PSTR("WiFi SSID set to: "));
#ifdef WITH_DEBUG_LOGGING
  log_str(logger, password);
#else
  log_str(logger, PSTR("****"));
#endif
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
#endif
#endif
