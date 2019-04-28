#ifndef CMQTTSNFORWARDER__CMQTTSNFORWARDERARDUINO_H_
#define CMQTTSNFORWARDER__CMQTTSNFORWARDERARDUINO_H_

#ifndef Arduino_h
#error "no arduino environment"
#endif
#ifndef WITH_LOGGING
#define WITH_LOGGING
#endif
#ifndef WITH_DEBUG_LOGGING
#define WITH_DEBUG_LOGGING
#endif
#ifdef WITH_LOGGING
#include "forwarder/MqttSnForwarderLogging.h"
#include "forwarder/logging/MqttSnForwarderLoggingBasic.h"
#include "forwarder/logging/MqttSnForwarderLoggingMessages.h"
#include "forwarder/logging/arduino/ArduinoSerial.h"
#ifndef WITH_DEBUG_LOGGING
#endif
#endif

#include "forwarder/global_defines.h"
#include "forwarder/MqttSnClientNetworkInterface.h"
#include "forwarder/MqttSnFixedSizeRingBuffer.h"
#include "forwarder/MqttSnForwarder.h"
#include "forwarder/MqttSnForwarderLogging.h"
#include "forwarder/MqttSnGatewayNetworkInterface.h"
#include "forwarder/MqttSnMessageParser.h"

#include "main/forwarder_config.h"

#endif // CMQTTSNFORWARDER__CMQTTSNFORWARDERARDUINO_H_
