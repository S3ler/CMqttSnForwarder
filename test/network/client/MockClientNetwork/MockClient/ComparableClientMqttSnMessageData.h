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

class ComparableClientMqttSnMessageData {
 public:

  const bool use_identifier;
  const device_address address;
  const uint16_t data_length;
  const std::vector<uint8_t> data;

  ComparableClientMqttSnMessageData(const MqttSnMessageData &mqttSnMessageData,
                                    bool useIdentifier = false)
      : address(mqttSnMessageData.address),
        data_length(mqttSnMessageData.data_length),
        use_identifier(useIdentifier),
        data(mqttSnMessageData.data, mqttSnMessageData.data + mqttSnMessageData.data_length) {}

  ComparableClientMqttSnMessageData(device_address *address,
                                    uint8_t *data,
                                    uint16_t data_length,
                                    bool useIdentifier)
      : address(*address),
        data_length(data_length),
        use_identifier(useIdentifier),
        data(data, data + data_length) {}

  ComparableClientMqttSnMessageData(const uint16_t data_length,
                                    const device_address *address,
                                    const uint16_t identifier)
      : address(*address),
        data_length(data_length),
        use_identifier(true),
        data(generateMessageData(data_length, identifier, true)) {
  }

  ComparableClientMqttSnMessageData(const uint16_t data_length,
                                    const device_address *address,
                                    const uint16_t identifier,
                                    const bool useIdentifier)
      : address(*address),
        data_length(data_length),
        use_identifier(useIdentifier),
        data(generateMessageData(data_length, identifier, true)) {
  }

  std::vector<uint8_t> generateMessageData(uint16_t data_length,
                                           const uint16_t identifier,
                                           const bool useIdentifier) {
    if (data_length < 2) {
      // TODO exception
    }
    if (useIdentifier && data_length < 3) {
      // TODO exception
    }

    std::vector<uint8_t> result;
    uint16_t bytesGenerated = 0;
    bool threeOctetsLengthMessageHeader = false;
    if (data_length > UINT8_MAX) {
      // generate three octets length message header
      result.push_back(static_cast<uint8_t>(0x01));
      result.push_back(static_cast<uint8_t>(data_length >> 8));
      result.push_back(static_cast<uint8_t>(data_length >> 0));
      bytesGenerated = 3;
      threeOctetsLengthMessageHeader = true;
    } else {
      // generate one octets length message header
      result.push_back(static_cast<uint8_t>(data_length));
      bytesGenerated = 1;
    }

    for (; bytesGenerated < data_length; ++bytesGenerated) {
      result.push_back(static_cast<uint8_t>(bytesGenerated % UINT8_MAX));
    }
    if (useIdentifier) {
      if (threeOctetsLengthMessageHeader) {
        result[4] = static_cast<uint8_t>(identifier >> 0);
        result[5] = static_cast<uint8_t>(identifier >> 8);
      } else {
        result[1] = static_cast<uint8_t>(identifier >> 0);
        result[2] = static_cast<uint8_t>(identifier >> 8);
      }
    }
    return result;
  }

  bool operator==(const ComparableClientMqttSnMessageData &rhs) const {
    if (use_identifier) {
      return rhs.use_identifier == use_identifier & data_length == rhs.data_length & data == rhs.data;
    }
    return rhs.use_identifier == use_identifier & data_length == rhs.data_length & data == rhs.data &
        (memcmp(address.bytes, rhs.address.bytes, sizeof(device_address)) == 0);

  }

  bool operator!=(const ComparableClientMqttSnMessageData &rhs) const {
    return !(rhs == *this);
  }

  friend std::ostream &operator<<(std::ostream &os, const ComparableClientMqttSnMessageData &data) {
    os << "forwarderAddress: ";
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
