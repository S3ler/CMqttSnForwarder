//
// Created by SomeDude on 05.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
#define CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H

#include <forwarder/global_defines.h>
#include <forwarder/MqttSnForwarderLogging.h>
#include <netdb.h>


int get_device_address_from_hostname(const char *hostname, device_address *dst);

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress);

device_address get_device_address_from_file_descriptor(int file_descriptor);

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr);

int get_device_address_from_addrinfo(struct addrinfo *ai_addr, device_address *dst);



#endif //CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
