//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_COMPAREABLEMQTTSNMESSAGEDATA_H
#define CMQTTSNFORWARDER_COMPAREABLEMQTTSNMESSAGEDATA_H

#include "MqttSnFixedSizeRingBuffer.h"
#include "CompareableMqttSnDeviceAddress.h"
#include <cstring>

class CompareableMqttSnMessageData {
 public:
  const ComparableMqttSnDeviceAddress address;
  const uint16_t data_length;
  const std::vector<uint8_t> data;

  CompareableMqttSnMessageData(const MqttSnMessageData mqttSnMessageData) :
      address(mqttSnMessageData.address),
      data_length(mqttSnMessageData.data_length),
      data(mqttSnMessageData.data, mqttSnMessageData.data + CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) {}

  bool operator==(const CompareableMqttSnMessageData &rhs) const {
    return address == rhs.address &&
        data_length == rhs.data_length &&
        data == rhs.data;
  }
  bool operator!=(const CompareableMqttSnMessageData &rhs) const {
    return !(rhs == *this);
  }
};

#endif //CMQTTSNFORWARDER_COMPAREABLEMQTTSNMESSAGEDATA_H
