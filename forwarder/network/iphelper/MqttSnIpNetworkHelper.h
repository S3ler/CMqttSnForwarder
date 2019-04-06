//
// Created by SomeDude on 05.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
#define CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H

#include <global_defines.h>
#include <netdb.h>

int get_device_address_from_hostname(const char *hostname, device_address *dst);

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress);

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr);

int get_device_address_from_addrinfo(struct addrinfo *ai_addr, device_address *dst);

int add_port_to_device_address(uint32_t port, device_address *dst);

#endif //CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
