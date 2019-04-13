//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H

#include <gtest/gtest.h>
#include "../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "CompareableMqttSnMessageData.h"

class MqttSnFixedSizeRingBufferTests : public ::testing::Test {
 protected:
  void SetUp() override {
    MqttSnFixedSizeRingBufferInit(&ringBuffer);
  }

  void TearDown() override {}

  MqttSnFixedSizeRingBuffer ringBuffer;

  static void generateData(uint8_t *data, uint16_t data_length) {
    for (uint16_t i = 0; i < data_length; ++i) {
      data[i] = static_cast<uint8_t>(i % UINT8_MAX);
    }
  }
  static void generateDeviceAddressFromUint32(device_address *address, uint32_t address_number) {
    int16_t shift = 3;
    for (uint8_t i = 0; i < sizeof(device_address); ++i, shift = (static_cast<int16_t>(3 - i))) {
      if (shift >= 0) {
        address->bytes[i] = static_cast<uint8_t>(address_number >> (shift * 8));
      } else {
        address->bytes[i] = 0;
      }
    }
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H
