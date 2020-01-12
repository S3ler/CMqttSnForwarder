//
// Created by SomeDude on 19.04.2019.
//

#include "plugin_device_address_converter.h"
#include <stdio.h>
#include <string.h>
int get_ipv4_string_and_port_from_device_address(char *dst,
                                                 uint16_t dst_len,
                                                 uint16_t *dst_port,
                                                 const uint8_t *device_address,
                                                 uint16_t device_address_len) {
  if (device_address_len < 6) {
    return -1;
  }
  if (dst_len < strlen("255.255.255.255") + 1) {
    return -1;
  }
  snprintf(dst, dst_len, "%d.%d.%d.%d",
           device_address[0],
           device_address[1],
           device_address[2],
           device_address[3]);

  return get_port_from_device_address(dst_port, device_address, device_address_len);
}

int get_port_from_device_address(uint16_t *dst_port, const uint8_t *device_address, uint16_t device_address_len) {
  if (device_address_len < 2) {
    return -1;
  }
  *dst_port = ((uint16_t) device_address[device_address_len - 2] << 8)
      + ((uint16_t) device_address[device_address_len - 1] << 0);
  return 0;
}
