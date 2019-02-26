//
// Created by bele on 10.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTMQTTSNMESSAGEDATA_H
#define CMQTTSNFORWARDER_MOCKCLIENTMQTTSNMESSAGEDATA_H

#include <cstdint>
#include <vector>
#include <cstring>
#include <ostream>
#include "../../../../../forwarder/global_defines.h"
#include "../../../../../forwarder/MqttSnFixedSizeRingBuffer.h"

class MockClientMqttSnMessageData {
 public:
  const bool use_identifier;
  const device_address address;
  const uint16_t data_length;
  std::vector<uint8_t> data;
  MockClientMqttSnMessageData(const MqttSnMessageData &mqttSnMessageData, bool useIdentifier = false)
      : address(mqttSnMessageData.address), data_length(mqttSnMessageData.data_length), use_identifier(useIdentifier) {
    data.assign(mqttSnMessageData.data, mqttSnMessageData.data + mqttSnMessageData.data_length);
  }
  bool operator==(const MockClientMqttSnMessageData &rhs) const {
    if (use_identifier) {
      return rhs.use_identifier == use_identifier & data_length == rhs.data_length & data == rhs.data;
    }
    return rhs.use_identifier == use_identifier & data_length == rhs.data_length & data == rhs.data &
        (memcmp(address.bytes, rhs.address.bytes, sizeof(device_address)) == 0);

  }
  bool operator!=(const MockClientMqttSnMessageData &rhs) const {
    return !(rhs == *this);
  }
  friend std::ostream &operator<<(std::ostream &os, const MockClientMqttSnMessageData &data) {
    os << "address: ";
    for (uint16_t i = 0; i < sizeof(device_address); i++) {
      os << std::to_string(data.address.bytes[i]);
      if (i != sizeof(device_address) - 1) {
        os << ", ";
      }
    }
    os << " data_length: " << std::to_string(data.data_length) << " data: ";
    for (uint16_t i = 0; i < data.data_length; i++) {
      os << std::to_string(data.data[i]);
      if (i != data.data_length - 1) {
        os << ", ";
      }
    }
    os << " use_identifier: ";
    data.use_identifier ? (os << "true") : (os << "false");
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTMQTTSNMESSAGEDATA_H
