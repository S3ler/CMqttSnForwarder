//
// Created by SomeDude on 05.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
#define CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H

#include <platform/device_address.h>
#include <logging/MqttSnLogging.h>
#include <netdb.h>
#ifdef __cplusplus
extern "C" {
#endif

struct sockaddr_in get_sockaddr_in_from_tcp_file_descriptor(int file_descriptor);

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress);

device_address get_device_address_from_tcp_file_descriptor(int file_descriptor);

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr);


int convert_hostname_port_to_device_address(const char *hostname,
                                            int port,
                                            device_address *address,
                                            const char *address_name);

int is_device_address_ipv4_zeroed(const device_address* address);
void set_device_address_ipv4_localhost( device_address *address);

int get_device_address_from_hostname(const char *hostname, device_address *dst);

int get_device_address_from_addrinfo(struct addrinfo *ai_addr, device_address *dst);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
