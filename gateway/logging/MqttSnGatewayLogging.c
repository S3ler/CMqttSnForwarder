//
// Created by SomeDude on 20.06.2019.
//

#include <parser/logging/common/MqttSnMessageLogging.h>
#include <platform/platform_compatibility.h>
#include "MqttSnGatewayLogging.h"

int32_t log_gateway_status(const MqttSnLogger *logger, MQTT_SN_GATEWAY_STATUS status) {
  if (status == MQTT_SN_GATEWAY_STATUS_ACTIVE) {
    log_str(logger, "active");
  } else if (status == MQTT_SN_GATEWAY_STATUS_STAND_BY) {
    log_str(logger, "stand-by");
  } else {
    log_str(logger, "TODO undefined");
  }
  return log_status(logger);
}
int32_t log_unhandled_message(const MqttSnLogger *logger,
                              MQTT_SN_GATEWAY_STATUS status,
                              device_address *from,
                              uint8_t signal_strength,
                              uint8_t *data,
                              int32_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, PSTR("gateway status: "));
  log_gateway_status(logger, status);
  log_str(logger, PSTR(" unhandled message "));
  return print_any_message(logger, from, signal_strength, data, data_len);
}
