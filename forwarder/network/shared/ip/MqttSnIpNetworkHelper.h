//
// Created by SomeDude on 05.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
#define CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H

#include <forwarder/global_defines.h>
#include <forwarder/MqttSnForwarderLogging.h>
#include <netdb.h>

#ifdef WITH_LOGGING

int log_open_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_close_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_new_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_gateway_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_gateway_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

#endif //WITH_LOGGING

int get_ipv4_and_port_from_device_address(uint32_t *dst_ip, uint16_t *dst_port, const device_address *address);

int get_device_address_from_hostname(const char *hostname, device_address *dst);

struct sockaddr_in get_sockaddr_in_from_device_address(const device_address *deviceAddress);

device_address get_device_address_from_file_descriptor(int file_descriptor);

device_address get_device_address_from_sockaddr_in(struct sockaddr_in *sockaddr);

int get_device_address_from_addrinfo(struct addrinfo *ai_addr, device_address *dst);

int add_port_to_device_address(uint32_t port, device_address *dst);

uint32_t get_port_from_device_address(const device_address *src);

#endif //CMQTTSNFORWARDER_MQTTSNIPNETWORKHELPER_H
