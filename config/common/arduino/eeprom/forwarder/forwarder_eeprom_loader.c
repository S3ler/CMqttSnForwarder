//
// Created by SomeDude on 12.05.2019.
//

#include "forwarder_eeprom_loader.h"
#include "forwarder_eeprom_loader_logging.h"
#include <platform/platform_compatibility.h>
#include <config/common/arduino/eeprom/eeprom_config_logging.h>
#include <assert.h>
#include <config/common/config_command_helper.h>

int eeprom_load_forwarder_config(EEPROM_cfg *ecfg, forwarder_config *fcfg, const MqttSnLogger *logger) {
  assert(ecfg != NULL);
  assert(ecfg == (EEPROM_cfg *) EEPROM.getDataPtr());
  assert(fcfg != NULL);
  assert(logger != NULL);
#ifdef WITH_LOGGING
  print_loading_fcfg_from_eeprom(logger);
#endif

  if (strlen(ecfg->fcfg_line) == 0) {
    return 0;
  }

  int32_t fcfg_rc = process_config_file_line(logger,
                                             ecfg->fcfg_line,
                                             strlen(ecfg->fcfg_line) + 1,
                                             fcfg->executable_name,
                                             fcfg,
                                             forwarder_config_file_process_command_callback);

  if (fcfg_rc == MQTT_SN_PARSE_CONFIG_HELP) {
    forwarder_config_print_usage(logger);
  } else if (fcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
#ifdef WITH_LOGGING
    print_invalid_fcfg_clearing_eeprom(logger);
#endif
    if (eeprom_config_clear(ecfg) == -2) {
      print_failed_to_clear_eeprom_locked(logger);
      return -2;
    }
  }

  return fcfg_rc;
}

int eeprom_save_forwarder_config_line(EEPROM_cfg *ecfg,
                                      forwarder_config *fcfg,
                                      const MqttSnLogger *logger,
                                      char *fcfg_line,
                                      size_t fcfg_line_len) {
  assert(fcfg != NULL);
  assert(logger != NULL);
  if (fcfg_line_len > EEPROM_LINE_SIZE) {
    return -1;
  }

  char line_copy[EEPROM_LINE_SIZE];
  memcpy(line_copy, fcfg_line, fcfg_line_len);

  int32_t fcfg_rc = process_config_file_line(logger,
                                             line_copy,
                                             fcfg_line_len,
                                             fcfg->executable_name,
                                             fcfg,
                                             forwarder_config_file_process_command_callback);
  if (fcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
    print_eeprom_line_parse_error(logger, fcfg_line, fcfg_line_len);
  }
  if (fcfg_rc == 0) {
    print_save_ecfg_line_to_eeprom(logger);
    int set_rc = eeprom_config_set(ecfg, line_copy, sizeof(line_copy));
    if (set_rc < 0) {
      if (set_rc == -1) {
        print_failed_to_save_fcfg_to_eeprom_input_too_long(logger);
      } else if (set_rc == -2) {
        print_failed_to_save_fcfg_to_eeprom_locked(logger);
      }
      return set_rc;
    }
  }
  if (fcfg_rc == MQTT_SN_PARSE_CONFIG_HELP) {
    forwarder_config_print_usage(logger);
  }
  if (fcfg_rc != MQTT_SN_PARSE_CONFIG_SUCCESS && fcfg_rc != MQTT_SN_PARSE_CONFIG_HELP) {
    log_str(logger, PSTR("Use 'cmqttsnforwarder --help' to see usage."));
    log_flush(logger);
  }
  return fcfg_rc;
}
