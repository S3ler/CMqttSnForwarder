//
// Created by SomeDude on 06.04.2019.
//

#ifndef CMQTTSNFORWARDER_MOCKLINUXUDPDATAGRAM_H
#define CMQTTSNFORWARDER_MOCKLINUXUDPDATAGRAM_H

#include <global_defines.h>
#include <vector>
#include <stdio.h>

class MockLinuxUdpDatagram {
 public:
  device_address address;
  std::vector<uint8_t> data;
  uint16_t packetLength;
  MockLinuxUdpDatagram(const device_address &address, const uint8_t *data, ssize_t packetLength);
};

#endif //CMQTTSNFORWARDER_MOCKLINUXUDPDATAGRAM_H
