//
// Created by bele on 10.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTMQTTSNMESSAGEDATA_H
#define CMQTTSNFORWARDER_MOCKCLIENTMQTTSNMESSAGEDATA_H

#include <cstdint>
#include <vector>
#include <cstring>
#include "../../forwarder/global_defines.h"
#include "../../forwarder/MqttSnFixedSizeRingBuffer.h"

class MockClientMqttSnMessageData {
 public:
  const device_address address;
  const uint16_t data_length;
  std::vector<uint8_t> data;
  MockClientMqttSnMessageData(const MqttSnMessageData &mqttSnMessageData)
      : address(mqttSnMessageData.address), data_length(mqttSnMessageData.data_length){
    data.assign(mqttSnMessageData.data, mqttSnMessageData.data + mqttSnMessageData.data_length);
  }
  bool operator==(const MockClientMqttSnMessageData &rhs) const {
    return (memcmp(address.bytes, rhs.address.bytes, sizeof(device_address))==0) &&
        data_length == rhs.data_length &&
        data == rhs.data;
  }
  bool operator!=(const MockClientMqttSnMessageData &rhs) const {
    return !(rhs == *this);
  }
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTMQTTSNMESSAGEDATA_H
