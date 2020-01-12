//
// Created by SomeDude on 19.04.2019.
//

#ifndef CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_PLUGIN_DEVICE_ADDRESS_CONVERTER_H_
#define CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_PLUGIN_DEVICE_ADDRESS_CONVERTER_H_

#include <stdint.h>

int get_ipv4_string_and_port_from_device_address(char *dst,
                                                 uint16_t dst_len,
                                                 uint16_t *dst_port,
                                                 const uint8_t *device_address,
                                                 uint16_t device_address_len);

int get_port_from_device_address(uint16_t *dst_port, const uint8_t *device_address, uint16_t device_address_len);

#endif //CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_PLUGIN_DEVICE_ADDRESS_CONVERTER_H_
