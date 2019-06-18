//
// Created by SomeDude on 11.05.2019.
//

#ifndef CMQTTSNFORWARDER_PLATFORM_COMPATIBILITY_H_
#define CMQTTSNFORWARDER_PLATFORM_COMPATIBILITY_H_

#if defined(Arduino_h) || defined(WITH_PLATFORMIO)// Arduino

#include <stdint.h>
#include <WiFiUdp.h>
#define WITH_LOGGING
#define WITH_DEBUG_LOGGING

#ifdef WITH_LOGGING
#include <logging/arduino/serial/ArduinoSerial.hpp>
#endif

// ESP32
#include <lwip/sockets.h>
#include <lwip/netdb.h>

#define MQTT_SN_PARSE_CONFIG_FAILURE -1
#define MQTT_SN_PARSE_SUCCESS 1
#define MQTT_SN_PARSE_CONFIG_HELP 2
#define MQTT_SN_PARSE_CONFIG_SUCCESS 0
#else // Linux

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

#ifdef WITH_LOGGING
#include <netinet/in.h>
#include <logging/linux/stdout/StdoutLogging.h>
#endif
#include <time.h>
#include <string.h>
#define MQTT_SN_PARSE_CONFIG_FAILURE -1
#define MQTT_SN_PARSE_SUCCESS 1
#define MQTT_SN_PARSE_CONFIG_HELP 2
#define MQTT_SN_PARSE_CONFIG_SUCCESS 0
//#define strcmp_P(str, pstr) strcmp(str, pstr)
int strcmp_P(const char *a, const char *b);
#define PSTR(str) (str)
#if defined(WITH_LINUX_FAKE_ARDUINO_CLASSES)
#include <digital.h>
#include <ESPClass.h>
#include <SerialClass.h>
#endif

/*
#ifdef __cplusplus
}
#endif
*/
int32_t PlatformCompatibilityGetTimestamp(uint64_t *t);
#endif // Linux

#endif //CMQTTSNFORWARDER_PLATFORM_COMPATIBILITY_H_
