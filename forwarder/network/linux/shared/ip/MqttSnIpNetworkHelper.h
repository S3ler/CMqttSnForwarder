//
// Created by SomeDude on 05.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
#define CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H

#include <forwarder/global_defines.h>
#include <forwarder/MqttSnForwarderLogging.h>
#include <netdb.h>



struct sockaddr_in get_sockaddr_in_from_tcp_file_descriptor(int file_descriptor);

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress);

device_address get_device_address_from_tcp_file_descriptor(int file_descriptor);

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr);


#ifdef WITH_LOGGING

int log_select_error(const MqttSnLogger *logger);

#endif //WITH_LOGGING

#endif //CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
