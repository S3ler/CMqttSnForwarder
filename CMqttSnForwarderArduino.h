//
// Created by bele on 18.05.19.
//

#ifndef TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_
#define TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_

#include <platform/platform_compatibility.h>
#include <platform/device_address.h>
#include <utility/arduino/system/system.h>
#include <utility/arduino/eeprom/eeprom_config.h>
#include <utility/arduino/eeprom/forwarder/forwarder_eeprom_loader.h>
#include <logging/MqttSnLoggingInterface.h>
#include <forwarder/config/forwarder_config.h>
#include <forwarder/MqttSnForwarder.h>

#include <network/shared/ip/IpHelper.h>
#include <network/arduino/gateway/ip/MqttSnGatewayUdpNetwork.hpp>
#include <network/arduino/client/ip/MqttSnClientUdpNetwork.hpp>

#include <utility/arduino/system/system_logging.h>
#include <forwarder/MqttSnForwarderLogging.h>

class CMqttSnForwarderArduino {

};

#endif //TESTPROJECT02_LIB_CMQTTSNFORWARDER_CMQTTSNFORWARDERARDUINO_H_
