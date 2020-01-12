//
// Created by SomeDude on 06.03.2019.
//

#ifndef CMQTTSNFORWARDER_REMOVEMQTTSNFORWARDINGHEADERTESTS_H
#define CMQTTSNFORWARDER_REMOVEMQTTSNFORWARDINGHEADERTESTS_H

#include <gtest/gtest.h>
#include <MqttSnFixedSizeRingBuffer.h>
#include <MqttSnForwarder.h>

class RemoveMqttSnForwardingHeaderTests : public ::testing::Test {
 public:
  MqttSnMessageData clientMessageData = {0};
  MqttSnMessageData gatewayMessageData = {0};

  virtual void SetUp() {

  }
  virtual void TearDown() {

  }
  virtual ~RemoveMqttSnForwardingHeaderTests() {}
  RemoveMqttSnForwardingHeaderTests() {}
};

#endif //CMQTTSNFORWARDER_REMOVEMQTTSNFORWARDINGHEADERTESTS_H
