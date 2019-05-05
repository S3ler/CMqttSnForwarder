//
// Created by SomeDude on 05.04.2019.
//

#include "MqttSnIpNetworkHelper.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <forwarder/logging/MqttSnForwarderLoggingBasic.h>
#include <forwarder/network/linux/shared/shared/IpHelper.h>
#include <errno.h>

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
  memset(&address, 0, sizeof(address));
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
struct sockaddr_in get_sockaddr_in_from_file_descriptor(int file_descriptor) {
  struct sockaddr_in address = {0};
  socklen_t addrlen = sizeof(address);
  int rc = getpeername(file_descriptor, (struct sockaddr *) &address, &addrlen);
  assert(rc != -1); // TODO handle
  return address;
}

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr) {

  device_address result = {0};
  uint32_t ip_as_number = ntohl(sockaddr->sin_addr.s_addr);
  result.bytes[1] = (ip_as_number >> 16) & 0xFF;
  result.bytes[2] = (ip_as_number >> 8) & 0xFF;
  result.bytes[3] = (ip_as_number >> 0) & 0xFF;

  uint16_t port_as_number = (uint16_t) ntohs(sockaddr->sin_port);
  result.bytes[4] = (port_as_number >> 8) & 0xFF;
  result.bytes[5] = (port_as_number >> 0) & 0xFF;

  return result;
}

device_address get_device_address_from_file_descriptor(int file_descriptor) {
  struct sockaddr_in address = get_sockaddr_in_from_file_descriptor(file_descriptor);
  return get_device_address_from_sockaddr_in(&address);
}
