//
// Created by SomeDude on 28.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_LOGGING_ARDUINO_ARDUINOSERIAL_H_
#define CMQTTSNFORWARDER_FORWARDER_LOGGING_ARDUINO_ARDUINOSERIAL_H_

#include <logging/MqttSnLogging.h>
#include <Arduino.h>

int32_t arduino_serial_log_init(MqttSnLogger *logger);
void arduino_serial_log_deinit(MqttSnLogger *logger);
int32_t arduino_serial_log_flush(const MqttSnLogger *logger);
int32_t arduino_serial_log_str(const MqttSnLogger *logger, const char *str);
int32_t arduino_serial_log_char(const MqttSnLogger *logger, char c);

#endif //CMQTTSNFORWARDER_FORWARDER_LOGGING_ARDUINO_ARDUINOSERIAL_H_
