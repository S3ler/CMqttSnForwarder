//
// Created by bele on 18.05.19.
//

#ifndef TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_
#define TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_

#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN 1
#define MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH 64

#define WITH_EEPROM
#define WITH_LOGGING
#define WITH_DEBUG_LOGGING

#define WITH_UDP_BROADCAST

#define WITH_ARDUINO_GATEWAY_NETWORK_UDP
#define WITH_UDP_BROADCAST_CLIENT
#define WITH_GATEWAY_NETWORK_BROADCAST

#define WITH_ARDUINO_CLIENT_NETWORK_UDP
#define WITH_UDP_BROADCAST_GATEWAY
#define WITH_CLIENT_NETWORK_BROADCAST

#define WITH_RESTART

#include <Arduino.h>

#include <platform/platform_compatibility.h>

#include <platform/device_address.c>
#include <platform/device_address.h>

// buffer
#include <ringbuffer/MqttSnFixedSizeRingBuffer.c>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>

// parser
#include <parser/MqttSnMessageParser.c>
#include <parser/MqttSnMessageParser.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.c>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>
#include <parser/MqttSnAdvertiseMessage.c>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/MqttSnGwInfoMessage.c>
#include <parser/MqttSnGwInfoMessage.h>
#include <parser/MqttSnSearchGwMessage.c>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/MqttSnForwarderEncapsulationMessage.c>
#include <parser/MqttSnForwarderEncapsulationMessage.h>
// logger
#include <logging/MqttSnLoggingInterface.c>
#include <logging/MqttSnLoggingInterface.h>
// arduino serial
#include "logging/arduino/serial/ArduinoSerial.cpp"
#include "logging/arduino/serial/ArduinoSerial.hpp"

// forwarder
#include <forwarder/config/forwarder_config.c>
#include <forwarder/config/forwarder_config.h>
#include <forwarder/MqttSnForwarder.c>
#include <forwarder/MqttSnForwarder.h>
#include <forwarder/MqttSnForwarderParser.c>
#include <forwarder/MqttSnForwarderParser.h>
#include <forwarder/MqttSnForwarderLogging.c>
#include <forwarder/MqttSnForwarderLogging.h>

// network
// client
#include <network/MqttSnClientNetworkInterface.c>
#include <network/MqttSnClientNetworkInterface.h>
// network
#include <network/MqttSnGatewayNetworkInterface.c>
#include <network/MqttSnGatewayNetworkInterface.h>
// shared
#include <network/shared/ip/IpHelper.c>
#include <network/shared/ip/IpHelper.h>
#include <network/shared/ip/IpHelperLogging.c>
#include <network/shared/ip/IpHelperLogging.h>
#include <network/arduino/shared/ip/ArduinoIpAddressHelper.cpp>
#include <network/arduino/shared/ip/ArduinoIpAddressHelper.hpp>
#include <network/arduino/shared/ip/udp/UdpHelper.cpp>
#include <network/arduino/shared/ip/udp/UdpHelper.hpp>
// udp
// gateway         arduino_serial_eval_process(eeprom_cfg, &fcfg, line, line_pos);
#include <network/arduino/gateway/ip/MqttSnGatewayUdpNetwork.cpp>
#include <network/arduino/gateway/ip/MqttSnGatewayUdpNetwork.hpp>
#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.c>
#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.h>
// client
#include <network/arduino/client/ip/MqttSnClientUdpNetwork.cpp>
#include <network/arduino/client/ip/MqttSnClientUdpNetwork.hpp>
#include <network/shared/client/logging/MqttSnDebugMessageLogging.c>
#include <network/shared/client/logging/MqttSnDebugMessageLogging.h>
// utility
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
// system
#include <config/common/arduino/system/system.c>
#include <config/common/arduino/system/system.h>
#include <config/common/arduino/system/system_logging.c>
#include <config/common/arduino/system/system_logging.h>

#include <config/common/mqtt_sn_logger_config.c>
#include <config/common/mqtt_sn_logger_config.h>

#include <config/common/mqtt_sn_version_config.c>
#include <config/common/mqtt_sn_version_config.h>

#include <config/client/mqtt_sn_gateway_config.c>
#include <config/client/mqtt_sn_gateway_config.h>

#include <config/client/client_find_mqtt_sn_gateway_config.c>
#include <config/client/client_find_mqtt_sn_gateway_config.h>

#include <config/network/gateway/gateway_network_config.c>
#include <config/network/gateway/gateway_network_config.h>

#include <config/network/client/client_network_config.c>
#include <config/network/client/client_network_config.h>

#include <parser/logging/MqttSnSearchGwMessageLogging.c>
#include <parser/logging/MqttSnSearchGwMessageLogging.h>

#include <parser/logging/MqttSnGwInfoMessageLogging.c>
#include <parser/logging/MqttSnGwInfoMessageLogging.h>

#include <parser/logging/MqttSnAdvertiseMessageLogging.c>
#include <parser/logging/MqttSnAdvertiseMessageLogging.h>


//#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.c>
//#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.h>
/*
class CMqttSnForwarderArduino {

};
*/
#endif //TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_
