//
// Created by bele on 20.05.19.
//

#include <parser/MqttSnSearchGwMessage.h>
#include <platform/platform_compatibility.h>

#include "MqttSnSearchGwMessageLogging.h"
#include "MqttSnForwarderLoggingMessages.h"
#include "MqttSnMessageParserLogging.h"

int32_t log_searchgw_message_byte(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_len) {
  uint8_t radius;
  if (parse_searchgw_message_byte(&radius, data, data_len) < 0) {
    return log_status(logger);
  }
  return log_searchgw_message(logger, radius);
}
int32_t log_searchgw_message(const MqttSnLogger *logger, uint8_t radius) {
  log_str(logger, PSTR("( "));
  log_searchgw_radius(logger, radius);
  log_str(logger, PSTR(")"));
  return log_status(logger);
}
int32_t log_searchgw_radius(const MqttSnLogger *logger, uint8_t radius) {
  return log_char_key_uint8_value(logger, 'r', radius);
}
int32_t log_searchgw_message_struct(const MqttSnLogger *logger, MqttSnSearchGw *search_gw) {
  return log_searchgw_message(logger, search_gw->radius);
}
int32_t log_searchgw_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number) {
    return log_msg_gen_error(logger, SEARCHGW, file_name, function_name, line_number);
}
