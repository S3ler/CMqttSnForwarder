//
// Created by SomeDude on 08.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNMULTIPLEFORWARDERTEST_MQTTSNMULTIPLEFORWARDERTESTS_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNMULTIPLEFORWARDERTEST_MQTTSNMULTIPLEFORWARDERTESTS_H_

#include <gtest/gtest.h>
#include <forwarder/MqttSnForwarder.h>
#include <client/MqttSnClient.h>
#include <gateway/MqttSnGateway.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainer.h>
#include <test/MqttSnClient/MqttSnClientTestContainer.h>
#include <test/MqttSnGateway/MqttSnGatewayArsmbTestContainer.h>
#include "gmock/gmock.h"

using std::vector;
using std::array;
using std::string;
using std::shared_ptr;

class MqttSnMultipleForwarderTests  : public ::testing::Test {
  vector<shared_ptr<MqttSnForwarderTestContainer>> forwarders;
  vector<shared_ptr<MqttSnClientTestContainer>> clients;
  vector<std::shared_ptr<MqttSnGatewayTestContainerInterface>> gateways;

  MqttSnLogger logger;
 protected:
  void SetUp() override;
  void TearDown() override;
 private:
  
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNMULTIPLEFORWARDERTEST_MQTTSNMULTIPLEFORWARDERTESTS_H_
