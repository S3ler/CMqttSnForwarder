//
// Created by SomeDude on 07.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_TCPHELPER_MQTTSNTCPNETWORKHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_TCPHELPER_MQTTSNTCPNETWORKHELPER_H_

#include <stdint.h>

int connect_to_tcp_host(uint32_t ip, uint16_t port);
int is_tcp_message_received(int socket_fd, int32_t timeout_ms);

int listen_on_tcp_socket(uint32_t ip, uint16_t port, uint32_t max_pending_connections);
int is_new_connection_or_message_received(int listen_socket_fd,
                                          int *client_socket_fds,
                                          int client_socket_fds_len,
                                          int32_t timeout_ms,
                                          fd_set *read_fds);
int send
#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_TCPHELPER_MQTTSNTCPNETWORKHELPER_H_
