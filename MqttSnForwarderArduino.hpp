//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERARDUINO_HPP
#define CMQTTSNFORWARDER_MQTTSNFORWARDERARDUINO_HPP

#define MQTT_SN_ARDUINO

#define VERSION "a2"
#define MAJOR 1
#define MINOR 1
#define TWEAK 0
#define CMAKE_BUILD_TIMESTAMP "build date 25-09-2019 15:00:53"

#define EEPROM_FCFG_NAME "EEPROM"

#define WITH_ARDUINO_WIFI
#define WITH_UDP_BROADCAST
#define WITH_UDP_BROADCAST_CLIENT
#define WITH_UDP_BROADCAST_GATEWAY

#define WITH_GATEWAY_NETWORK_BROADCAST
#define WITH_ARDUINO_GATEWAY_NETWORK_UDP

#define WITH_CLIENT_NETWORK_BROADCAST
#define WITH_ARDUINO_CLIENT_NETWORK_UDP

#define WITH_RESTART
#define WITH_EEPROM
#define WITH_LOGGING
#define WITH_DEBUG_LOGGING

#define MQTT_SN_FORWARDER_PIN_CONNECTION_DEFAULT_VALUE 1
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN 2
#define MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH 1024

#define DEFAULT_MQTT_SN_GATEWAY_HOST "arsmb.de"

#define DEFAULT_MQTT_SN_CLIENT_BIND_PORT 10002
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS "225.1.1.2"
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT 1883

#define DEFAULT_MQTT_SN_GATEWAY_BIND_PORT 10001
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS "225.1.1.1"
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT 1883

#define WITH_MQTT_SN_FORWARDER_CONFIG_CONTAINER

#include <platform/platform_compatibility.c>
#include <platform/platform_compatibility.h>

#include <platform/device_address.c>
#include <platform/device_address.h>
#include <platform/MqttSnMessageData.c>
#include <platform/MqttSnMessageData.h>
// buffer
#include <ringbuffer/MqttSnFixedSizeRingBuffer.c>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
// parser
#include <MqttSnParserArduino.hpp>
// networks
#include <MqttSnGatewayNetworkArduino.hpp>
#include "MqttSnClientNetworkArduino.hpp"
// discovered gateway list
#include <MqttSnDiscoveredGatewayList.hpp>

// logger
#include <logging/MqttSnLogging.c>
#include <logging/MqttSnLogging.h>

#include <logging/arduino/serial/ArduinoSerial.cpp>
#include <logging/arduino/serial/ArduinoSerial.hpp>
#include <logging/arduino/SD/ArduinoSD.cpp>
#include <logging/arduino/SD/ArduinoSD.hpp>
#include <logging/arduino/SSD1306/ArduinoSSD1306.cpp>
#include <logging/arduino/SSD1306/ArduinoSSD1306.hpp>

// common
#include <config/common/arduino/system/system.c>
#include <config/common/arduino/system/system.h>

// config
#include <config/common/mqtt_sn_logger_config.c>
#include <config/common/mqtt_sn_logger_config.h>
#include <config/common/mqtt_sn_version_config.c>
#include <config/common/mqtt_sn_version_config.h>
#include <config/client/client_mqtt_sn_gateway_config.c>
#include <config/client/client_mqtt_sn_gateway_config.h>
#include <config/network/client/client_network_config.c>
#include <config/network/client/client_network_config.h>
#include <config/network/gateway/gateway_network_config.c>
#include <config/network/gateway/gateway_network_config.h>

#include <forwarder/config/forwarder_config.c>
#include <forwarder/config/forwarder_config.h>

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

#include <config/common/arduino/system/system_logging.c>
#include <config/common/arduino/system/system_logging.h>


// client network
#ifdef WITH_ARDUINO_CLIENT_NETWORK_UDP

#endif

// forwarder
#include <forwarder/MqttSnForwarder.c>
#include <forwarder/MqttSnForwarder.h>
#include <forwarder/MqttSnForwarderLogging.c>
#include <forwarder/MqttSnForwarderLogging.h>

#endif  //CMQTTSNFORWARDER_MQTTSNFORWARDERARDUINO_HPP
