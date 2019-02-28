//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKCONNECTTESTS_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKCONNECTTESTS_H

#include <gtest/gtest.h>
#include "../TestConfigurations/MqttSnGatewayNetworkValueParameter.h"

class MqttSnGatewayNetworkInterfaceConnectTests  : public ::testing::TestWithParam<MqttSnGatewayNetworkValueParameter>{
 public:
  virtual void SetUp() {

  }
  virtual void TearDown() {

  }

  MqttSnGatewayNetworkInterfaceConnectTests() {}

  virtual ~MqttSnGatewayNetworkInterfaceConnectTests() {

  }
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKCONNECTTESTS_H
