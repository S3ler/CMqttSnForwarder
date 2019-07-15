//
// Created by bele on 20.05.19.
//

#include <platform/platform_compatibility.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <parser/logging/common/MqttSnMessageLogging.h>
#include "MqttSnGwInfoMessageLogging.h"
#include "MqttSnForwarderLoggingMessages.h"

int32_t log_gwinfo_gw_add(const MqttSnLogger *logger, const device_address *gw_add, uint16_t gw_add_len) {
  log_str(logger, PSTR("gwAdd"));
  const char *dot = ".";
  for (size_t i = 0; i < gw_add_len; ++i) {
    if (log_uint8(logger, gw_add->bytes[i])) {
      return -1;
    }
    if (i + 1 < sizeof(device_address)) {
      if (log_str(logger, dot)) {
        return -1;
      }
    }
  }
  return log_status(logger);
}
int32_t log_gwinfo_gw_id(const MqttSnLogger *logger, uint8_t gw_id) {
  return log_str_key_uint8_value(logger, PSTR("gw"), gw_id);
}
int32_t log_gwinfo_message(const MqttSnLogger *logger,
                           uint8_t gw_id,
                           const device_address *gw_add,
                           uint16_t gw_add_len) {
  log_str(logger, PSTR("( "));
  log_gwinfo_gw_id(logger, gw_id);
  if (gw_add != NULL && gw_add_len > 0) {
    log_str(logger, PSTR(", "));
    log_gwinfo_gw_add(logger, gw_add, gw_add_len);
  }
  log_str(logger, PSTR(")"));
  return log_status(logger);
}
int32_t log_gwinfo_message_byte(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_len) {
  uint8_t gw_id = 0;
  device_address gw_add = {0};
  uint16_t gw_add_len = 0;
  if (parse_gwinfo_message_byte(&gw_id, &gw_add, &gw_add_len, data, data_len) < 0) {
    return log_status(logger);
  }
  return log_gwinfo_message(logger, gw_id, &gw_add, gw_add_len);
}
int32_t log_gwinfo_message_struct(const MqttSnLogger *logger, MqttSnGwInfo *gw_info) {
  return log_gwinfo_message(logger, gw_info->gwId, &gw_info->gwAdd, gw_info->gwAddLen);
}
int32_t log_gwinfo_gen_error(const MqttSnLogger *logger,
                             const char *file_name,
                             const char *function_name,
                             int line_number) {
  return log_msg_gen_error(logger, GWINFO, file_name, function_name, line_number);
}

