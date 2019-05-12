//
// Created by SomeDude on 12.05.2019.
//

#include <assert.h>
#include <forwarder/config/forwarder_config_logger.h>
#include <platform/platform_compatibility.h>
#include "forwarder_eeprom_loader.h"
#include "forwarder_eeprom_loader_logging.h"
#include "utility/arduino/eeprom/eeprom_config_logging.h"

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
  if (fcfg_line_len > EEPROM_LINE_SIZE) {
    return -1;
  }

  const char *argv_0 = "fcfg_line";
  char line_copy[EEPROM_LINE_SIZE];
  memcpy(line_copy, fcfg_line, fcfg_line_len);

  int fcfg_rc = process_forwarder_config_str(fcfg, fcfg_line, fcfg_line_len, argv_0);
  if (fcfg_rc == -1) {
    print_eeprom_line_parse_error(fcfg->logger, fcfg_line, fcfg_line_len);
  }
  if (fcfg_rc == 0) {
    print_save_ecfg_line_to_eeprom(fcfg->logger);
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
