//
// Created by SomeDude on 01.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// UNDEF
#ifndef WITH_ARDUINO_WIFI
#define WITH_ARDUINO_WIFI
#endif

#include <EEPROM.h>
#include <stddef.h>
#include <main/forwarder_config.h>

#ifndef EEPROM_FCFG_NAME
#define EEPROM_FCFG_NAME "CMqttSnForwarderArduino"
#endif

#ifndef EEPROM_DATA_SIZE
#define EEPROM_DATA_SIZE 1024
#endif

#ifdef WITH_ARDUINO_WIFI
#define EEPROM_LINE_SIZE (EEPROM_DATA_SIZE  - sizeof(uint8_t) - sizeof(EEPROM_FCFG_NAME) - sizeof(EEPROM_WiFi_cfg))
#else
#define EEPROM_LINE_SIZE (EEPROM_DATA_SIZE  - sizeof(uint8_t) - sizeof(EEPROM_FCFG_NAME))
#endif
#ifdef WITH_ARDUINO_WIFI

#pragma pack(push)
#pragma pack(1)
typedef struct EEPROM_WiFi_cfg_ {
  char ssid[32 + 1];
  char password[63 + 1];
} EEPROM_WiFi_cfg;
#pragma pack(pop)

void clear_wifi_eeprom();
void set_wifi_eeprom_ssid_name();
void set_wifi_eeprom_password();
#endif

#pragma pack(push)
#pragma pack(1)
typedef struct EEPROM_cfg_ {
  uint8_t locked;
  char fcfg_name[sizeof(EEPROM_FCFG_NAME)];
  char fcfg_line[EEPROM_LINE_SIZE];
#ifdef WITH_ARDUINO_WIFI
  EEPROM_WiFi_cfg WiFi_cfg;
#endif
} EEPROM_cfg;
#pragma pack(pop)

/**
 * Begins the EEPROM with EEPROM_DATA_SIZE and sets the ecfg to the EEPROM DataPtr
 * @param ecfg must point to NULL
 */
void eeprom_config_init(EEPROM_cfg **ecfg);

/**
 * Ends the EEPROM and sets ecfg to NULL
 * This des not save the data to the EEPROM
 * @param ecfg must not be NULL
 */
void eeprom_config_cleanup(EEPROM_cfg **ecfg);

/**
 * clears the fcfg_line and commits it to the EEPROM
 * ignores the eeprom lock
 * @param ecfg must not be NULL
 */
int eeprom_config_clear(EEPROM_cfg *ecfg);

/**
 * Locks the ecfg.
 * Note: This feature can be used to use configuration files from an SD card which is not attached when uploading the firmware.
 * Most likely results in a endless reset/restart loop.
 * @param ecfg to lock
 */
void eeprom_lock(EEPROM_cfg *ecfg);

/**
 * Unlocks the ecfg
 * @param ecfg to unlock
 */
void eeprom_unlock(EEPROM_cfg *ecfg);

/**
 * Saves the fcfg_line persistently into the EEPROM
 * @param ecfg must not be NULL
 * @param fcfg_line must not be NULL
 * @param fcfg_line_len must be smaller than EEPROM_LINE_SIZE
 * @return -2 if the EEPROM is locked, -1 if fcfg_line_len is greater than EEPROM_LINE_SIZE, else 0
 */
int eeprom_config_set(EEPROM_cfg *ecfg, const char *fcfg_line, size_t fcfg_line_len);

/**
 * Initializes ecfg via eeprom_config_init. Parses fcfg_line via process_forwarder_config_str.
 * On parse error from process_forwarder_config_str clears the eeprom.
 * On --help returns FORWARDER_CONFIG_HELP and does not clear the eeprom.
 * Note: do not call eeprom_config_cleanup as long as you use fcfg.
 * @param ecfg must point to NULL
 * @param fcfg must not be NULL and must fcfg.logger must not be NULL. If the fcfg.logger is not set, the bahaviour is undefined.
 * @return the return value from process_forwarder_config_str.
 */
int eeprom_load_forwarder_config(EEPROM_cfg *ecfg, forwarder_config *fcfg);

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
int eeprom_save_forwarder_config_line(EEPROM_cfg *ecfg, forwarder_config *fcfg, char *fcfg_line, size_t fcfg_line_len);

int print_eeprom_config_usage(const MqttSnLogger *logger);
int process_eeprom_force_set_str(EEPROM_cfg *ecfg, const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len);
int process_eeprom_config_str(EEPROM_cfg *ecfg, const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len);
int validate_eeprom_config_token(const MqttSnLogger *logger, int argc, char *argv[]);
int process_eeprom_config_token(EEPROM_cfg *ecfg, const MqttSnLogger *logger, int argc, char *argv[]);

#ifdef WITH_LOGGING
int print_loading_fcfg_from_eeprom(const MqttSnLogger *logger);
int print_invalid_fcfg_clearing_eeprom(const MqttSnLogger *logger);
int print_save_fcfg_to_eeprom(const MqttSnLogger *logger);
int print_failed_to_save_fcfg_to_eeprom_input_too_long(const MqttSnLogger *logger);
int print_failed_to_save_fcfg_to_eeprom_locked(const MqttSnLogger *logger);
int print_failed_to_save_fcfg_to_eeprom(const MqttSnLogger *logger, const char *reason);
int print_failed_to_clear_eeprom_locked(const MqttSnLogger *logger);
int print_failed_to_clear_eeprom(const MqttSnLogger *logger, const char *reason);
int print_eeprom_ecfg(const MqttSnLogger *logger, const EEPROM_cfg *ecfg);
int print_eeprom_wifi(const MqttSnLogger *logger, const EEPROM_cfg *ecfg);
int print_eeprom_unknown_option(const MqttSnLogger *logger, const char *unknown_option);
int print_eeprom_invalid_too_long_wifi_name(const MqttSnLogger *logger, const char *wifi_name);
int log_eeprom_invalid_too_long_wifi_password(const MqttSnLogger *logger);
int print_eeprom_line_parse_error(const MqttSnLogger *logger, const char *line, size_t line_len);

int print_eeprom_lock_status(const MqttSnLogger *logger, const EEPROM_cfg *ecfg);
int print_eeprom_cleared(const MqttSnLogger *logger);

int print_eeprom_wifi_cleared(const MqttSnLogger *logger);
int print_eeprom_wifi_set_ssid(const MqttSnLogger *logger, const char *ssid);
int print_eeprom_wifi_set_password(const MqttSnLogger *logger, const char *password);
#endif

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_H_
