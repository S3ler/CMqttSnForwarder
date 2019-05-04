//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_

#include <stdint.h>
#include <forwarder/global_defines.h>
#include <forwarder/MqttSnForwarderLogging.h>

int add_port_to_device_address(uint32_t port, device_address *dst);

uint32_t get_port_from_device_address(const device_address *src);

int get_ipv4_and_port_from_device_address(uint32_t *dst_ip, uint16_t *dst_port, const device_address *address);

int convert_string_to_device_address(const char *string, device_address *address);



#ifdef WITH_LOGGING

int log_open_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_close_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_new_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_gateway_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_gateway_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

#endif //WITH_LOGGING

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_
