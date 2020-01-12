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
#include "MockLinuxUdpDatagram.h"
#include <network/iphelper/MqttSnIpNetworkHelper.h>

class MockLinuxUdpHelper {
 public:
  static int initializeUdpSocket(uint16_t port);
  static ssize_t sendUdpDatagram(int socket_fd, const device_address *to, const uint8_t *buf, uint16_t len);
  static device_address getDeviceAddressFromSockAddrIn(struct sockaddr_in *sockaddr);
  static int isUdpMessageReceiver(int socket_fd, uint32_t timeout_ms);
  static std::unique_ptr<MockLinuxUdpDatagram> receiveUdpMessage(int socket_fd, uint16_t max_data_length);
};

#endif //CMQTTSNFORWARDER_MOCKLINUXUDPHELPER_H
