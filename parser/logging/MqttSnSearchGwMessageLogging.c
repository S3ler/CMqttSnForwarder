//
// Created by bele on 20.05.19.
//

#include <parser/MqttSnSearchGwMessage.h>
#include "MqttSnSearchGwMessageLogging.h"
#include "MqttSnForwarderLoggingMessages.h"

int32_t log_searchgw_message_byte(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_len) {
  uint8_t radius;
  if (parse_searchgw_message_byte(&radius, data, data_len) < 0) {
    return log_status(logger);
  }
  return log_searchgw_message(logger, radius);
}
int32_t log_searchgw_message(const MqttSnLogger *logger, uint8_t radius) {
  log_open_braked(logger);
  log_searchgw_radius(logger, radius);
  log_close_braked(logger);
  return log_status(logger);
}
int32_t log_searchgw_radius(const MqttSnLogger *logger, uint8_t radius) {
  return log_char_key_uint8_value(logger, 'r', radius);
}
