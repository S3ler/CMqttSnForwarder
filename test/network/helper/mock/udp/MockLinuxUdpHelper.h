//
// Created by SomeDude on 03.04.2019.
//

#ifndef CMQTTSNFORWARDER_MOCKLINUXUDPHELPER_H
#define CMQTTSNFORWARDER_MOCKLINUXUDPHELPER_H

#include <global_defines.h>
#include <netinet/in.h>
#include <unistd.h>
#include <network/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <vector>
#include <cstring>
#include <memory>

class MockLinuxUdpDatagram {
 public:
  device_address address;
  std::vector<uint8_t> data;
  uint16_t packetLength;
  MockLinuxUdpDatagram(const device_address &address, const uint8_t *data, ssize_t packetLength)
      : address(address), data(data, data + packetLength), packetLength(packetLength) {}
};

class MockLinuxUdpHelper {
 public:
  static int initializeUdpSocket(uint16_t port) {
    return initialize_udp_socket(port);
  }
  static ssize_t sendUdpDatagram(int socket_fd, const device_address *to, const uint8_t *buf, uint16_t len) {
    return send_udp_message(socket_fd, to, buf, len);
  }
  static device_address getDeviceAddressFromSockAddrIn(struct sockaddr_in *sockaddr) {
    return get_device_address_from_sockaddr_in(sockaddr);
  }
  static int isUdpMessageReceiver(int socket_fd, uint32_t timeout_ms) {
    return is_udp_message_received(socket_fd, timeout_ms);
  }
  static std::unique_ptr<MockLinuxUdpDatagram> receiveUdpMessage(int socket_fd, uint16_t max_data_length) {
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
};

#endif //CMQTTSNFORWARDER_MOCKLINUXUDPHELPER_H
