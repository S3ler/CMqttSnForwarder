//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTARDUINO_HPP
#define CMQTTSNFORWARDER_MQTTSNCLIENTARDUINO_HPP

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

#define WITH_GATEWAY_NETWORK_BROADCAST
#define WITH_ARDUINO_GATEWAY_NETWORK_UDP

#define WITH_CLIENT_NETWORK_BROADCAST
#define WITH_ARDUINO_CLIENT_NETWORK_UDP

#define WITH_RESTART
#define WITH_EEPROM
#define WITH_LOGGING
#define WITH_DEBUG_LOGGING

#define MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH 64

#define DEFAULT_MQTT_SN_GATEWAY_HOST "arsmb.de"

#define DEFAULT_MQTT_SN_CLIENT_BIND_PORT 10003
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_ADDRESS "225.1.1.2"
#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT 1883

#define MQTT_SN_CLIENT_ARDUINO_IDENTIFIER "MqttSnArduinoClient"

/*
#define MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH 1
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH 1
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH 1
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_BLACKLIST_LENGTH 1
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_BLACKLIST_LENGTH 1
*/

#include <platform/platform_compatibility.c>
#include <platform/platform_compatibility.h>

#include <platform/device_address.c>
#include <platform/device_address.h>
#include <platform/MqttSnMessageData.c>
#include <platform/MqttSnMessageData.h>

#include <MqttSnDiscoveredGatewayList.hpp>
#include <MqttSnGatewayNetworkArduino.hpp>
#include <MqttSnParserArduino.hpp>

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

#include <config/network/gateway/gateway_network_config.c>
#include <config/network/gateway/gateway_network_config.h>

#include <config/client/client_find_mqtt_sn_gateway_config.c>
#include <config/client/client_find_mqtt_sn_gateway_config.h>
#include <config/client/client_connect_config.c>
#include <config/client/client_connect_config.h>
#include <config/client/client_mqtt_sn_gateway_config.c>
#include <config/client/client_mqtt_sn_gateway_config.h>
#include <config/client/client_network_parameter_config.c>
#include <config/client/client_network_parameter_config.h>
#include <config/client/client_publish_config.c>
#include <config/client/client_publish_config.h>
#include <config/client/client_register_config.c>
#include <config/client/client_register_config.h>
#include <config/client/client_searchgw_reply_config.c>
#include <config/client/client_searchgw_reply_config.h>
#include <config/client/client_sleep_config.c>
#include <config/client/client_sleep_config.h>
#include <config/client/client_subscribe_config.c>
#include <config/client/client_subscribe_config.h>
#include <config/client/client_unsubscribe_config.c>
#include <config/client/client_unsubscribe_config.h>


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

// command buffer
#include <config/common/confg_command_buffer.c>
#include <config/common/confg_command_buffer.h>

// interactive
#include <client/interactive/starter/mqtt_sn_client_interactive_action_type.h>
#include <client/interactive/starter/mqtt_sn_client_interactive_action_result_type.h>
#include <client/interactive/starter/mqtt_sn_client_interactive.c>
#include <client/interactive/starter/mqtt_sn_client_interactive.h>
#include <client/interactive/starter/mqtt_sn_client_interactive_cmd_result.c>
#include <client/interactive/starter/mqtt_sn_client_interactive_cmd_result.h>
#include <client/interactive/config/interactive_client_config.c>
#include <client/interactive/config/interactive_client_config.h>

// mqtt-sn client
#include <client/MqttSnClientReturnCode.h>
#include <client/MqttSnClientMsgHandlerReturnValue.h>
#include <client/MqttSnClientConnectionStatus.h>
#include <client/MqttSnClientRegistration.h>
#include <client/MqttSnClientAwaitMessageStrut.h>
#include <client/MqttSnClientFindGatewayPattern.h>
#include <client/MqttSnClient.c>
#include <client/MqttSnClient.h>
#include <client/MqttSnClientMethod.c>
#include <client/MqttSnClientMethod.h>
#include <client/MqttSnClientAwaitMessage.c >
#include <client/MqttSnClientAwaitMessage.h>
#include <client/MqttSnClientReceivedAdvertise.h>
#include <client/MqttSnClientReceivedGwInfo.h>
#include <client/MqttSnClientLogger.c>
#include <client/MqttSnClientLogger.h>



#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTARDUINO_HPP
