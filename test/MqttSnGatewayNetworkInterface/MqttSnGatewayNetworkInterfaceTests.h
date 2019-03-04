//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <GatewayNetworkMock.h>
#include "../../forwarder/MqttSnFixedSizeRingBuffer.h"

GatewayNetworkMock *globalGatewayNetworkMockObj;

class MqttSnGatewayNetworkInterfaceTests : public ::testing::Test {
 public:
  GatewayNetworkMock gatewayNetworkMock;
  virtual void SetUp() {
    globalGatewayNetworkMockObj = &gatewayNetworkMock;
  }
  virtual void TearDown() {
    globalGatewayNetworkMockObj = nullptr;
  }
  MqttSnGatewayNetworkInterfaceTests() {}
  virtual ~MqttSnGatewayNetworkInterfaceTests() {}
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
