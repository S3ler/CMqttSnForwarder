//
// Created by SomeDude on 05.04.2019.
//

#include "MqttSnIpNetworkHelper.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <forwarder/logging/MqttSnForwarderLoggingBasic.h>
#include <forwarder/network/shared/ip/IpHelper.h>
#include <errno.h>

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress) {
  uint32_t ip = 0;
  uint16_t port = 0;
  get_ipv4_and_port_from_device_address(&ip, &port, deviceAddress);

  sa_family_t family = AF_INET;

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = family;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = htonl(ip);

  return address;
}

struct sockaddr_in get_sockaddr_in_from_tcp_file_descriptor(int file_descriptor) {
  struct sockaddr_in address = {0};
  socklen_t addrlen = sizeof(address);
  getpeername(file_descriptor, (struct sockaddr *) &address, &addrlen);
  return address;
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

device_address get_device_address_from_tcp_file_descriptor(int file_descriptor) {
  struct sockaddr_in address = get_sockaddr_in_from_tcp_file_descriptor(file_descriptor);
  return get_device_address_from_sockaddr_in(&address);
}

#ifdef WITH_LOGGING

int log_select_error(const MqttSnLogger *logger) {
  const char *select_error_str = "select error - ";
  log_msg_start(logger);
  log_str(logger, select_error_str);
  log_str(logger, strerror(errno));
  log_flush(logger);
  return log_status(logger);
}

#endif //WITH_LOGGING