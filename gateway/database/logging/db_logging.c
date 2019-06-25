//
// Created by SomeDude on 19.06.2019.
//

#include <platform/platform_compatibility.h>
#include "db_logging.h"

int32_t log_start_client_transaction(const MqttSnLogger *logger,
                                     DB_HANDLER_RESULT db_handler_result,
                                     const device_address *address,
                                     DB_ENTRY_MQTT_SN_CLIENT_RESULT client_result,
                                     const char *client_id,
                                     const char *file_number) {
  // TODO log: client transaction started - data - client result - db handler result
  log_msg_start(logger);
  log_str(logger, PSTR("client transaction started "));
  log_flush(logger);
  return log_status(logger);
}
int32_t log_database_error(const MqttSnLogger *logger, const char *message) {
  log_msg_start(logger);
  log_str(logger, PSTR("database error: "));
  if (message) {
    log_str(logger, message);
  }
  log_flush(logger);
  return log_status(logger);
}
int32_t log_apply_client_transaction(const MqttSnLogger *logger,
                                     DB_HANDLER_RESULT db_handler_result,
                                     DB_ENTRY_MQTT_SN_CLIENT_RESULT client_result,
                                     bool database_error) {
  log_msg_start(logger);
  log_str(logger, PSTR("apply transaction - "));
  if (client_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
    log_str(logger, PSTR("success"));
  }else{
    log_str(logger, PSTR("error"));
    // TODO other cases
  }
  log_flush(logger);
  return log_status(logger);
}
