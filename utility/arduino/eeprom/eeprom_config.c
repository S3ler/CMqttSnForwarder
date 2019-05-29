//
// Created by SomeDude on 01.05.2019.
//

#include "eeprom_config.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <platform/platform_compatibility.h>
#include <forwarder/config/forwarder_config.h>
#include <forwarder/config/forwarder_config_logger.h>
#include <utility/arduino/system/system.h>
#include "eeprom_config_logging.h"

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

int process_eeprom_force_set_str(EEPROM_cfg *ecfg, const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len) {
  const char *efl_short = "-efl";
  const char *efl_long = "--eeprom_force_line ";
  const char *efl = NULL;
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
    print_argument_value_not_specified(logger, efl, PSTR("input"));
    return -2;
  }
  print_save_ecfg_line_to_eeprom(logger);
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

  const char *argv_0 = "ecfg_line";

  if (ecfg_line == NULL) {
    return -1;
  }
  if (ecfg_line_len == 0) {
    return -1;
  }
  if (ecfg_line_len > EEPROM_LINE_SIZE) {
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

  int tk_count = 0;
  {
    char line_copy[EEPROM_LINE_SIZE];
    memcpy(line_copy, ecfg_line, ecfg_line_len);
    // estimates argc
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      tk_count++;
    }
  }

  char *argv[EEPROM_LINE_SIZE];
  int argc = 0;
  char line_copy[EEPROM_LINE_SIZE];
  {
    memcpy(line_copy, ecfg_line, ecfg_line_len);
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

  // process
  int rc = process_eeprom_config_token(ecfg, logger, argc, argv);
  return rc;
}
int validate_eeprom_config_token(const MqttSnLogger *logger, int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (!strcmp_P(argv[i], "-el") || !strcmp_P(argv[i], "--eeprom_lock")) {
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
        print_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_name"));
        return 1;
      } else {
        // validation only
      }
      i++;
    } else if (!strcmp(argv[i], "-ewp") || !strcmp(argv[i], "--eeprom_wifi_password")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_password"));
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
        print_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_name"));
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
        print_argument_value_not_specified(logger, argv[i], PSTR("eeprom_wifi_password"));
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
