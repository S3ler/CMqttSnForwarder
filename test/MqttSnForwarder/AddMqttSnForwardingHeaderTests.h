//
// Created by SomeDude on 06.03.2019.
//

#ifndef CMQTTSNFORWARDER_ADDMQTTSNFORWARDINGHEADERTESTS_H
#define CMQTTSNFORWARDER_ADDMQTTSNFORWARDINGHEADERTESTS_H

#include <gtest/gtest.h>
#include <MqttSnFixedSizeRingBuffer.h>
#include <MqttSnForwarder.h>

class AddMqttSnForwardingHeaderTests : public ::testing::Test {
 public:
  MqttSnMessageData clientMessageData = {0};
  MqttSnMessageData gatewayMessageData = {0};

  virtual ~AddMqttSnForwardingHeaderTests() {}
  AddMqttSnForwardingHeaderTests() {}
};


#endif //CMQTTSNFORWARDER_ADDMQTTSNFORWARDINGHEADERTESTS_H
