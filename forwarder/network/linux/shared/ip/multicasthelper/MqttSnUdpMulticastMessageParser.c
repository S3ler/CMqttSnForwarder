//
// Created by SomeDude on 04.05.2019.
//

#if defined(WITH_TCP_BROADCAST) || defined(WITH_UDP_BROADCAST)

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <assert.h>
#include <forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <errno.h>
#include <forwarder/MqttSnMessageParser.h>

#include "MqttSnUdpMulticastMessageParser.h"

int initialize_udp_multicast_socket(int unicast_socket_fd, uint32_t broadcast_ip, uint16_t broadcast_port) {
  int multicast_socket_fd;

  // socket new udp socket
  if ((multicast_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    close(multicast_socket_fd);
    return -1;
  }

  // disable loopback
  // TODO do i need this?
  uint8_t disable_loopback = 0;
  if (disable_loopback) {
    u_char loop;
    if (setsockopt(multicast_socket_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0) {
      close(multicast_socket_fd);
      return -1;
    }
  }
  // allow multiple process to access socket
  int option = 1;
  if (setsockopt(multicast_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    close(multicast_socket_fd);
    return -1;
  }

  // bind port for multicast address and port (from ip and port):
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  // sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_addr.s_addr = htonl(broadcast_ip);
  sockaddr.sin_port = htons(broadcast_port);
  if (bind(multicast_socket_fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    close(multicast_socket_fd);
    return -1;
  }

  // use the same network interface as the unicast_socket for receiving
  struct sockaddr_in unicast_sockaddr = get_sockaddr_in_from_bind_file_descriptor(unicast_socket_fd);
  struct in_addr unicast_in_addr = unicast_sockaddr.sin_addr;
  //socklen_t unicast_sockaddr_socklen = sizeof(unicast_sockaddr);
  if (setsockopt(multicast_socket_fd,
                 IPPROTO_IP,
                 IP_MULTICAST_IF,
                 &unicast_in_addr,
                 sizeof(unicast_in_addr)) < 0) {
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

void close_udp_multicast_socket(int multicast_socket_fd) {
  // drop multicast group
  struct ip_mreq group;
  // during teardown we can ignore return value of setsockopt
  setsockopt(multicast_socket_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &group, sizeof(group));
  // close socket
  close(multicast_socket_fd);
}

struct sockaddr_in get_sockaddr_in_from_bind_file_descriptor(int file_descriptor) {
  struct sockaddr_in address = {0};
  socklen_t addrlen = sizeof(address);
  int rc = getsockname(file_descriptor, (struct sockaddr *) &address, &addrlen);
  assert(rc != -1); // TODO handle
  return address;
}

ssize_t send_udp_mutlicast_message(int socket_fd,
                                   const device_address *to,
                                   const uint8_t *data,
                                   uint16_t data_length) {
  return send_udp_message(socket_fd, to, data, data_length);
}

int is_multicast_or_unicast_message_receive(int unicast_socket_fd, int multicast_socket_fd, int timeout_ms) {
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  fd_set readfds;
  FD_ZERO(&readfds);

  FD_SET(unicast_socket_fd, &readfds);
  FD_SET(multicast_socket_fd, &readfds);

  int max_sd = unicast_socket_fd;
  if (multicast_socket_fd > max_sd) {
    max_sd = multicast_socket_fd;
  }

  int activity;
  if (timeout_ms == -1) {
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
  } else {
    activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
  }

  if ((activity < 0) && (errno != EINTR)) {
    return -1;
  }
  if (activity == 0) {
    return 0;
  }

  if (FD_ISSET(unicast_socket_fd, &readfds) && FD_ISSET(multicast_socket_fd, &readfds)) {
    return 3;
  }
  if (FD_ISSET(unicast_socket_fd, &readfds)) {
    return 1;
  }
  if (FD_ISSET(multicast_socket_fd, &readfds)) {
    return 2;
  }

  return -1;
}

int is_multicast_message_receive(int multicast_socket_fd, int timeout_ms) {
  return is_udp_message_received(multicast_socket_fd, timeout_ms);
}

int receive_udp_mutlicast_message(int socket_fd,
                                  uint8_t *buffer,
                                  ssize_t *read_bytes,
                                  uint16_t buffer_max_length,
                                  device_address *from) {
  return receive_udp_message(socket_fd, buffer, read_bytes, buffer_max_length, from);

}

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

  return save_udp_messages_into_receive_buffer(buffer,
                                               read_bytes,
                                               broadcast_radius,
                                               received_address,
                                               receiveBuffer);
}

#endif
