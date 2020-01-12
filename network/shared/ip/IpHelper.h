//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <platform/device_address.h>
#include <logging/MqttSnLogging.h>

int add_port_to_device_address(uint32_t port, device_address *dst);

uint32_t get_port_from_device_address(const device_address *src);

int get_ipv4_and_port_from_device_address(uint32_t *dst_ip, uint16_t *dst_port, const device_address *src);

int convert_string_to_device_address(const char *string, device_address *address);

int convert_string_ip_port_to_device_address(const MqttSnLogger *logger,
                                             const char *ip_str,
                                             int port,
                                             device_address *address,
                                             const char *address_name);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_
