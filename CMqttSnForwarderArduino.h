#ifndef CMQTTSNFORWARDER__CMQTTSNFORWARDERARDUINO_H_
#define CMQTTSNFORWARDER__CMQTTSNFORWARDERARDUINO_H_

#ifndef Arduino_h
#error "no Arduino environment"
#define PSTR(str) ((str))
#endif

#ifdef madpilot_mDNSResolver
#include <mDNSResolver.h>
#endif

#define DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT 5353
#define DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT 5352

#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN 2
#define CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH 64

#define WITH_UDP_BROADCAST
#define WITH_UDP_BROADCAST_GATEWAY
#define WITH_UDP_BROADCAST_CLIENT

#ifndef WITH_RESTART
#define WITH_RESTART
#endif

#ifndef WITH_LOGGING
#define WITH_LOGGING
#ifndef WITH_DEBUG_LOGGING
#define WITH_DEBUG_LOGGING
#endif
#endif

#ifndef WITH_UDP
#define WITH_UDP

#ifndef WITH_ARDUINO_CLIENT_NETWORK_UDP
#define WITH_ARDUINO_CLIENT_NETWORK_UDP
#endif

#ifndef WITH_ARDUINO_GATEWAY_NETWORK_UDP
#define WITH_ARDUINO_GATEWAY_NETWORK_UDP
#endif

#endif

#ifndef WITH_TCP
#define WITH_TCP

#ifndef WITH_ARDUINO_CLIENT_NETWORK_UDP
#define WITH_ARDUINO_CLIENT_NETWORK_TCP
#endif

#ifndef WITH_ARDUINO_GATEWAY_NETWORK_UDP
#define WITH_ARDUINO_GATEWAY_NETWORK_TCP
#endif

#endif

#ifndef WITH_ARDUINO_WIFI
#define WITH_ARDUINO_WIFI
#endif

#ifndef WITH_CONFIG_FILE
#define WITH_CONFIG_FILE
#endif

#ifndef WITH_EEPROM
#define WITH_EEPROM
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifdef WITH_EEPROM
#include "utility/arduino/system/system.c"
#include "utility/arduino/system/system.h"
#include "utility/arduino/eeprom/eeprom_config.c"
#include "utility/arduino/eeprom/eeprom_config.h"
#endif

#ifdef WITH_LOGGING
//#include "forwarder/MqttSnForwarderLogging.c"
#include "forwarder/MqttSnForwarderLogging.h"
#include "forwarder/logging/MqttSnForwarderLoggingBasic.c"
#include "forwarder/logging/MqttSnForwarderLoggingBasic.h"
#include "forwarder/logging/MqttSnForwarderLoggingMessages.c"
#include "forwarder/logging/MqttSnForwarderLoggingMessages.h"
#include "forwarder/logging/arduino/ArduinoSerial.c"
#include "forwarder/logging/arduino/ArduinoSerial.h"
#ifndef WITH_DEBUG_LOGGING
#endif
#endif

#include "forwarder/global_defines.h"
#include "forwarder/MqttSnClientNetworkInterface.c"
#include "forwarder/MqttSnClientNetworkInterface.h"
#include "forwarder/MqttSnFixedSizeRingBuffer.c"
#include "forwarder/MqttSnFixedSizeRingBuffer.h"
#include "forwarder/MqttSnForwarder.c"
#include "forwarder/MqttSnForwarder.h"
#include "forwarder/MqttSnForwarderLogging.c"
#include "forwarder/MqttSnForwarderLogging.h"
#include "forwarder/MqttSnGatewayNetworkInterface.c"
#include "forwarder/MqttSnGatewayNetworkInterface.h"
#include "forwarder/MqttSnMessageParser.c"
#include "forwarder/MqttSnMessageParser.h"

#include "forwarder/network/shared/ip/IpHelper.c"
#include "forwarder/network/shared/ip/IpHelper.h"

#include "main/forwarder_config.c"
#include "main/forwarder_config.h"

#if defined(WITH_ARDUINO_CLIENT_NETWORK_TCP) || defined(WITH_ARDUINO_GATEWAY_NETWORK_TCP) || defined(WITH_ARDUINO_CLIENT_NETWORK_UDP) || defined(WITH_ARDUINO_GATEWAY_NETWORK_UDP)
#include "forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.c"
#include "forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.h"
#endif

#if defined(WITH_ARDUINO_CLIENT_NETWORK_UDP) || defined(WITH_ARDUINO_GATEWAY_NETWORK_UDP)
#include "forwarder/network/arduino/shared/ip/udp/UdpHelper.c"
#include "forwarder/network/arduino/shared/ip/udp/UdpHelper.h"
#endif

#ifdef WITH_ARDUINO_CLIENT_NETWORK_UDP
#include "forwarder/network/arduino/client/ip/MqttSnClientUdpNetwork.c"
#include "forwarder/network/arduino/client/ip/MqttSnClientUdpNetwork.h"
#endif

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
#include "forwarder/network/arduino/gateway/ip/MqttSnGatewayUdpNetwork.c"
#include "forwarder/network/arduino/gateway/ip/MqttSnGatewayUdpNetwork.h"
#endif

#ifdef WITH_ARDUINO_CLIENT_NETWORK_TCP
#endif

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_TCP
#endif

#endif // CMQTTSNFORWARDER__CMQTTSNFORWARDERARDUINO_H_
