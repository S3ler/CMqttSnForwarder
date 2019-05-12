//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_ARDUINOIPADDRESSHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_ARDUINOIPADDRESSHELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/global_defines.h>
#include <IPAddress.h>

void arduino_ipv4_and_port_to_device_address(IPAddress *src, uint16_t src_port, device_address *dst);
void arduino_device_address_to_IPAddress_and_port(device_address *src, IPAddress *dst, uint16_t *dst_port);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_ARDUINOIPADDRESSHELPER_H_
