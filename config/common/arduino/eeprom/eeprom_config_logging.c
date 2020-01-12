//
// Created by SomeDude on 12.05.2019.
//

#include <platform/platform_compatibility.h>
#include "eeprom_config_logging.h"

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
    log_str(logger, PSTR("        Can be used to circumvent command line validation.\n"));
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

int print_save_ecfg_line_to_eeprom(const MqttSnLogger *logger) {
    log_str(logger, PSTR("Saving Configuration to EEPROM."));
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
    log_str(logger, PSTR("Failed to save Configuration to EEPROM - "));
    log_str(logger, reason);
    log_str(logger, PSTR("."));
    log_flush(logger);
    return log_status(logger);
}
int print_failed_to_clear_eeprom_locked(const MqttSnLogger *logger) {
    return print_failed_to_clear_eeprom(logger, PSTR("EEPROM locked."));
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
    if (shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        log_str(logger, PSTR("****"));
    } else {
        log_str(logger, ecfg->WiFi_cfg.password);
    }
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
    if (shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        log_str(logger, PSTR("****"));
    }else{
        log_str(logger, password);
    }
#else
    log_str(logger, PSTR("****"));
#endif
    log_str(logger, PSTR("."));
    log_flush(logger);
    return log_status(logger);
}
#endif
