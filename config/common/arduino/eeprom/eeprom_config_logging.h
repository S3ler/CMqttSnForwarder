//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_LOGGING_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_LOGGING_H_

#include <logging/MqttSnLogging.h>
#include "eeprom_config.h"

#ifdef __cplusplus
extern "C" {
#endif

int print_eeprom_config_usage(const MqttSnLogger *logger);

int print_save_ecfg_line_to_eeprom(const MqttSnLogger *logger);
int print_failed_to_save_fcfg_to_eeprom_input_too_long(const MqttSnLogger *logger);
int print_failed_to_save_fcfg_to_eeprom_locked(const MqttSnLogger *logger);
int print_failed_to_save_fcfg_to_eeprom(const MqttSnLogger *logger, const char *reason);
int print_failed_to_clear_eeprom_locked(const MqttSnLogger *logger);
int print_failed_to_clear_eeprom(const MqttSnLogger *logger, const char *reason);
int print_eeprom_ecfg(const MqttSnLogger *logger, const EEPROM_cfg *ecfg);
int print_eeprom_unknown_option(const MqttSnLogger *logger, const char *unknown_option);
int print_eeprom_line_parse_error(const MqttSnLogger *logger, const char *line, size_t line_len);
int print_eeprom_lock_status(const MqttSnLogger *logger, const EEPROM_cfg *ecfg);
int print_eeprom_cleared(const MqttSnLogger *logger);

#ifdef WITH_ARDUINO_WIFI
int print_eeprom_invalid_too_long_wifi_name(const MqttSnLogger *logger, const char *wifi_name);
int log_eeprom_invalid_too_long_wifi_password(const MqttSnLogger *logger);
int print_eeprom_wifi(const MqttSnLogger *logger, const EEPROM_cfg *ecfg);
int print_eeprom_wifi_cleared(const MqttSnLogger *logger);
int print_eeprom_wifi_set_ssid(const MqttSnLogger *logger, const char *ssid);
int print_eeprom_wifi_set_password(const MqttSnLogger *logger, const char *password);
#endif

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_LOGGING_H_
