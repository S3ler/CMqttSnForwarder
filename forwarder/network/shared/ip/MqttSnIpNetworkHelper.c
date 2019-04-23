//
// Created by SomeDude on 05.04.2019.
//

#include "MqttSnIpNetworkHelper.h"
#include <logging/MqttSnForwarderLoggingBasic.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

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

int get_device_address_from_hostname(const char *hostname, device_address *dst) {
  memset(dst, 0, sizeof(device_address));

  struct addrinfo hints = {0};

  struct addrinfo *ainfo, *rp;
  int rc = 0;

  rc = getaddrinfo(hostname, NULL, &hints, &ainfo);
  if (rc) {
    return -1;
  }
  // prefer ip v4 address
  for (rp = ainfo; rp != NULL; rp = rp->ai_next) {
    if (rp->ai_family == AF_INET) {
      if (get_device_address_from_addrinfo(rp, dst)) {
        continue;
      }
      freeaddrinfo(ainfo);
      return 0;
    }
  }

  // no ip v4 address found use a ip v6
  for (rp = ainfo; rp != NULL; rp = rp->ai_next) {
    if (rp->ai_family == AF_INET6) {
      if (get_device_address_from_addrinfo(rp, dst)) {
        continue;
      }
      freeaddrinfo(ainfo);
      return 0;
    }
  }

  freeaddrinfo(ainfo);
  return -1;
}

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress) {
  uint32_t ip = 0;
  uint16_t port = 0;
  get_ipv4_and_port_from_device_address(&ip, &port, deviceAddress);

  sa_family_t family = AF_INET;

  struct sockaddr_in address;
  address.sin_family = family;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = htonl(ip);

  return address;
}

int get_device_address_from_addrinfo(struct addrinfo *ai_addr, device_address *dst) {
  assert(ai_addr->ai_family == AF_INET || ai_addr->ai_family == AF_INET6);
  assert(ai_addr->ai_family == AF_INET ? (sizeof(device_address) >= 4 + 2) : (sizeof(device_address) >= 16 + 2));

  if (ai_addr->ai_family == AF_INET) {
    struct sockaddr_in *sockaddr = (struct sockaddr_in *) ai_addr->ai_addr;
    uint32_t ip_as_number = ntohl(sockaddr->sin_addr.s_addr);
    dst->bytes[0] = (ip_as_number >> 24) & 0xFF;
    dst->bytes[1] = (ip_as_number >> 16) & 0xFF;
    dst->bytes[2] = (ip_as_number >> 8) & 0xFF;
    dst->bytes[3] = (ip_as_number >> 0) & 0xFF;

    add_port_to_device_address(ntohs(sockaddr->sin_port), dst);
  }
  if (ai_addr->ai_family == AF_INET6) {
    struct sockaddr_in6 *sockaddr = (struct sockaddr_in6 *) ai_addr->ai_addr;
    int ai_family_size = sizeof(sockaddr->sin6_addr.__in6_u.__u6_addr8);
    memcpy(dst->bytes, sockaddr->sin6_addr.__in6_u.__u6_addr8, ai_family_size);
    add_port_to_device_address(ntohs(sockaddr->sin6_port), dst);
  }
  return 0;
}

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr) {

  device_address result = {0};
  uint32_t ip_as_number = ntohl(sockaddr->sin_addr.s_addr);
  result.bytes[0] = (ip_as_number >> 24) & 0xFF;
  result.bytes[1] = (ip_as_number >> 16) & 0xFF;
  result.bytes[2] = (ip_as_number >> 8) & 0xFF;
  result.bytes[3] = (ip_as_number >> 0) & 0xFF;

  uint16_t port_as_number = (uint16_t) ntohs(sockaddr->sin_port);
  result.bytes[4] = (port_as_number >> 8) & 0xFF;
  result.bytes[5] = (port_as_number >> 0) & 0xFF;

  return result;
}

int add_port_to_device_address(uint32_t port, device_address *dst) {
  dst->bytes[sizeof(device_address) - 2] = (port >> 8) & 0xFF;
  dst->bytes[sizeof(device_address) - 1] = (port >> 0) & 0xFF;
  return 0;
}

uint32_t get_port_from_device_address(const device_address *src) {
  return (((uint32_t) src->bytes[sizeof(device_address) - 2]) << 8)
      + (((uint32_t) src->bytes[sizeof(device_address) - 1]) << 0);
}

device_address get_device_address_from_file_descriptor(int file_descriptor) {
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  getpeername(file_descriptor, (struct sockaddr *) &address, &addrlen);
  return get_device_address_from_sockaddr_in(&address);
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


