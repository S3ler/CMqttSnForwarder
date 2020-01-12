//
// Created by SomeDude on 20.09.2019.
//

#ifndef CMQTTSNFORWARDER_ARDUINOSD_H
#define CMQTTSNFORWARDER_ARDUINOSD_HPP

#include <logging/MqttSnLogging.h>
#include <Arduino.h>

int arduino_sd_log_init(MqttSnLogger *logger);
void arduino_sd_log_deinit(MqttSnLogger *logger);
int arduino_sd_log_flush(const MqttSnLogger *logger);
int arduino_sd_log_str(const char *str);
int arduino_sd_log_char(char c);

#endif  //CMQTTSNFORWARDER_ARDUINOSD_H
