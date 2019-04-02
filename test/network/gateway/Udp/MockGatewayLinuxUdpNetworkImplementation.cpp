//
// Created by SomeDude on 01.04.2019.
//

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "MockGatewayLinuxUdpNetworkImplementation.h"

bool MockGatewayLinuxUdpNetworkImplementation::isNetworkConnected() {
  return !isNetworkDisconnected();
}
bool MockGatewayLinuxUdpNetworkImplementation::isNetworkDisconnected() {
  return master_socket_fd == -1;
}
device_address MockGatewayLinuxUdpNetworkImplementation::getForwarderDeviceAddress() {
  return forwarderDeviceAddress;
}
int MockGatewayLinuxUdpNetworkImplementation::sendNetwork(const device_address *to, const uint8_t *buf, uint16_t len) {
  if (master_socket_fd == -1) {
    return -1;
  }
  struct sockaddr_in to_sockaddr = getSockAddrInFromDeviceAddress(to);
  socklen_t to_sockaddr_socklen = sizeof(to_sockaddr);
  int flags = 0;

  ssize_t send_bytes =
      sendto(master_socket_fd, buf, len, flags, (const struct sockaddr *) &to_sockaddr, to_sockaddr_socklen);
  if (send_bytes == -1) {
    close(master_socket_fd);
    master_socket_fd = -1;
    return -1;
  }
  if (send_bytes != len) {
    return -1;
  }
  return len;
}
bool MockGatewayLinuxUdpNetworkImplementation::connectNetwork(device_address *gatewayDeviceAddress,
                                                              device_address *forwarderDeviceAddress) {
  if (master_socket_fd != -1) {
    return false;
  }
  this->gatewayDeviceAddress = gatewayDeviceAddress;
  this->forwarderDeviceAddress = *forwarderDeviceAddress;

  uint16_t port = gatewayDeviceAddress->bytes[4] << 8;
  port += gatewayDeviceAddress->bytes[5] << 0;

  int master_socket_fd;
  struct sockaddr_in sockaddr;

  if ((master_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    close(master_socket_fd);
    return false;
  }

  int option = true;
  if (setsockopt(master_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    close(master_socket_fd);
    return false;
  }

  // type of socket created
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(port);

  // bind the socket to localhost
  if (bind(master_socket_fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    close(master_socket_fd);
    return false;
  }

  this->master_socket_fd = master_socket_fd;

  return true;
}
void MockGatewayLinuxUdpNetworkImplementation::disconnectNetwork() {
  close(master_socket_fd);
  master_socket_fd = -1;
}
int MockGatewayLinuxUdpNetworkImplementation::loopNetwork(MockGatewayNetworkReceiver *receiver) {
  if (master_socket_fd == -1) {
    return -1;
  }
  return receiveForwarderMessage(receiver);
}

int MockGatewayLinuxUdpNetworkImplementation::receiveForwarderMessage(MockGatewayNetworkReceiver *receiver) {
  int timeout_ms = 100;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  // init client_socket to readset
  fd_set readfds;
  FD_ZERO(&readfds);

  // add master socket to set
  FD_SET(master_socket_fd, &readfds);
  int max_sd = master_socket_fd;

  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
  // int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

  if ((activity < 0) && (errno != EINTR)) {
    return -1;
  }
  if (activity == 0) {
    return 0;
  }

  if (FD_ISSET(master_socket_fd, &readfds)) {
    int sd = master_socket_fd;
    uint8_t buffer[MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE];
    int buffer_length = MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE;
    int valread;

    struct sockaddr_in forwarder_sockaddr;
    memset(&forwarder_sockaddr, 0, sizeof(forwarder_sockaddr));
    socklen_t forwarder_sockaddr_socklen = sizeof(forwarder_sockaddr);

    // TODO what happens if too much data are send aka packetsize > MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE
    // TODO read out first 3 bytes, peek datasize, if too long remove next datasize bytes out of buffer
    // TODO write a testcase for this behaviour

    if ((valread = recvfrom(sd, buffer, buffer_length, MSG_WAITALL,
                            (struct sockaddr *) &forwarder_sockaddr, &forwarder_sockaddr_socklen)) <= 0) {
      // client_socket broken
      close(sd);
      master_socket_fd = -1;
      return -1;
    } else {
      device_address peerAddress = getDeviceAddressFromSockAddrIn(&forwarder_sockaddr);
      receiver->receive_any_message(&peerAddress, buffer, valread);
    }
  }

  return 0;
}
device_address MockGatewayLinuxUdpNetworkImplementation::getDeviceAddressFromSockAddrIn(struct sockaddr_in *sockaddr) {
  device_address result = {0};
  unsigned char *ip = (unsigned char *) &sockaddr->sin_addr.s_addr;
  result.bytes[0] = ip[0];
  result.bytes[1] = ip[1];
  result.bytes[2] = ip[2];
  result.bytes[3] = ip[3];

  uint16_t port_as_number = (uint16_t) ntohs(sockaddr->sin_port);
  result.bytes[4] = (uint8_t) (port_as_number >> 8);
  result.bytes[5] = (uint8_t) (port_as_number >> 0);
  return result;
}
sockaddr_in MockGatewayLinuxUdpNetworkImplementation::getSockAddrInFromDeviceAddress(const device_address *deviceAddress) {

  uint16_t port = (((uint16_t) deviceAddress->bytes[4]) << 8)
      + ((uint16_t) deviceAddress->bytes[5]);
  sa_family_t family = AF_INET;
  uint32_t ip = (((uint32_t) deviceAddress->bytes[0]) << 24)
      + (((uint32_t) deviceAddress->bytes[1]) << 16)
      + (((uint32_t) deviceAddress->bytes[2]) << 8)
      + (((uint32_t) deviceAddress->bytes[3]) << 0);

  struct sockaddr_in address;
  address.sin_family = family;
  address.sin_port = port;
  address.sin_addr.s_addr = htonl(ip);

  return address;
}
