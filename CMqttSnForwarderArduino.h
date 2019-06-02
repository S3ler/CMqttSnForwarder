//
// Created by bele on 18.05.19.
//

#ifndef TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_
#define TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_

#include <platform/device_address.c>
#include <platform/device_address.h>
#include <platform/platform_compatibility.h>

// buffer
#include <ringbuffer/MqttSnFixedSizeRingBuffer.c>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>

// parser
#include <parser/MqttSnMessageParser.c>
#include <parser/MqttSnMessageParser.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.c>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>

// logger
#include <logging/MqttSnLoggingInterface.c>
#include <logging/MqttSnLoggingInterface.h>
// arduino serial
#include <logging/arduino/ArduinoSerial.cpp>
#include <logging/arduino/ArduinoSerial.hpp>

// forwarder
#include <forwarder/config/old/forwarder_config.c>
#include <forwarder/config/old/forwarder_config.h>
#include <forwarder/config/old/forwarder_config_logger.c>
#include <forwarder/config/old/forwarder_config_logger.h>
#include <forwarder/MqttSnForwarder.c>
#include <forwarder/MqttSnForwarder.h>

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
// gateway
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
#include <utility/arduino/eeprom/eeprom_config.c>
#include <utility/arduino/eeprom/eeprom_config.h>
#include <utility/arduino/eeprom/forwarder/forwarder_eeprom_loader.c>
#include <utility/arduino/eeprom/forwarder/forwarder_eeprom_loader.h>
#include <utility/arduino/eeprom/eeprom_config_logging.c>
#include <utility/arduino/eeprom/eeprom_config_logging.h>
#include <utility/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.c>
#include <utility/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.h>
// system
#include <utility/arduino/system/system.c>
#include <utility/arduino/system/system.h>
#include <utility/arduino/system/system_logging.c>
#include <utility/arduino/system/system_logging.h>

class CMqttSnForwarderArduino {

};

#endif //TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_
