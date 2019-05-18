//
// Created by SomeDude on 28.04.2019.
//

#include "ArduinoSerial.hpp"

int arduino_serial_log_init(MqttSnLogger *logger) {
  logger->log_init = arduino_serial_log_init;
  logger->log_deinit = arduino_serial_log_deinit;
  logger->log_flush = arduino_serial_log_flush;
  logger->log_str = arduino_serial_log_str;
  logger->log_char = arduino_serial_log_char;
  if (!Serial) {
    return -1;
  }
  return 0;
}

void arduino_serial_log_deinit(MqttSnLogger *logger) {
  Serial.flush();
  //Serial.end();
}

int arduino_serial_log_flush(const MqttSnLogger *logger) {
  if (!Serial) {
    return -1;
  }
  if (logger->log_str("\n")) {
    return -1;
  }
  Serial.flush();
  return 0;
}

int arduino_serial_log_str(const char *str) {
  if (!Serial) {
    return -1;
  }
  Serial.print(str);
  return 0;
}
int arduino_serial_log_char(char c) {
  if (!Serial) {
    return -1;
  }
  Serial.print(c);
  return 0;
}