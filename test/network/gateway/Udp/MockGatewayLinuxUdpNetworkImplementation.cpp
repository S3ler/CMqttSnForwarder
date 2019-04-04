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
#include "../../helper/mock/udp/MockLinuxUdpHelper.h"

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
  ssize_t send_bytes = MockLinuxUdpHelper::sendUdpDatagram(master_socket_fd, to, buf, len);
  if (send_bytes == -1) {
    close(master_socket_fd);
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

  this->master_socket_fd = MockLinuxUdpHelper::initializeUdpSocket(port);

  return this->master_socket_fd != -1;
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
  int message_received = MockLinuxUdpHelper::isUdpMessageReceiver(this->master_socket_fd, timeout_ms);
  if (message_received == -1) {
    return -1;
  }

  if (message_received == 0) {
    return 0;
  }

  std::unique_ptr<MockLinuxUdpDatagram> mockLinuxUdpDatagram
      = MockLinuxUdpHelper::receiveUdpMessage(master_socket_fd,
                                              MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE);

  if (mockLinuxUdpDatagram == nullptr) {
    return -1;
  }
  receiver->receive_any_message(&mockLinuxUdpDatagram->address,
                                &mockLinuxUdpDatagram->data[0],
                                mockLinuxUdpDatagram->packetLength);
  return 0;
}
