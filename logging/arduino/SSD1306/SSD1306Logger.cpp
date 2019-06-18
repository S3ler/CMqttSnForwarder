//
// Created by bele on 11.06.19.
//

#include "SSD1306Logger.hpp"
#include <string.h>

int32_t ssd1306_screen_log_init(MqttSnLogger *logger) {
  logger->log_init = ssd1306_screen_log_init;
  logger->log_deinit = ssd1306_screen_log_deinit;
  logger->log_flush = ssd1306_screen_log_flush;
  logger->log_str = ssd1306_screen_log_str;
  logger->log_char = ssd1306_screen_log_char;

  memset(&display_status, 0, sizeof(display_status));
  display_status.screen_width = SSD1306_SCREEN_WIDTH;
  display_status.screen_height = SSD1306_SCREEN_HEIGHT;
  display_status.line_count = SSD1306_LINE_COUNT;
  display_status.line_length = SSD1306_LINE_LENGTH;

  display.setLogBuffer(5, 30);
  display.clear();
  display.display();
  return 0;
}
void ssd1306_screen_log_deinit(MqttSnLogger *logger) {
  display.resetDisplay();
  display.end();
}
int32_t ssd1306_screen_log_flush(const MqttSnLogger *logger) {
  Serial.println();

  display.clear();
  display.display();

  for (uint16_t i = 0; i <= display_status.current_line; i++) {
    display.println(display_status.line[i]);
    display.display();
  }

  display.drawLogBuffer(0, 0);
  display.display();
  memset(&display_status.line,0,sizeof(display_status.line));
  display_status.current_line = 0;
  display_status.current_line_count = 0;
  return 0;
}
int32_t ssd1306_screen_log_str(const char *str) {
  Serial.print(str);
  if (display_status.current_line > display_status.line_count) {
    return 0;
  }
  uint16_t str_len = strlen(str);
  for (uint16_t i = 0; i < str_len; i++) {
    display_status.line[display_status.current_line][display_status.current_line_count] = str[i];
    display_status.current_line_count++;
    if (display_status.current_line_count + 1 == display_status.line_length) {
      display_status.current_line++;
      display_status.current_line_count = 0;
      if(display_status.current_line > display_status.line_count){
        return 0;
        // TODO flush
      }
    }
  }
  return 0;
}
int32_t ssd1306_screen_log_char(char c) {
  char str[2];
  str[0] = c;
  str[1] = '\0';
  return ssd1306_screen_log_str(str);
}
