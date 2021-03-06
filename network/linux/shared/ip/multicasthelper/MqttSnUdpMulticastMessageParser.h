//
// Created by SomeDude on 04.05.2019.
//

#include <platform/device_address.h>
#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_MULTICASTHELPER_MQTTSNUDPMULTICASTMESSAGEPARSER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_MULTICASTHELPER_MQTTSNUDPMULTICASTMESSAGEPARSER_H_

#if defined(WITH_LINUX_TCP_NETWORK_BROADCAST) || defined(WITH_LINUX_UDP_NETWORK_BROADCAST)
struct sockaddr_in get_sockaddr_in_from_bind_file_descriptor(int file_descriptor);
int initialize_udp_multicast_socket(int unicast_socket_fd, uint32_t broadcast_ip, uint16_t broadcast_port);
ssize_t send_udp_mutlicast_message(int socket_fd,
                                   const device_address *to,
                                   const uint8_t *data,
                                   uint16_t data_length);
/**
 *
 * @param unicast_socket_fd
 * @param multicast_socket_fd
 * @param timeout_ms
 * @return 0 for nothing, 1 for unicast, 2 for multicast, 3 for both unicast and multicast
 */
int is_multicast_or_unicast_message_receive(int unicast_socket_fd, int multicast_socket_fd, int timeout_ms);

#endif

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_MULTICASTHELPER_MQTTSNUDPMULTICASTMESSAGEPARSER_H_
