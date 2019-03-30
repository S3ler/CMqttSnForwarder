//
// Created by bele on 03.02.19.
//

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "MockGatewayLinuxTcpNetworkImplementation.h"

bool MockGatewayLinuxTcpNetworkImplementation::isNetworkConnected() {
  return !isNetworkDisconnected();
}

bool MockGatewayLinuxTcpNetworkImplementation::isNetworkDisconnected() {
  // check if someone is connected
  return master_socket_fd == -1;
}

int MockGatewayLinuxTcpNetworkImplementation::sendNetwork(const device_address *to,
                                                          const uint8_t *data,
                                                          uint16_t dataLength) {
  if (forwarder_socket_fd == -1) {
    return -1;
  }

  device_address forwarderAddress = {0};
  if (getDeviceAddressFromFileDescriptor(forwarder_socket_fd, &forwarderAddress) != 0) {
    return -1;
  }
  if (memcmp(&forwarderAddress, to, sizeof(device_address)) != 0) {
    return -1;
  }

  // check if Tcp socket is connected, if not then do it!
  uint16_t total_send_bytes = 0;
  while (total_send_bytes < dataLength) {
    uint16_t send_bytes = send(forwarder_socket_fd, data, dataLength, 0);
    if (send_bytes <= 0) {
      close(forwarder_socket_fd);
      forwarder_socket_fd = -1;
      return -1;
    }
    total_send_bytes += send_bytes;
  }
  return total_send_bytes;
}

bool MockGatewayLinuxTcpNetworkImplementation::connectNetwork(device_address *gatewayDeviceAddress) {
  this->gatewayDeviceAddress = gatewayDeviceAddress;

  uint16_t port = gatewayDeviceAddress->bytes[4] << 8;
  port += gatewayDeviceAddress->bytes[5] << 0;

  int master_socket_fd;
  struct sockaddr_in sockaddr;

  // create master socket
  if ((master_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return false;
  }

  int option = true;
  if (setsockopt(master_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    return false;
  }

  // type of socket created
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(port);

  // bind the socket to localhost
  if (bind(master_socket_fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    return false;
  }

  if (listen(master_socket_fd, 3) < 0) {
    return false;
  }

  this->master_socket_fd = master_socket_fd;
  return true;
}

void MockGatewayLinuxTcpNetworkImplementation::disconnectNetwork() {
  close(master_socket_fd);
  master_socket_fd = -1;
  close(forwarder_socket_fd);
  forwarder_socket_fd = -1;
}

int MockGatewayLinuxTcpNetworkImplementation::loopNetwork(MockGatewayNetworkReceiver *receiver) {

  int timeout_ms = 100;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  if (forwarder_socket_fd == -1) {
    return awaitForwarderConnection(receiver);
  }
  return receiveForwarderMessage(receiver);

}

int MockGatewayLinuxTcpNetworkImplementation::awaitForwarderConnection(MockGatewayNetworkReceiver *receiver) {

  int timeout_ms = 100;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  // init master_socket readset
  fd_set readfds;
  FD_ZERO(&readfds);

  // add master socket to set
  FD_SET(master_socket_fd, &readfds);
  int max_sd = master_socket_fd;

  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);

  if ((activity < 0) && (errno != EINTR)) {
    printf("select error");
    exit(EXIT_FAILURE);
  }
  if (activity == 0) {
    // no connection
    return 0;
  }

  if (FD_ISSET(master_socket_fd, &readfds)) {
    struct sockaddr_in sockaddr;
    int addrlen = sizeof(sockaddr);
    if ((forwarder_socket_fd = accept(master_socket_fd, (struct sockaddr *) &sockaddr,
                                      (socklen_t *) &addrlen)) < 0) {
      return -1;
    }
  }
  return 0;
}

int MockGatewayLinuxTcpNetworkImplementation::getDeviceAddressFromFileDescriptor(int peer_fd,
                                                                                 device_address *peerAddress) {
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  getpeername(peer_fd, (struct sockaddr *) &address, &addrlen);

  unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;
  (*peerAddress).bytes[0] = ip[0];
  (*peerAddress).bytes[1] = ip[1];
  (*peerAddress).bytes[2] = ip[2];
  (*peerAddress).bytes[3] = ip[3];

  uint16_t port_as_number = (uint16_t) htons(address.sin_port);
  (*peerAddress).bytes[4] = (uint8_t) (port_as_number >> 8);
  (*peerAddress).bytes[5] = (uint8_t) (port_as_number >> 0);
  return 0;
}

int MockGatewayLinuxTcpNetworkImplementation::receiveForwarderMessage(MockGatewayNetworkReceiver *receiver) {

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
  FD_SET(forwarder_socket_fd, &readfds);
  int max_sd = forwarder_socket_fd;

  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
  // int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

  if ((activity < 0) && (errno != EINTR)) {
    //printf("select error");
    exit(EXIT_FAILURE);
  }
  if (activity == 0) {
    // no connection
    return 0;
  }

  if (FD_ISSET(forwarder_socket_fd, &readfds)) {
    int sd = forwarder_socket_fd;
    uint8_t buffer[MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE];
    int buffer_length = MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE;
    int valread;
    if ((valread = read(sd, buffer, buffer_length)) <= 0) {
      // client_socket broken
      close(sd);
      forwarder_socket_fd = -1;
    } else {
      device_address peerAddress;
      if (getDeviceAddressFromFileDescriptor(forwarder_socket_fd, &peerAddress) < 0) {
        return -1;
      }
      receiver->receive_any_message(&peerAddress, buffer, valread);
    }
  }

  return 0;
}
device_address MockGatewayLinuxTcpNetworkImplementation::getForwarderDeviceAddress() {
  device_address result = {0};
  if (forwarder_socket_fd >= 0) {
    getDeviceAddressFromFileDescriptor(forwarder_socket_fd, &result);
  }
  return result;
}
