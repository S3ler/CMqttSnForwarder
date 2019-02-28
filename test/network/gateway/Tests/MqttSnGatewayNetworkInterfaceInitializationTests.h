//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACEINITIALIZATIONTESTS_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACEINITIALIZATIONTESTS_H

#include <gtest/gtest.h>
#include "../TestConfigurations/MqttSnGatewayNetworkValueParameter.h"

class MqttSnGatewayNetworkInterfaceInitializationTests : public ::testing::TestWithParam<
    MqttSnGatewayNetworkValueParameter> {
 public:

  virtual void SetUp() {

  }
  virtual void TearDown() {

  }

  MqttSnGatewayNetworkInterfaceInitializationTests() {}

  virtual ~MqttSnGatewayNetworkInterfaceInitializationTests() {

  }
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACEINITIALIZATIONTESTS_H
