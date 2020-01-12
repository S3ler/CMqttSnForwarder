//
// Created by SomeDude on 29.04.2019.
//

#include "IpHelper.h"
#include "IpHelperLogging.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

// TODO to uint16_t port
// TODO  ntohs
int add_port_to_device_address(uint32_t port, device_address *dst) {
  dst->bytes[sizeof(device_address) - 2] = (port >> 8) & 0xFF;
  dst->bytes[sizeof(device_address) - 1] = (port >> 0) & 0xFF;
  return 0;
}

// TODO to uint16_t port
// TODO  ntohs
uint32_t get_port_from_device_address(const device_address *src) {
  return (((uint32_t) src->bytes[sizeof(device_address) - 2]) << 8)
      + (((uint32_t) src->bytes[sizeof(device_address) - 1]) << 0);
}

int get_ipv4_and_port_from_device_address(uint32_t *dst_ip, uint16_t *dst_port, const device_address *src) {
  *dst_ip = (((uint32_t) src->bytes[0]) << 24)
      + (((uint32_t) src->bytes[1]) << 16)
      + (((uint32_t) src->bytes[2]) << 8)
      + (((uint32_t) src->bytes[3]) << 0);

  *dst_port = get_port_from_device_address(src);
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

int convert_string_ip_port_to_device_address(const MqttSnLogger *logger,
                                             const char *ip_str,
                                             int port,
                                             device_address *address,
                                             const char *address_name) {
  if (ip_str == NULL) {
    memset(address, 0, sizeof(device_address));
  } else {
    if (convert_string_to_device_address(ip_str, address)) {
#ifdef WITH_LOGGING
      print_cannot_convert_ip_str_to_network_address(logger, ip_str, address_name);
#endif
      return EXIT_FAILURE;
    }
  }

  if (port < -1 || port > 65535) {
#ifdef WITH_LOGGING
    print_invalid_port_given(logger, port);
#endif
    return EXIT_FAILURE;
  }
  if (port > -1) {
    add_port_to_device_address(port, address);
  }
  return EXIT_SUCCESS;
}


