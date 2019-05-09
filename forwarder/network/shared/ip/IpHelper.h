//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <forwarder/global_defines.h>
#include <forwarder/MqttSnForwarderLogging.h>

int add_port_to_device_address(uint32_t port, device_address *dst);

uint32_t get_port_from_device_address(const device_address *src);

int get_ipv4_and_port_from_device_address(uint32_t *dst_ip, uint16_t *dst_port, const device_address *src);

int convert_string_to_device_address(const char *string, device_address *address);

int convert_string_ip_port_to_device_address(const MqttSnLogger *logger,
                                             const char *ip_str,
                                             int port,
                                             device_address *address,
                                             const char *address_name);

#ifdef WITH_LOGGING
int print_cannot_convert_ip_str_to_network_address(const MqttSnLogger *logger,
                                                   const char *ip_str,
                                                   const char *address_name);

int print_invalid_port_given(const MqttSnLogger *logger, int32_t port);

int log_opening_unicast_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_opening_multicast_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_close_unicast_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_close_multicast_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_failed_opening_unicast_socket(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_failed_opening_multicast_socket(const MqttSnLogger *logger,
                                        const char *protocol,
                                        const device_address *address);

int log_unicast_socket_failed(const MqttSnLogger *logger, const char *protocol, const device_address *address);
int log_multicast_socket_failed(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_opening_socket(const MqttSnLogger *logger,
                       const char *cast,
                       const char *protocol,
                       const device_address *address);

int log_failed_opening_socket(const MqttSnLogger *logger,
                              const char *cast,
                              const char *protocol,
                              const device_address *address);
int log_socket_failed(const MqttSnLogger *logger,
                      const char *cast,
                      const char *protocol,
                      const device_address *address);

int log_close_socket(const MqttSnLogger *logger, const char *cast, const char *protocol, const device_address *address);

int log_new_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_closed_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_client_disconnected(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_gateway_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_gateway_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_failed_convert_device_address_to_ipv4_and_port(const MqttSnLogger *logger, const device_address *from);

int log_failed_convert_device_address_to(const MqttSnLogger *logger, const device_address *from, const char *to);

#endif //WITH_LOGGING

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_SHARED_IPHELPER_H_
