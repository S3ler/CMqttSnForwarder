//
// Created by SomeDude on 04.05.2019.
//

#include <forwarder/global_defines.h>
#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_MULTICASTHELPER_MQTTSNUDPMULTICASTMESSAGEPARSER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_MULTICASTHELPER_MQTTSNUDPMULTICASTMESSAGEPARSER_H_

// TODO make to cmake_options
#ifndef WITH_BROADCAST
#define WITH_BROADCAST
#endif

#ifdef WITH_BROADCAST
struct sockaddr_in get_sockaddr_in_from_bind_file_descriptor(int file_descriptor);
int initialize_udp_multicast_socket(int unicast_socket_fd);
ssize_t send_udp_mutlicast_message(int multicast_socket_fd,
                                   const device_address *to,
                                   const uint8_t *data,
                                   uint16_t data_length);
int is_multicast_message_receive(int multicast_socket_fd, int timeout_ms);
int receive_udp_mutlicast_message(int socket_fd,
                                  uint8_t *buffer,
                                  ssize_t *read_bytes,
                                  uint16_t buffer_max_length,
                                  device_address *from);
int receive_and_save_udp_multicast_message(int socket_fd,
                                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                                           uint16_t max_data_length);

#endif
#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_LINUX_SHARED_IP_MULTICASTHELPER_MQTTSNUDPMULTICASTMESSAGEPARSER_H_
