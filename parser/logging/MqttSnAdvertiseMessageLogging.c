//
// Created by bele on 20.05.19.
//

#include <platform/platform_compatibility.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/logging/common/MqttSnMessageLogging.h>
#include "MqttSnAdvertiseMessageLogging.h"
#include "MqttSnForwarderLoggingMessages.h"

int32_t log_advertise_message_byte(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_len) {
  uint8_t gw_id;
  uint16_t duration;
  if (parse_advertise_byte(&gw_id, &duration, data, data_len) < 0) {
    return log_status(logger);
  }
  return log_advertise_message(logger, gw_id, duration);
}
int32_t log_advertise_message(const MqttSnLogger *logger, uint8_t gw_id, uint16_t duration) {
  log_str(logger, PSTR("( "));
  log_advertise_gw_id(logger, gw_id);
  log_str(logger, PSTR(", "));
  log_advertise_duration(logger, duration);
  log_str(logger, PSTR(")"));
  return log_status(logger);
}
int32_t log_advertise_message_struct(const MqttSnLogger *logger, MqttSnAdvertise *advertise) {
  return log_advertise_message(logger, advertise->gwId, advertise->duration);
}
int32_t log_advertise_gw_id(const MqttSnLogger *logger, uint8_t gw_id) {
  return log_str_key_uint8_value(logger, PSTR("gw"), gw_id);
}
int32_t log_advertise_duration(const MqttSnLogger *logger, uint16_t duration) {
  return log_str_key_uint16_value(logger, PSTR("dur"), duration);
}
int32_t log_advertise_gen_error(const MqttSnLogger *logger,
                                const char *file_name,
                                const char *function_name,
                                int line_number) {
  return log_msg_gen_error(logger, ADVERTISE, file_name, function_name, line_number);
}
