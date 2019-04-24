//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_COMPARABLEMQTTSNMESSAGEDATA_H
#define CMQTTSNFORWARDER_COMPARABLEMQTTSNMESSAGEDATA_H

#include "MqttSnFixedSizeRingBuffer.h"
#include "ComparableMqttSnDeviceAddress.h"
#include <cstring>

class ComparableMqttSnMessageData {
 public:
  const ComparableMqttSnDeviceAddress address;
  const uint16_t data_length;
  const std::vector<uint8_t> data;

  ComparableMqttSnMessageData(const MqttSnMessageData mqttSnMessageData) :
      address(mqttSnMessageData.address),
      data_length(mqttSnMessageData.data_length),
      data(mqttSnMessageData.data, mqttSnMessageData.data + CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) {}

  bool operator==(const ComparableMqttSnMessageData &rhs) const {
    return address == rhs.address &&
        data_length == rhs.data_length &&
        data == rhs.data;
  }
  bool operator!=(const ComparableMqttSnMessageData &rhs) const {
    return !(rhs == *this);
  }
};

#endif //CMQTTSNFORWARDER_COMPARABLEMQTTSNMESSAGEDATA_H
