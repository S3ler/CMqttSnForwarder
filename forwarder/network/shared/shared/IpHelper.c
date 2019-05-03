//
// Created by SomeDude on 29.04.2019.
//

#include "IpHelper.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <forwarder/logging/MqttSnForwarderLoggingBasic.h>

int add_port_to_device_address(uint32_t port, device_address *dst) { // TODO to uint16_t
  dst->bytes[sizeof(device_address) - 2] = (port >> 8) & 0xFF;
  dst->bytes[sizeof(device_address) - 1] = (port >> 0) & 0xFF;
  return 0;
}

uint32_t get_port_from_device_address(const device_address *src) { // TODO to uint16_t
  return (((uint32_t) src->bytes[sizeof(device_address) - 2]) << 8)
      + (((uint32_t) src->bytes[sizeof(device_address) - 1]) << 0);
}

int get_ipv4_and_port_from_device_address(uint32_t *dst_ip, uint16_t *dst_port, const device_address *address) {
  *dst_ip = (((uint32_t) address->bytes[0]) << 24)
      + (((uint32_t) address->bytes[1]) << 16)
      + (((uint32_t) address->bytes[2]) << 8)
      + (((uint32_t) address->bytes[3]) << 0);

  *dst_port = ((uint16_t) address->bytes[4] << 8)
      + ((uint16_t) address->bytes[5]);
  return 0;
}

int convert_string_to_device_address(const char *string, device_address *address) {
  char *cp_string = strdup(string);
  char *token = strtok(cp_string, ".");
  size_t i = 0;
  int rc = 0;
  while (token != NULL) {
    char *end_prt;
    long int n = strtol(token, &end_prt, 10);
    if (errno == EOVERFLOW) {
      rc = -1;
      break;
    }
    if (*end_prt != '\0') {
      // no conversion performed
      rc = -1;
      break;
    }
    if (n > UINT8_MAX || n < 0) {
      rc = -1;
      break;
    }
    // address->bytes[i++] = atoi(token);
    if (i + 1 > sizeof(device_address)) {
      // given string address is too long
      rc = -1;
      break;
    }
    address->bytes[i++] = n;
    token = strtok(NULL, ".");
  }

  free(cp_string);
  return rc;
}

#ifdef WITH_LOGGING
int log_open_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *opening = "Opening ";
  const char *listen_socket = " socket ";
  const char *on_port = " on port ";
  const char *dot = ".";
  uint32_t port = get_port_from_device_address(address);

  log_msg_start(logger);
  log_str(logger, opening);
  log_str(logger, protocol);
  log_str(logger, listen_socket);
  log_device_address(logger, address);
  log_str(logger, on_port);
  log_uint32(logger, port);
  log_str(logger, dot);
  log_flush(logger);
  return log_status(logger);
}

int log_close_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *close = "Close ";
  const char *listen_socket = " socket ";
  const char *on_port = " on port ";
  const char *dot = ".";
  uint32_t port = get_port_from_device_address(address);

  log_msg_start(logger);
  log_str(logger, close);
  log_str(logger, protocol);
  log_str(logger, listen_socket);
  log_device_address(logger, address);
  log_str(logger, on_port);
  log_uint32(logger, port);
  log_str(logger, dot);
  log_flush(logger);
  return log_status(logger);
}

int log_new_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *client_str = "Client ";
  const char *space = " ";
  const char *connected_dot = " connected.";

  log_msg_start(logger);
  log_str(logger, client_str);
  log_str(logger, protocol);
  log_str(logger, space);
  log_device_address(logger, address);
  log_str(logger, connected_dot);
  log_flush(logger);
  return log_status(logger);
}

int log_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *client_str = "Client ";
  const char *space = " ";
  const char *disconnected_dot = " disconnected.";

  log_msg_start(logger);
  log_str(logger, client_str);
  log_str(logger, protocol);
  log_str(logger, space);
  log_device_address(logger, address);
  log_str(logger, disconnected_dot);
  log_flush(logger);
  return log_status(logger);
}

int log_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *client_str = "Client ";
  const char *space = " ";
  const char *disconnected_dot = " lost.";

  log_msg_start(logger);
  log_str(logger, client_str);
  log_str(logger, protocol);
  log_str(logger, space);
  log_device_address(logger, address);
  log_str(logger, disconnected_dot);
  log_flush(logger);
  return log_status(logger);
}

int log_gateway_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *client_str = "MQTT-SN Gateway ";
  const char *space = " ";
  const char *disconnected_dot = " disconnected.";

  log_msg_start(logger);
  log_str(logger, client_str);
  log_str(logger, protocol);
  log_str(logger, space);
  log_device_address(logger, address);
  log_str(logger, disconnected_dot);
  log_flush(logger);
  return log_status(logger);
}

int log_gateway_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *client_str = "MQTT-SN Gateway ";
  const char *space = " ";
  const char *disconnected_dot = " lost.";

  log_msg_start(logger);
  log_str(logger, client_str);
  log_str(logger, protocol);
  log_str(logger, space);
  log_device_address(logger, address);
  log_str(logger, disconnected_dot);
  log_flush(logger);
  return log_status(logger);
}

#endif //WITH_LOGGING
