//
// Created by SomeDude on 08.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNMULTIPLEFORWARDERTEST_MQTTSNMULTIPLEFORWARDERTESTS_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNMULTIPLEFORWARDERTEST_MQTTSNMULTIPLEFORWARDERTESTS_H_

#include <gtest/gtest.h>
#include <forwarder/MqttSnForwarder.h>
#include <client/MqttSnClient.h>
#include <gateway/MqttSnGateway.h>
#include "gmock/gmock.h"

using std::vector;

class MqttSnMultipleForwarderTests  : public ::testing::Test {
  vector<MqttSnForwarder> forwarders;
  vector<MqttSnClient> clients;
  MqttSnGateway gateway;
 protected:
  void SetUp() override;
  void TearDown() override;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNMULTIPLEFORWARDERTEST_MQTTSNMULTIPLEFORWARDERTESTS_H_
