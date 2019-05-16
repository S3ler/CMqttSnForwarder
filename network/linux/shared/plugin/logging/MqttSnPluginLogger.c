//
// Created by SomeDude on 10.04.2019.
//

#include "MqttSnPluginLogger.h"
#include <parser/logging/MqttSnForwarderLoggingMessages.h>

// client log messages
int log_client_network_connect(const MqttSnLogger *logger,
                               const char *protocol_name,
                               const device_address *as,
                               const device_address *to) {
  return log_network_connect(logger, protocol_name, "client", as, to);
}
int log_client_network_connect_fail(const MqttSnLogger *logger,
                                    const char *protocol_name,
                                    const device_address *as,
                                    const device_address *to) {
  return log_network_connect_result(logger, protocol_name, "client", "failed", as, to);
}
int log_client_network_disconnect(const MqttSnLogger *logger,
                                  const char *protocol_name,
                                  const device_address *address) {
  return log_network_disconnect(logger, protocol_name, "client", address);
}
int log_client_network_deinitialized(const MqttSnLogger *logger,
                                     const char *protocol_name,
                                     const device_address *address) {
  return log_network_deinitialized(logger, protocol_name, "client", address);
}


// gateway log messages
int log_gateway_network_connect(const MqttSnLogger *logger,
                                const char *protocol_name,
                                const device_address *as,
                                const device_address *to) {
  return log_network_connect(logger, protocol_name, "gateway", as, to);
}
int log_gateway_network_connect_fail(const MqttSnLogger *logger,
                                     const char *protocol_name,
                                     const device_address *as,
                                     const device_address *to) {
  return log_network_connect_result(logger, protocol_name, "gateway", "failed", as, to);
}
int log_gateway_network_disconnect(const MqttSnLogger *logger,
                                   const char *protocol_name,
                                   const device_address *address) {
  return log_network_disconnect(logger, protocol_name, "gateway", address);
}
int log_gateway_network_deinitialized(const MqttSnLogger *logger,
                                      const char *protocol_name,
                                      const device_address *address) {
  return log_network_deinitialized(logger, protocol_name, "gateway", address);
}

// shared log messages
int log_dlerror(const MqttSnLogger *logger, const char *error) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  log_msg_start(logger);
  log_str(logger, error);
  log_flush(logger);
  return log_status(logger);
}

int log_max_msg_length_mismatch(const MqttSnLogger *logger, int level, uint16_t gw_max, uint16_t plugin_max) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  const char *mismatch = "maximum message length mismatch - forwarder: ";
  const char *plugin_max_str = " plugin: ";
  const char *dot = ".";

  log_msg_start(logger);
  log_str(logger, mismatch);
  log_uint16(logger, gw_max);
  log_str(logger, plugin_max_str);
  log_uint16(logger, plugin_max);
  log_str(logger, dot);
  log_flush(logger);
  return log_status(logger);
}

int log_network_connect(const MqttSnLogger *logger,
                        const char *protocol_name,
                        const char *network_name,
                        const device_address *as,
                        const device_address *to) {
  return log_network_connect_result(logger, protocol_name, network_name, "success", as, to);
}

int log_network_connect_fail(const MqttSnLogger *logger,
                             const char *protocol_name,
                             const char *network_name,
                             const device_address *as,
                             const device_address *to) {
  return log_network_connect_result(logger, protocol_name, network_name, "failed", as, to);
}
int log_network_connect_result(const MqttSnLogger *logger,
                               const char *protocol_name,
                               const char *network_name,
                               const char *result,
                               const device_address *as,
                               const device_address *to) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  const char *connect = "Connect to ";
  const char *space = " ";
  const char *network_as = " network as ";

  log_msg_start(logger);
  log_str(logger, connect);

  if (protocol_name) {
    log_str(logger, protocol_name);
    log_str(logger, space);
  }

  log_str(logger, network_name);
  log_str(logger, network_as);
  log_device_address(logger, as);
  if (to) {
    const char *to_str = " to ";
    log_str(logger, to_str);
    log_device_address(logger, to);
  }

  log_str(logger, space);
  log_str(logger, result);
  log_flush(logger);
  return log_status(logger);
}
int log_network_disconnect(const MqttSnLogger *logger,
                           const char *protocol_name,
                           const char *network_name,
                           const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *connect = "Disconnect ";
  const char *space = " ";
  const char *network_as = " network connect as ";
  const char *dot = ".";

  log_msg_start(logger);
  log_str(logger, connect);
  log_str(logger, protocol_name);
  log_str(logger, space);
  log_str(logger, network_name);
  log_str(logger, network_as);
  log_device_address(logger, address);
  log_str(logger, dot);

  log_flush(logger);
  return log_status(logger);
}

int log_network_deinitialized(const MqttSnLogger *logger,
                              const char *protocol_name,
                              const char *network_name,
                              const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *connect = "Disconnect ";
  const char *space = " ";
  const char *network_as = " network connect as ";
  const char *dot = ".";

  log_msg_start(logger);
  log_str(logger, connect);
  log_str(logger, protocol_name);
  log_str(logger, space);
  log_str(logger, network_name);
  log_str(logger, network_as);
  log_device_address(logger, address);
  log_str(logger, dot);

  log_flush(logger);
  return log_status(logger);
}
int log_protocol_mismatch(const MqttSnLogger *logger, const char *expected, const char *actual) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *mismatch = "plugin protocol mismatch - expected: ";
  const char *actual_str = " actual: ";
  const char *dot = ".";

  log_msg_start(logger);
  log_str(logger, mismatch);
  log_str(logger, expected);
  log_str(logger, actual_str);
  log_str(logger, actual);
  log_str(logger, dot);

  log_flush(logger);
  return log_status(logger);
}
int log_too_long_message(const MqttSnLogger *logger,
                         const device_address *from,
                         MQTT_SN_FORWARDER_NETWORK network,
                         const uint8_t *data,
                         uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *description = "dropping too long message: ";
  return log_mqtt_sn_message(logger, from, network, data, data_len, description);
}
