//
// Created by SomeDude on 09.04.2019.
//

#include "MqttSnForwarderLogging.h"
#include <logging/linux/stdout/StdoutLogging.h>
#include "MqttSnMessageParser.h"
#include "logging/MqttSnForwarderLoggingBasic.h"
#include "logging/MqttSnForwarderLoggingMessages.h"

int MqttSnLoggerInit(MqttSnLogger *logger, log_level_t log_level) {
  logger->log_init = stdout_log_init;
  logger->log_deinit = stdout_log_deinit;
  logger->log_flush = stdout_log_flush;
  logger->log_str = stdout_log_str;
  logger->log_char = stdout_log_char;
  logger->log_level = log_level;
  logger->status = -1;
  if (logger->log_init(logger) == 0) {
    logger->status = 0;
  }
  return logger->status;
}

void MqttSnLoggerDeinit(MqttSnLogger *logger) {
  logger->log_deinit(logger);
  logger->status = -1;
}

int log_status(const MqttSnLogger *logger) {
  return logger->status;
}

int is_logger_not_available(const MqttSnLogger *logger) {
  if (logger->status < 0) {
    return 1;
  }
  return 0;
}

int shall_not_be_logged(const MqttSnLogger *logger, int max_level) {
  if (logger->log_level <= LOG_LEVEL_QUIET) {
    return 1;
  }
  if (logger->log_level < max_level) {
    return 1;
  }
  return 0;
}

int log_flush(const MqttSnLogger *logger) {
  if (logger->status < 0) {
    return -1;
  }
  return logger->log_flush(logger);
}

int log_char(const MqttSnLogger *logger, char c) {
  if (logger->status <= 0) {
    return -1;
  }
  return logger->log_char(c);
}

int log_str(const MqttSnLogger *logger, const char *str) {
  if (logger->status < 0) {
    return -1;
  }
  return logger->log_str(str);
}

int log_forwarder_started(const MqttSnLogger *logger,
                          int level,
                          const char *version,
                          int major,
                          int minor,
                          int tweak,
                          const char *build_date) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *is = " is ";
  const char *dot = ".";
  const char *build_date_str = " (build date ";
  const char *action = ") started.";

  return (log_current_time(logger) ||
      log_str(logger, name_version) ||
      log_str(logger, version) ||
      log_str(logger, is) ||
      log_uint32(logger, major) ||
      log_str(logger, dot) ||
      log_uint32(logger, minor) ||
      log_str(logger, dot) ||
      log_uintmax(logger, tweak) ||
      log_str(logger, build_date_str) ||
      log_str(logger, build_date) ||
      log_str(logger, action) ||
      log_flush(logger) != 0);
}

int log_forwarder_terminated(const MqttSnLogger *logger,
                             int level,
                             const char *version,
                             uint32_t major,
                             uint32_t minor,
                             uintmax_t tweak) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *is = " is ";
  const char *dot = ".";
  const char *action = " terminated.";

  return (log_current_time(logger) ||
      log_str(logger, name_version) ||
      log_str(logger, version) ||
      log_str(logger, is) ||
      log_uint32(logger, major) ||
      log_str(logger, dot) ||
      log_uint32(logger, minor) ||
      log_str(logger, dot) ||
      log_uintmax(logger, tweak) ||
      log_str(logger, action) ||
      log_flush(logger) != 0);
}

int log_network_connect(const MqttSnLogger *logger,
                        int level,
                        const char *protocol,
                        const char *network_name,
                        const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *connect = ": Connect ";
  const char *space = " ";
  const char *network_as = " network connect as  ";
  const char *dot = ".";

  return (log_current_time(logger) ||
      log_str(logger, connect) ||
      log_str(logger, protocol) ||
      log_str(logger, space) ||
      log_str(logger, network_name) ||
      log_str(logger, network_as) ||
      log_device_address(logger, address) ||
      log_str(logger, dot) ||
      log_flush(logger) != 0);
}

int log_network_connect_fail(const MqttSnLogger *logger,
                             const char *network_name,
                             const device_address *as,
                             const device_address *to) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  const char *connect = " Connect to ";
  const char *network_as = " network as ";
  const char *failed_dot = " failed.";
  if (log_msg_start(logger)) {
    return -1;
  }
  if (log_str(logger, connect)) {
    return -1;
  }
  if (log_str(logger, network_name)) {
    return -1;
  }
  if (log_str(logger, network_as)) {
    return -1;
  }
  if (log_device_address(logger, as)) {
    return -1;
  }
  if (to != NULL) {
    const char *to_str = " to ";
    if (log_str(logger, to_str)) {
      return -1;
    }
    if (log_device_address(logger, to)) {
      return -1;
    }
  }
  if (log_str(logger, failed_dot)) {
    return -1;
  }
  return 0;
}

int log_network_disconnect(const MqttSnLogger *logger,
                           int level,
                           const char *protocol,
                           const char *network_name,
                           const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *connect = ": Disconnect ";
  const char *space = " ";
  const char *network_as = " network connect as  ";
  const char *dot = ".";

  return (log_current_time(logger) ||
      log_str(logger, connect) ||
      log_str(logger, protocol) ||
      log_str(logger, space) ||
      log_str(logger, network_name) ||
      log_str(logger, network_as) ||
      log_device_address(logger, address) ||
      log_str(logger, dot) ||
      log_flush(logger) != 0);
}

int log_protocol_mismatch(const MqttSnLogger *logger, int level, const char *expected, const char *actual) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *mismatch = ": plugin protocol mismatch - expected: ";
  const char *actual_str = " actual: ";
  const char *dot = ".";

  if (log_current_time(logger)) {
    return -1;
  }
  if (log_str(logger, mismatch)) {
    return -1;
  }
  if (log_str(logger, expected)) {
    return -1;
  }
  if (log_str(logger, actual_str)) {
    return -1;
  }
  if (log_str(logger, actual)) {
    return -1;
  }
  if (log_str(logger, dot)) {
    return -1;
  }
  if (log_flush(logger)) {
    return -1;
  }

  return 0;
}

int log_too_long_message(const MqttSnLogger *logger,
                         int level,
                         const device_address *address,
                         const uint8_t *data,
                         uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *description = "dropping too long message: ";
  if (log_gateway_mqtt_sn_message(logger, level, address, data, data_len, description)) {
    return -1;
  }
  return 0;
}

#ifdef WITH_DEBUG_LOGGING
int log_db_rec_client_message(const MqttSnLogger *logger,
                              int level,
                              const device_address *from,
                              const device_address *to,
                              const uint8_t *data,
                              uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *rec_client_msg_from = "received client message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_braked_open = ", data( ";
  const char *double_braked_close_dot = ")).";

  return (log_msg_start(logger) ||
      log_str(logger, rec_client_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      log_device_address(logger, to) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_braked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_braked_close_dot) ||
      log_flush(logger) != 0);
}

int log_db_rec_gateway_message(const MqttSnLogger *logger,
                               int level,
                               const device_address *from,
                               const uint8_t *data,
                               uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *send_g_msg_from = ": receive gateway message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_bracked_open = ", data( ";
  const char *double_bracked_close_dot = ")).";
  //device_address* std; // TODO get dst

  return (log_current_time(logger) ||
      log_str(logger, send_g_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      //log_device_address(logger, dst) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_bracked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_bracked_close_dot) ||
      log_flush(logger) != 0);
}

int log_db_send_client_message(const MqttSnLogger *logger,
                               int level,
                               const device_address *from,
                               const device_address *dst,
                               const uint8_t *data,
                               uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *send_c_msg_from = ": send client message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_bracked_open = ", data( ";
  const char *double_bracked_close_dot = ")).";

  return (log_current_time(logger) ||
      log_str(logger, send_c_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      log_device_address(logger, dst) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_bracked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_bracked_close_dot) ||
      log_flush(logger) != 0);
}

int log_db_send_gateway_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *from,
                                const device_address *dst,
                                const uint8_t *data,
                                uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *send_g_msg_from = ": send gateway message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_bracked_open = ", data( ";
  const char *double_bracked_close_dot = ")).";

  return (log_current_time(logger) ||
      log_str(logger, send_g_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      log_device_address(logger, dst) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_bracked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_bracked_close_dot) ||
      log_flush(logger) != 0);
}
int log_incomplete_message(const MqttSnLogger *logger,
                           int level,
                           const device_address *address,
                           const uint8_t *data,
                           uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *description = "could not send message completely - try again later: ";
  if (log_gateway_mqtt_sn_message(logger, level, address, data, data_len, description)) {
    return -1;
  }
  return 0;
}
#endif
