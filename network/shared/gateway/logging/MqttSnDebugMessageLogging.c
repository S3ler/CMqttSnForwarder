//
// Created by SomeDude on 11.05.2019.
//

#include "MqttSnDebugMessageLogging.h"
#include <logging/MqttSnLoggingInterface.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>
#include <platform/platform_compatibility.h>

int log_db_rec_gateway_message(const MqttSnLogger *logger,
                               const device_address *from,
                               const device_address *to,
                               const uint8_t *data,
                               uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  log_msg_start(logger);
  log_str(logger, PSTR("receive gateway message from "));
  log_device_address(logger, from);
  if (to) {
    log_str(logger, PSTR(" to "));
    log_device_address(logger, to);
  }
  log_str(logger, PSTR(" ( len"));
  log_uint16(logger, data_len);
  log_str(logger, PSTR(", data( "));
  log_uint8_array(logger, data, data_len);
  log_str(logger, PSTR("))."));

  log_flush(logger);
  return log_status(logger);
}

int log_db_send_gateway_message(const MqttSnLogger *logger,
                                const device_address *from,
                                const device_address *dst,
                                const uint8_t *data,
                                uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  log_msg_start(logger);
  log_str(logger, PSTR("send gateway message from "));
  log_device_address(logger, from);
  log_str(logger, PSTR(" to "));
  log_device_address(logger, dst);
  log_str(logger, PSTR(" ( len"));
  log_uint32(logger, data_len);
  log_str(logger, PSTR(", data( "));
  log_uint8_array(logger, data, data_len);
  log_str(logger, PSTR("))."));

  log_flush(logger);
  return log_status(logger);
}
int log_incomplete_gateway_message(const MqttSnLogger *logger,
                                   const device_address *address,
                                   const uint8_t *data,
                                   uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *description = "could not send message completely - try again later: ";
  return log_gateway_mqtt_sn_message(logger, address, data, data_len, description);
}
