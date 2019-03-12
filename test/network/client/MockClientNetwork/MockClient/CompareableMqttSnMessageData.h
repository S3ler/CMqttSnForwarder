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

class CompareableMqttSnMessageData {
 public:
  const bool use_identifier;
  const device_address address;
  const uint16_t data_length;
  const std::vector<uint8_t> data;

  CompareableMqttSnMessageData(const device_address address, const uint16_t data_length, const uint16_t identifier)
      : address(address),
        data_length(data_length),
        use_identifier(true),
        data(generateMessageData(data_length, identifier, true)) {
    // TODO test what happens with more than 255 clients with 1 byte message and 2 messag
  }

  CompareableMqttSnMessageData(const uint16_t data_length, const uint16_t identifier)
      : address({0}),
        data_length(data_length),
        use_identifier(true),
        data(generateMessageData(data_length, identifier, true)) {
    // TODO test what happens with more than 255 clients with 1 byte message and 2 messag
  }

  std::vector<uint8_t> generateMessageData(uint16_t data_length, const uint16_t identifier, const bool useIdentifier) {
    std::vector<uint8_t> result;
    for (uint16_t i = 0; i < data_length; ++i) {
      result.push_back(static_cast<uint8_t>(i % UINT8_MAX));
    }
    if (useIdentifier) {
      result[0] = static_cast<uint8_t>(identifier >> 0);
      if (data_length > 1) {
        result[1] = static_cast<uint8_t>(identifier >> 8);
      }
    }
    return result;
  }

  CompareableMqttSnMessageData(const MqttSnMessageData &mqttSnMessageData, bool useIdentifier = false)
      : address(mqttSnMessageData.address),
        data_length(mqttSnMessageData.data_length),
        use_identifier(useIdentifier),
        data(mqttSnMessageData.data, mqttSnMessageData.data + mqttSnMessageData.data_length) {}

  bool operator==(const CompareableMqttSnMessageData &rhs) const {
    if (use_identifier) {
      return rhs.use_identifier == use_identifier & data_length == rhs.data_length & data == rhs.data;
    }
    return rhs.use_identifier == use_identifier & data_length == rhs.data_length & data == rhs.data &
        (memcmp(address.bytes, rhs.address.bytes, sizeof(device_address)) == 0);

  }

  bool operator!=(const CompareableMqttSnMessageData &rhs) const {
    return !(rhs == *this);
  }

  friend std::ostream &operator<<(std::ostream &os, const CompareableMqttSnMessageData &data) {
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
