//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_COMPARABLEMQTTSNMESSAGEDATA_H
#define CMQTTSNFORWARDER_COMPARABLEMQTTSNMESSAGEDATA_H

#include "../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include <cstring>

class ComparableMqttSnMessageData {
 public:
  const device_address address;
  const uint16_t data_length;
  const std::vector<uint8_t> data;

  ComparableMqttSnMessageData(const MqttSnMessageData mqttSnMessageData) :
      address(mqttSnMessageData.address),
      data_length(mqttSnMessageData.data_length),
      data(mqttSnMessageData.data, mqttSnMessageData.data + MAX_MESSAGE_LENGTH) {}

  bool operator==(const ComparableMqttSnMessageData &rhs) const {
    return memcmp(&address, &rhs.address, sizeof(device_address)) == 0 &&
        data_length == rhs.data_length &&
        data == rhs.data;
  }
  bool operator!=(const ComparableMqttSnMessageData &rhs) const {
    return !(rhs == *this);
  }
};

#endif //CMQTTSNFORWARDER_COMPARABLEMQTTSNMESSAGEDATA_H
