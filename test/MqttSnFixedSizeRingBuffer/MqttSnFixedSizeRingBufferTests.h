//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H

#include <gtest/gtest.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>

class MqttSnFixedSizeRingBufferTests : public ::testing::Test {
 protected:
  void SetUp() override {
    MqttSnFixedSizeRingBufferInit(&ringBuffer);
  }

  void TearDown() override {}

  MqttSnFixedSizeRingBuffer ringBuffer;

};

#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H
