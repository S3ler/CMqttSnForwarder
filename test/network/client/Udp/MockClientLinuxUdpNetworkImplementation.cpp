//
// Created by SomeDude on 03.04.2019.
//

#include "MockClientLinuxUdpNetworkImplementation.h"
#include "../../helper/mock/udp/MockLinuxUdpHelper.h"
bool MockClientLinuxUdpNetworkImplementation::isNetworkConnected() {
  return !isNetworkDisconnected();
}
bool MockClientLinuxUdpNetworkImplementation::isNetworkDisconnected() {
  return master_socket_fd == -1;
}
device_address MockClientLinuxUdpNetworkImplementation::getForwarderDeviceAddress() {
  return forwarderDeviceAddress;
}
int MockClientLinuxUdpNetworkImplementation::sendToNetwork(const device_address *to,
                                                           const uint8_t *data,
                                                           uint16_t dataLength) {
  if (master_socket_fd == -1) {
    return -1;
  }
  ssize_t send_bytes = MockLinuxUdpHelper::sendUdpDatagram(master_socket_fd, to, data, dataLength);
  if (send_bytes == -1) {
    close(master_socket_fd);
    return -1;
  }
  if (send_bytes != dataLength) {
    return -1;
  }
  return dataLength;
}
bool MockClientLinuxUdpNetworkImplementation::connectNetwork(device_address *forwarderAddress,
                                                             device_address *clientAddress) {
  if (master_socket_fd != -1) {
    return false;
  }
  this->clientDeviceAddress = clientAddress;
  this->forwarderDeviceAddress = *forwarderAddress;

  uint16_t port = clientDeviceAddress->bytes[4] << 8;
  port += clientDeviceAddress->bytes[5] << 0;

  this->master_socket_fd = MockLinuxUdpHelper::initializeUdpSocket(port);

  return this->master_socket_fd != -1;
}
void MockClientLinuxUdpNetworkImplementation::disconnectNetwork() {
  close(master_socket_fd);
  master_socket_fd = -1;
}
int MockClientLinuxUdpNetworkImplementation::loopNetwork(MockClientNetworkReceiver *receiver) {
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
                                              MockClientLinuxUdpNetworkImplementation_BUFFER_SIZE);

  if (mockLinuxUdpDatagram == nullptr) {
    return -1;
  }
  receiver->receive_any_message(&mockLinuxUdpDatagram->address,
                                &mockLinuxUdpDatagram->data[0],
                                mockLinuxUdpDatagram->packetLength);
  return 0;
}
