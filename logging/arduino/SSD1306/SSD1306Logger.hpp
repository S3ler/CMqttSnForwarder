//
// Created by bele on 11.06.19.
//

#ifndef CMQTTSNFORWARDER_LOGGING_ARDUINO_SSD1306_SSD1306LOGGER_H_
#define CMQTTSNFORWARDER_LOGGING_ARDUINO_SSD1306_SSD1306LOGGER_H_

#include <logging/MqttSnLoggingInterface.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306.h>
#include <Arduino.h>

#define SSD1306_SCREEN_WIDTH    128
#define SSD1306_SCREEN_HEIGHT   64

// SSD1306  display(0x3c, 5, 4);
#define SSD1306_SCREEN_ADDRESS  0x3c
#define SSD1306_SCREEN_SDA      5
#define SSD1306_SCREEN_SCL      4

#define SSD1306_LINE_COUNT 4
#define SSD1306_LINE_LENGTH 26
extern SSD1306 display;

typedef struct ssd1306status_ {
  uint16_t screen_width;
  uint16_t screen_height;
  uint16_t line_count;
  uint16_t line_length;
  char line[SSD1306_LINE_COUNT][SSD1306_LINE_LENGTH];
  uint16_t current_line;
  uint16_t current_line_count;
} ssd1306status;

extern ssd1306status display_status;

int32_t ssd1306_screen_log_init(MqttSnLogger *logger);
void ssd1306_screen_log_deinit(MqttSnLogger *logger);
int32_t ssd1306_screen_log_flush(const MqttSnLogger *logger);
int32_t ssd1306_screen_log_str(const char *str);
int32_t ssd1306_screen_log_char(char c);

#endif //CMQTTSNFORWARDER_LOGGING_ARDUINO_SSD1306_SSD1306LOGGER_H_
