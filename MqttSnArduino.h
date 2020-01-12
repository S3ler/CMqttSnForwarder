//
// Created by SomeDude on 20.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNARDUINO_H
#define CMQTTSNFORWARDER_MQTTSNARDUINO_H

#define MQTT_SN_ARDUINO

#define WITH_RESTART

#define WITH_EEPROM
#define WITH_LOGGING
#define WITH_DEBUG_LOGGING

#include <logging/MqttSnLogging.c>
#include <logging/MqttSnLogging.h>

#include <logging/arduino/serial/ArduinoSerial.cpp>
#include <logging/arduino/serial/ArduinoSerial.hpp>
#include <logging/arduino/SD/ArduinoSD.cpp>
#include <logging/arduino/SD/ArduinoSD.hpp>
#include <logging/arduino/SSD1306/ArduinoSSD1306.cpp>
#include <logging/arduino/SSD1306/ArduinoSSD1306.hpp>

#include <config/common/mqtt_sn_logger_config.c>
#include <config/common/mqtt_sn_logger_config.h>

// eeprom
#include <config/common/arduino/eeprom/eeprom_config.c>
#include <config/common/arduino/eeprom/eeprom_config.h>
#include <config/common/config_command_helper.c>
#include <config/common/config_command_helper.h>
#include <config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader.c>
#include <config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader.h>
#include <config/common/arduino/eeprom/eeprom_config_logging.c>
#include <config/common/arduino/eeprom/eeprom_config_logging.h>
#include <config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.c>
#include <config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.h>

#include <config/common/arduino/system/system.c>
#include <config/common/arduino/system/system.h>
#ifdef WITH_LOGGING
#include <config/common/arduino/system/system_logging.c>
#include <config/common/arduino/system/system_logging.h>
#endif

#endif  //CMQTTSNFORWARDER_MQTTSNARDUINO_H
