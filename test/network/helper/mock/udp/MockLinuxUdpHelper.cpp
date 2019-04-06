//
// Created by SomeDude on 03.04.2019.
//

#include "MockLinuxUdpHelper.h"
#include "MockLinuxUdpDatagram.h"
#include <network/iphelper/MqttSnIpNetworkHelper.h>

int MockLinuxUdpHelper::initializeUdpSocket(uint16_t port) {
  return initialize_udp_socket(port);
}
ssize_t MockLinuxUdpHelper::sendUdpDatagram(int socket_fd, const device_address *to, const uint8_t *buf, uint16_t len) {
  return send_udp_message(socket_fd, to, buf, len);
}
device_address MockLinuxUdpHelper::getDeviceAddressFromSockAddrIn(struct sockaddr_in *sockaddr) {
  device_address result = {0};
  uint32_t ip_as_number = ntohl(sockaddr->sin_addr.s_addr);
  result.bytes[0] = (ip_as_number >> 24) & 0xFF;
  result.bytes[1] = (ip_as_number >> 16) & 0xFF;
  result.bytes[2] = (ip_as_number >> 8) & 0xFF;
  result.bytes[3] = (ip_as_number >> 0) & 0xFF;

  uint16_t port_as_number = (uint16_t) ntohs(sockaddr->sin_port);
  result.bytes[4] = (port_as_number >> 8) & 0xFF;
  result.bytes[5] = (port_as_number >> 0) & 0xFF;

  return result;
  // TODO fixme....
  // FIXME does not compile with:
  //return get_device_address_from_sockaddr_in(sockaddr);
}
int MockLinuxUdpHelper::isUdpMessageReceiver(int socket_fd, uint32_t timeout_ms) {
  return is_udp_message_received(socket_fd, timeout_ms);
}
std::unique_ptr<MockLinuxUdpDatagram> MockLinuxUdpHelper::receiveUdpMessage(int socket_fd, uint16_t max_data_length) {
  uint8_t buffer[max_data_length];
  int buffer_length = max_data_length;
  ssize_t read_bytes;

  struct sockaddr_in recv_sockaddr;
  memset(&recv_sockaddr, 0, sizeof(recv_sockaddr));
  socklen_t recv_sockaddr_socklen = sizeof(recv_sockaddr);

  // TODO what happens if too much data are send aka packetsize > MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE
  // TODO read out first 3 bytes, peek datasize, if too long remove next datasize bytes out of buffer
  // TODO write a testcase for this behaviour

  if ((read_bytes = recvfrom(socket_fd, buffer, buffer_length, MSG_WAITALL,
                             (struct sockaddr *) &recv_sockaddr, &recv_sockaddr_socklen)) <= 0) {
    return std::unique_ptr<MockLinuxUdpDatagram>{nullptr};
  }

  device_address gateway_address = MockLinuxUdpHelper::getDeviceAddressFromSockAddrIn(&recv_sockaddr);

  return std::unique_ptr<MockLinuxUdpDatagram>{
      new MockLinuxUdpDatagram(gateway_address, (uint8_t *) &buffer, read_bytes)};
}
