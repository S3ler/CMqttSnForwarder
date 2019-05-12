//
// Created by SomeDude on 29.04.2019.
//


#include "ArduinoIpAddressHelper.h"
#include <IPAddress.h>
#include <network/shared/ip/IpHelper.h>

void arduino_ipv4_and_port_to_device_address(IPAddress *src, uint16_t src_port, device_address *dst) {
  // IPAddress 0 - 3 bytes
  dst->bytes[0] = (*src)[0];
  dst->bytes[1] = (*src)[1];
  dst->bytes[2] = (*src)[2];
  dst->bytes[3] = (*src)[3];

  add_port_to_device_address(src_port, dst);
}

void arduino_device_address_to_IPAddress_and_port(device_address *src, IPAddress *dst, uint16_t *dst_port) {
  // IPAddress 0 - 3 bytes
  (*dst)[0] = src->bytes[0];
  (*dst)[1] = src->bytes[1];
  (*dst)[2] = src->bytes[2];
  (*dst)[3] = src->bytes[3];

  *dst_port = get_port_from_device_address(src);
}
