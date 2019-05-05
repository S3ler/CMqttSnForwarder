//
// Created by SomeDude on 04.05.2019.
//

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <assert.h>
#include <forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>

#include "MqttSnUdpMulticastMessageParser.h"

#define MQTT_SN_MDNS_IP     "224.0.0.251"
#define MQTT_SN_MDNS_PORT   5353

int initialize_udp_multicast_socket(int unicast_socket_fd) {
  // parts from: https://www.tldp.org/HOWTO/Multicast-HOWTO-6.html
  int multicast_socket_fd;

  // socket new udp socket
  if ((multicast_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    close(multicast_socket_fd);
    return -1;
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
  sockaddr.sin_addr.s_addr = inet_addr(MQTT_SN_MDNS_IP);
  sockaddr.sin_port = htons(MQTT_SN_MDNS_PORT);
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
  group.imr_multiaddr.s_addr = inet_addr(MQTT_SN_MDNS_IP);
  group.imr_interface.s_addr = unicast_sockaddr.sin_addr.s_addr;
  if (setsockopt(multicast_socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
    close(multicast_socket_fd);
    return -1;
  }

  return multicast_socket_fd;
}

struct sockaddr_in get_sockaddr_in_from_bind_file_descriptor(int file_descriptor) {
  struct sockaddr_in address = {0};
  socklen_t addrlen = sizeof(address);
  int rc = getsockname(file_descriptor, (struct sockaddr *) &address, &addrlen);
  assert(rc != -1); // TODO handle
  return address;
}

ssize_t send_udp_mutlicast_message(int multicast_socket_fd,
                                   const device_address *to,
                                   const uint8_t *data,
                                   uint16_t data_length) {
  return send_udp_message(multicast_socket_fd, to, data, data_length);
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

int receive_and_save_udp_multicast_message(int socket_fd,
                                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                                           uint16_t max_data_length) {
  return receive_and_udp_message_into_receive_buffer(socket_fd, receiveBuffer, max_data_length);
}




