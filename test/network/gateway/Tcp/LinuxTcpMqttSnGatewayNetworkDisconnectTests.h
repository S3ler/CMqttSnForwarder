//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKDISCONNECTTESTS_H
#define CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKDISCONNECTTESTS_H

#include <gtest/gtest.h>
#include "../TestConfigurations/MqttSnGatewayNetworkValueParameter.h"

class LinuxTcpMqttSnGatewayNetworkDisconnectTests : public ::testing::TestWithParam<
    MqttSnGatewayNetworkValueParameter> {
 public:
  virtual void SetUp() {

  }
  virtual void TearDown() {

  }

  LinuxTcpMqttSnGatewayNetworkDisconnectTests() {}

  virtual ~LinuxTcpMqttSnGatewayNetworkDisconnectTests() {

  }
};

#endif //CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKDISCONNECTTESTS_H
