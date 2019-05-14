//
// Created by SomeDude on 07.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_TCPHELPER_MQTTSNTCPNETWORKHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_TCPHELPER_MQTTSNTCPNETWORKHELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int connect_to_tcp_host(uint32_t ip, uint16_t port);
int is_tcp_message_received(int socket_fd, int32_t timeout_ms);

int listen_on_tcp_socket(uint32_t ip, uint16_t port, uint32_t max_pending_connections);

// TODO return: error -1,
//  nothing received 0,
//  listen only 1,
//  client only 2,
//  multicast only 3,
//  listen and client only 4,
//  listen and multicast only 5,
//  client and multicast only 6,
//  listen, client and multicast 7
int new_connection_or_is_message_received(int listen_socket_fd,
                                          const int client_socket_fds[],
                                          int client_socket_fds_len,
                                          int32_t timeout_ms,
                                          fd_set *read_fds,
                                          int multicast_socket_fd);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_TCPHELPER_MQTTSNTCPNETWORKHELPER_H_
