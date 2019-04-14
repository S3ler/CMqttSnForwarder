//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ClientNetworkMock.h>
#include "../../forwarder/MqttSnClientNetworkInterface.h"

#ifndef GLOBAL_TEST_EXEC
ClientNetworkMock* globalClientNetworkMockObj = nullptr;
#else
extern ClientNetworkMock* globalClientNetworkMockObj;
#endif

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
