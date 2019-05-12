//
// Created by SomeDude on 12.05.2019.
//

#include <platform/platform_compatibility.h>
#include "forwarder_eeprom_loader_logging.h"

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
