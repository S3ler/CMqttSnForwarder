//
// Created by SomeDude on 01.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_H_

#include <EEPROM.h>
#include <stdint.h>
#include <stddef.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO WITH_ARDUINO_WIFI
#ifndef WITH_ARDUINO_WIFI
#define WITH_ARDUINO_WIFI
#endif

#ifndef EEPROM_FCFG_NAME
#define EEPROM_FCFG_NAME "N/D"
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

int process_eeprom_force_set_str(EEPROM_cfg *ecfg, const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len);
int process_eeprom_config_str(EEPROM_cfg *ecfg, const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len);
int validate_eeprom_config_token(const MqttSnLogger *logger, int argc, char *argv[]);
int process_eeprom_config_token(EEPROM_cfg *ecfg, const MqttSnLogger *logger, int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_EEPROM_CONFIG_H_
