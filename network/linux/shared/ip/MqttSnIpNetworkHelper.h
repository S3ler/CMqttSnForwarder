//
// Created by SomeDude on 05.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
#define CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <platform/device_address.h>
#include <logging/MqttSnLoggingInterface.h>
#include <netdb.h>

struct sockaddr_in get_sockaddr_in_from_tcp_file_descriptor(int file_descriptor);

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress);

device_address get_device_address_from_tcp_file_descriptor(int file_descriptor);

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
