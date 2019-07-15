//
// Created by SomeDude on 15.07.2019.
//

#include <platform/platform_compatibility.h>
#include <parser/logging/MqttSnMessageParserLogging.h>
#include "MqttSnGatewayClientConnectionLogging.h"

int32_t log_db_entry_client_result(const MqttSnLogger *logger, DB_ENTRY_MQTT_SN_CLIENT_RESULT db_entry_result) {
  if (db_entry_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
    log_str(logger, PSTR("RESULT_SUCCESS"));
  } else if (db_entry_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_SHORT) {
    log_str(logger, PSTR("RESULT_CLIENT_ID_TOO_SHORT"));
  } else if (db_entry_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_LONG) {
    log_str(logger, PSTR("RESULT_CLIENT_ID_TOO_LONG"));
  } else if (db_entry_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_NOT_FOUND) {
    log_str(logger, PSTR("RESULT_CLIENT_NOT_FOUND"));
  } else if (db_entry_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR) {
    log_str(logger, PSTR("RESULT_ERROR"));
  } else {
    log_str(logger, PSTR("RESULT_UNKNOWN"));
  }
  return log_status(logger);
}
int32_t log_db_handler_result(const MqttSnLogger *logger, DB_HANDLER_RESULT db_handler_result) {
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
int32_t log_fatal_client_db_error(const MqttSnLogger *logger,
                                  const char *file_name,
                                  const char *function_name,
                                  int32_t line_number,
                                  DB_ENTRY_MQTT_SN_CLIENT_RESULT db_entry_result,
                                  DB_HANDLER_RESULT db_handler_result) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);

  log_str(logger, PSTR("fatal db error in file: "));
  log_str(logger, file_name);
  log_str(logger, PSTR(" function: "));
  log_str(logger, function_name);
  log_str(logger, PSTR(" line: "));
  log_int32(logger, line_number);
  log_str(logger, PSTR(" db_entry_result: "));
  log_db_entry_client_result(logger, db_entry_result);
  log_str(logger, PSTR(" db_handler_result: "));
  log_db_handler_result(logger, db_handler_result);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_fatal_client_connection_check_error(const MqttSnLogger *logger,
                                                const char *file_name,
                                                const char *function_name,
                                                int32_t line_number) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);

  log_str(logger, PSTR("fatal client check error in file: "));
  log_str(logger, file_name);
  log_str(logger, PSTR(" function: "));
  log_str(logger, function_name);
  log_str(logger, PSTR(" line: "));
  log_int32(logger, line_number);

  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_client_lost(const MqttSnLogger *logger,
                                          const device_address *wireless_node_id,
                                          const char *client_id,
                                          uint16_t connect_duration) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, PSTR("client lost "));
  log_device_address(logger, wireless_node_id);
  log_str(logger, PSTR(" "));
  log_str(logger, client_id);
  log_str(logger, PSTR(" "));
  log_duration(logger, connect_duration);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_client_connected(const MqttSnLogger *logger,
                                               const device_address *wireless_node_id,
                                               const char *client_id,
                                               uint16_t connect_duration,
                                               uint8_t clean_session) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, PSTR("client connected "));
  log_device_address(logger, wireless_node_id);
  log_str(logger, PSTR(" "));
  log_str(logger, client_id);
  log_str(logger, PSTR(" "));
  log_duration(logger, connect_duration);
  log_clean_session_flag(logger, clean_session);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_checking_client_timeout(const MqttSnLogger *logger, uint64_t client_count) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);

  log_str(logger, PSTR("checking client connection timeout for "));
  log_uint64(logger, client_count);
  log_str(logger, PSTR(" clients"));

  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_ping_req_received(const MqttSnLogger *logger, const device_address *wireless_node_id) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_msg_from(logger, PINGREQ, wireless_node_id);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_client_send_ping_req(const MqttSnLogger *logger,
                                                   const device_address *wireless_node_id,
                                                   const char *client_id,
                                                   uint16_t connect_duration,
                                                   uint64_t ping_req_received,
                                                   uint64_t ping_resp_received) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_msg_to(logger, PINGREQ, wireless_node_id);
  log_str(logger, client_id);
  log_duration(logger, connect_duration);
  log_str(logger, PSTR(" last ping req "));
  log_uint64(logger, ping_req_received);
  log_str(logger, PSTR(" last ping resp "));
  log_uint64(logger, ping_resp_received);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_ping_resp_received(const MqttSnLogger *logger, const device_address *wireless_node_id) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_msg_from(logger, PINGRESP, wireless_node_id);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_client_ping_resp_received(const MqttSnLogger *logger,
                                                        const device_address *wireless_node_id,
                                                        const char *client_id,
                                                        uint16_t connect_duration,
                                                        uint64_t ping_req_received,
                                                        uint64_t ping_resp_received) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_msg_from(logger, PINGRESP, wireless_node_id);
  log_str(logger, client_id);
  log_duration(logger, connect_duration);
  log_str(logger, PSTR(" last ping req "));
  log_uint64(logger, ping_req_received);
  log_str(logger, PSTR(" last ping resp "));
  log_uint64(logger, ping_resp_received);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_client_connection_client_ping_req_received(const MqttSnLogger *logger,
                                                       const device_address *wireless_node_id,
                                                       const char *client_id,
                                                       uint16_t connect_duration,
                                                       uint64_t ping_req_received,
                                                       uint64_t ping_resp_received) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_msg_from(logger, PINGREQ, wireless_node_id);
  log_str(logger, client_id);
  log_duration(logger, connect_duration);
  log_str(logger, PSTR(" last ping req "));
  log_uint64(logger, ping_req_received);
  log_str(logger, PSTR(" last ping resp "));
  log_uint64(logger, ping_resp_received);
  log_flush(logger);
  return log_status(logger);
}

