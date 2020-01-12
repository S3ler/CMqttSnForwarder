//
// Created by SomeDude on 04.05.2019.
//

#if defined(WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST) || defined(WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST) ||                               \
  defined(WITH_LINUX_UDP_NETWORK_BROADCAST)

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <assert.h>
#include <network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <errno.h>
#include <parser/MqttSnMessageParser.h>
#include <stdio.h>

#include "MqttSnUdpMulticastMessageParser.h"

int initialize_udp_multicast_socket(int unicast_socket_fd, uint32_t broadcast_ip, uint16_t broadcast_port) {
    int multicast_socket_fd;

    // create new udp socket
    if ((multicast_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        close(multicast_socket_fd);
        return -1;
    }

    // disable loopback
    uint8_t disable_loopback = 0;
    if (disable_loopback) {
        u_char loop = 0;
        if (setsockopt(multicast_socket_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0) {
            close(multicast_socket_fd);
            return -1;
        }
    }
    // allow multiple process to access socket
    int option = 1;
    if (setsockopt(multicast_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option)) < 0) {
        close(multicast_socket_fd);
        return -1;
    }

    // bind port for multicast address and port (from ip and port):
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(broadcast_ip);
    sockaddr.sin_port        = htons(broadcast_port);
    if (bind(multicast_socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
        perror("bind failed. Error");
        close(multicast_socket_fd);
        return -1;
    }

    // use the same network interface as the unicast_socket for receiving
    struct sockaddr_in unicast_sockaddr = get_sockaddr_in_from_bind_file_descriptor(unicast_socket_fd);
    struct in_addr     unicast_in_addr  = unicast_sockaddr.sin_addr;
    //socklen_t unicast_sockaddr_socklen = sizeof(unicast_sockaddr);
    if (setsockopt(multicast_socket_fd, IPPROTO_IP, IP_MULTICAST_IF, &unicast_in_addr, sizeof(unicast_in_addr)) < 0) {
        close(multicast_socket_fd);
        return -1;
    }

    // join multicast group
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = htonl(broadcast_ip);
    group.imr_interface.s_addr = unicast_sockaddr.sin_addr.s_addr;
    if (setsockopt(multicast_socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
        close(multicast_socket_fd);
        return -1;
    }

    return multicast_socket_fd;
}
/*
void close_udp_multicast_socket(int multicast_socket_fd) {
  // drop multicast group
  struct ip_mreq group;
  // during teardown we can ignore return value of setsockopt
  setsockopt(multicast_socket_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &group, sizeof(group));
  // close socket
  close(multicast_socket_fd);
}
*/
struct sockaddr_in get_sockaddr_in_from_bind_file_descriptor(int file_descriptor) {
    struct sockaddr_in address = { 0 };
    socklen_t          addrlen = sizeof(address);
    getsockname(file_descriptor, (struct sockaddr *)&address, &addrlen);
    return address;
}
/*
ssize_t send_udp_mutlicast_message(int socket_fd,
                                   const device_address *to,
                                   const uint8_t *data,
                                   uint16_t data_length) {
  return send_udp_message(socket_fd, to, data, data_length);
}
*/
int is_multicast_or_unicast_message_receive(int unicast_socket_fd, int multicast_socket_fd, int timeout_ms) {
    struct timeval interval = { timeout_ms / 1000, (timeout_ms % 1000) * 1000 };
    if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
        interval.tv_sec  = 0;
        interval.tv_usec = 1;  // TODO 0 wait ignored - read docu how intervall behave on 0
    }

    fd_set read_fds;
    FD_ZERO(&read_fds);

    FD_SET(unicast_socket_fd, &read_fds);
    if (multicast_socket_fd > -1) {
        FD_SET(multicast_socket_fd, &read_fds);
    }

    int max_fd = unicast_socket_fd;
    if (multicast_socket_fd > max_fd) {
        max_fd = multicast_socket_fd;
    }

    int activity;
    if (timeout_ms < 0) {
        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    } else {
        activity = select(max_fd + 1, &read_fds, NULL, NULL, &interval);
    }

    if ((activity < 0) && (errno != EINTR)) {
        return -1;
    }
    if (activity == 0) {
        return 0;
    }

    if (multicast_socket_fd > -1) {
        if (FD_ISSET(unicast_socket_fd, &read_fds) && FD_ISSET(multicast_socket_fd, &read_fds)) {
            return 3;
        }
        if (FD_ISSET(multicast_socket_fd, &read_fds)) {
            return 2;
        }
    }
    if (FD_ISSET(unicast_socket_fd, &read_fds)) {
        return 1;
    }
    return -1;
}
/*



*/
/*
int receive_and_save_udp_multicast_message_into_receive_buffer(int socket_fd,
                                                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                               uint16_t max_data_length) {
  uint8_t buffer[max_data_length];
  memset(buffer, 0, max_data_length);
  ssize_t read_bytes;
  device_address received_address = {0};
  uint8_t broadcast_radius = MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BROADCAST_RADIUS;

  if (receive_udp_message(socket_fd, buffer, &read_bytes, max_data_length, &received_address)) {
    return -1;
  }

  MqttSnMessageData receiveMessageData = {0};
  receiveMessageData.broadcast_radius = broadcast_radius;
  receiveMessageData.data_length = read_bytes;
  memcpy(receiveMessageData.data, buffer, receiveMessageData.data_length);
  receiveMessageData.address = received_address;
  put(receiveBuffer, &receiveMessageData);
  return 1;
}
*/
#endif
