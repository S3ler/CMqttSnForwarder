//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_FORWARDER_EEPROM_LOADER_LOGGING_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_FORWARDER_EEPROM_LOADER_LOGGING_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <logging/MqttSnLoggingInterface.h>

int print_loading_fcfg_from_eeprom(const MqttSnLogger *logger);
int print_invalid_fcfg_clearing_eeprom(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_EEPROM_FORWARDER_EEPROM_LOADER_LOGGING_H_
