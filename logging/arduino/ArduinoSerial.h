//
// Created by SomeDude on 28.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_LOGGING_ARDUINO_ARDUINOSERIAL_H_
#define CMQTTSNFORWARDER_FORWARDER_LOGGING_ARDUINO_ARDUINOSERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <logging/MqttSnLoggingInterface.h>

int arduino_serial_log_init(MqttSnLogger *logger);
void arduino_serial_log_deinit(MqttSnLogger *logger);
int arduino_serial_log_flush(const MqttSnLogger *logger);
int arduino_serial_log_str(const char *str);
int arduino_serial_log_char(char c);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_FORWARDER_LOGGING_ARDUINO_ARDUINOSERIAL_H_
