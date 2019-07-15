//
// Created by SomeDude on 14.07.2019.
//

#include <gateway/database/db_handler.h>
#include <platform/platform_compatibility.h>
#include "MqttSnGatewayDiscoveryLogging.h"

int32_t log_db_entry_discovered_result(const MqttSnLogger *logger,
                                       DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT db_entry_result) {
  if (db_entry_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS) {
    log_str(logger, PSTR("RESULT_SUCCESS"));
  } else if (db_entry_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS) {
    log_str(logger, PSTR("RESULT_IN_PROGRESS"));
  } else if (db_entry_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR) {
    log_str(logger, PSTR("RESULT_ERROR"));
  } else {
    log_str(logger, PSTR("RESULT_UNKNOWN"));
  }
  return log_status(logger);
}
int32_t log_db_entry_discovered_db_handler_result(const MqttSnLogger *logger, DB_HANDLER_RESULT db_handler_result) {
  if (db_handler_result == DB_HANDLER_RESULT_SUCCESS) {
    log_str(logger, PSTR("RESULT_SUCCESS"));
  } else if (db_handler_result == DB_HANDLER_RESULT_NO_TRANSACTION_STARTED) {
    log_str(logger, PSTR("RESULT_NO_TRANSACTION_STARTED"));
  } else if (db_handler_result == DB_HANDLER_RESULT_ERROR) {
    log_str(logger, PSTR("RESULT_ERROR"));
  } else {
    log_str(logger, PSTR("RESULT_UNKNOWN"));
  }
  return log_status(logger);
}
int32_t log_fatal_discovery_db_error(const MqttSnLogger *logger,
                                     const char *file_name,
                                     const char *function_name,
                                     int line_number,
                                     DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT db_entry_result,
                                     DB_HANDLER_RESULT db_handler_result) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);

  log_str(logger, PSTR("fatal error in file: "));
  log_str(logger, file_name);
  log_str(logger, PSTR(" function: "));
  log_str(logger, function_name);
  log_str(logger, PSTR(" line: "));
  log_int32(logger, line_number);
  log_str(logger, PSTR(" db_entry_result: "));
  log_db_entry_discovered_result(logger, db_entry_result);
  log_str(logger, PSTR(" db_handler_result: "));
  log_db_entry_discovered_db_handler_result(logger, db_handler_result);
  log_flush(logger);
  return log_status(logger);
}

int32_t log_discovery_handler_initialized(const MqttSnLogger *logger, int32_t result) {
  log_msg_start(logger);
  log_str(logger, PSTR("discovery handler init "));
  if (result < 0) {
    log_str(logger, PSTR("failed"));
  } else {
    log_str(logger, PSTR("success"));
  }
  log_flush(logger);
  return log_status(logger);
}
