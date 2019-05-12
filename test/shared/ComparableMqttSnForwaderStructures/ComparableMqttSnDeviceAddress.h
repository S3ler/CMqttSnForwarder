//
// Created by bele on 13.04.19.
//

#ifndef CMQTTSNFORWARDER_COMPARABLEMQTTSNDEVICEADDRESS_H
#define CMQTTSNFORWARDER_COMPARABLEMQTTSNDEVICEADDRESS_H

#include <platform/device_address.h>
#include <vector>

class ComparableMqttSnDeviceAddress {
 public:
  const std::vector<uint8_t> bytes;

  ComparableMqttSnDeviceAddress(const device_address address) :
      bytes(address.bytes, address.bytes + sizeof(device_address)) {}
  bool operator==(const ComparableMqttSnDeviceAddress &rhs) const {
    return bytes == rhs.bytes;
  }
  bool operator!=(const ComparableMqttSnDeviceAddress &rhs) const {
    return !(rhs == *this);
  }
};

#endif //CMQTTSNFORWARDER_COMPARABLEMQTTSNDEVICEADDRESS_H
