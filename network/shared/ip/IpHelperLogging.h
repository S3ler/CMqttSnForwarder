//
// Created by SomeDude on 11.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_IP_IPHELPERLOGGING_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_IP_IPHELPERLOGGING_H_

#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

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

int log_client_unknown_destination(const MqttSnLogger *logger,
                                   const device_address *from,
                                   const device_address *to,
                                   const uint8_t *data,
                                   uint16_t data_len);

int log_gateway_unknown_destination(const MqttSnLogger *logger,
                                    const device_address *from,
                                    const device_address *to,
                                    const uint8_t *data,
                                    uint16_t data_len);

int log_gateway_close_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_gateway_lost_connection(const MqttSnLogger *logger, const char *protocol, const device_address *address);

int log_failed_convert_device_address_to_ipv4_and_port(const MqttSnLogger *logger, const device_address *from);

int log_failed_convert_device_address_to(const MqttSnLogger *logger, const device_address *from, const char *to);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_IP_IPHELPERLOGGING_H_
