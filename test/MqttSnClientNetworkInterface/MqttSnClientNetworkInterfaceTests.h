//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../forwarder/MqttSnClientNetworkInterface.h"
#include "MockMqttSnClientNetwork/ClientNetworkMock.h"

ClientNetworkMock* globalClientNetworkMockObj = nullptr;

class MqttSnClientNetworkInterfaceTests : public ::testing::Test {
 public:
  ClientNetworkMock clientNetworkMock;

  virtual void SetUp() {
    globalClientNetworkMockObj = &clientNetworkMock;
  }
  virtual void TearDown() {
    globalClientNetworkMockObj = nullptr;
  }
  virtual ~MqttSnClientNetworkInterfaceTests() {}

  MqttSnClientNetworkInterfaceTests() {}
};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H
