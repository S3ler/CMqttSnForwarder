//
// Created by SomeDude on 11.05.2019.
//

#ifndef CMQTTSNFORWARDER_PLATFORM_COMPATIBILITY_H_
#define CMQTTSNFORWARDER_PLATFORM_COMPATIBILITY_H_
#ifdef __cplusplus
extern "C" {
#endif

#if defined(Arduino_h) // Arduino
#ifdef WITH_LOGGING
#include <forwarder/logging/arduino/ArduinoSerial.h>
#endif
#else // Linux
#ifdef WITH_LOGGING
#include <netinet/in.h>
#include <logging/linux/stdout/StdoutLogging.h>
#endif

#define PSTR(str) (str)
#if defined(WITH_LINUX_FAKE_ARDUINO_CLASSES)
#include <digital.h>
#include <ESPClass.h>
#include <SerialClass.h>
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PLATFORM_COMPATIBILITY_H_
