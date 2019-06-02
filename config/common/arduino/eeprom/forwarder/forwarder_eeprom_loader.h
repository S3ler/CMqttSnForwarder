//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_FORWARDER_EEPROM_LOADER_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_FORWARDER_EEPROM_LOADER_H_

#include <forwarder/config/forwarder_config.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "config/common/arduino/eeprom/eeprom_config.h"

/**
 * Initializes ecfg via eeprom_config_init. Parses fcfg_line via process_forwarder_config_str.
 * On parse error from process_forwarder_config_str clears the eeprom.
 * On --help returns FORWARDER_CONFIG_HELP and does not clear the eeprom.
 * Note: do not call eeprom_config_cleanup as long as you use fcfg.
 * @param ecfg must point to NULL
 * @param fcfg must not be NULL and must fcfg.logger must not be NULL. If the fcfg.logger is not set, the bahaviour is undefined.
 * @return the return value from process_forwarder_config_str.
 */
int eeprom_load_forwarder_config(EEPROM_cfg *ecfg, forwarder_config *fcfg, const MqttSnLogger *logger);

/*
 * parses the config to fcfg.
 * On fcfg parse-error eeprom is not changed - return fcfg-parse return value
 * on success fcfg is changed to the data from the given line and the line is saved in the eeprom
 * normally/sucess return 0; (like fcfg parase)
 */
/**
 *
 * @param fcfg must not be NULL and must fcfg.logger must not be NULL. If the fcfg.logger is not set, the bahaviour is undefined.
 * @param fcfg_line
 * @param fcfg_line_len
 * @return the return value from process_forwarder_config_str if saved. Else the return value of eeprom_config_set.
 */
int eeprom_save_forwarder_config_line(EEPROM_cfg *ecfg,
                                      forwarder_config *fcfg,
                                      const MqttSnLogger *logger,
                                      char *fcfg_line,
                                      size_t fcfg_line_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_FORWARDER_EEPROM_LOADER_H_
