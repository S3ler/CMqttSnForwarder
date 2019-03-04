//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H

#include <gtest/gtest.h>
#include <ClientNetworkMock.h>
#include <GatewayNetworkMock.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <MqttSnForwarder.h>

ClientNetworkMock *globalClientNetworkMockObj;
GatewayNetworkMock *globalGatewayNetworkMockObj;
MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;

class MqttSnForwarderTests : public ::testing::Test {
 public:
  ClientNetworkMock clientNetworkMock;
  GatewayNetworkMock gatewayNetworkMock;
  MqttSnFixedSizeRingBufferMock mqttSnFixedSizeRingBufferMock;

  MqttSnForwarder mqttSnForwarder;
  device_address client_network_address;
  void *clientNetworkContext = nullptr;

  device_address gateway_network_address;
  device_address mqtt_sn_gateway_address;
  void *gatewayNetworkContext = nullptr;

  virtual void SetUp() {
    globalClientNetworkMockObj = &clientNetworkMock;
    globalGatewayNetworkMockObj = &gatewayNetworkMock;
    globalMqttSnFixedSizeRingBufferMock = &mqttSnFixedSizeRingBufferMock;

    {
      device_address client_network_address({0, 0, 0, 0, 0, 0});
      this->client_network_address = client_network_address;

      device_address gateway_network_address({0, 0, 0, 0, 0, 0});
      device_address mqtt_sn_gateway_address({0, 0, 0, 0, 0, 0});
      this->gateway_network_address = gateway_network_address;
      this->mqtt_sn_gateway_address = mqtt_sn_gateway_address;
    }
    clientNetworkMock.DelegateToFake();
    EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnForwarder.clientNetwork, clientNetworkContext))
        .Times(1);

    gatewayNetworkMock.DelegateToFake();
    EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
        .Times(1);

    ASSERT_EQ(ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                                &client_network_address,
                                clientNetworkContext,
                                mock_client_network_init), 0);
    EXPECT_EQ(GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                                 &gateway_network_address,
                                 &mqtt_sn_gateway_address,
                                 gatewayNetworkContext,
                                 mock_gateway_network_init), 0);
  }
  virtual void TearDown() {
    globalClientNetworkMockObj = nullptr;
    globalGatewayNetworkMockObj = nullptr;
  }
  MqttSnForwarderTests() {}
  virtual ~MqttSnForwarderTests() {}
};

TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_ClientNetworkMessageReceived_Test) {

  {
    if (MqttSnForwarderInit(&mqttSnForwarder, clientNetworkContext, gatewayNetworkContext) != 0) {
      MqttSnForwarderDeinit(&mqttSnForwarder);
      ASSERT_TRUE(false) << "SHOULD NOT HAPPEN";
    }
/*
    if (ClientNetworkConnect(&mqttSnForwarder.clientNetwork, mqttSnForwarder.clientNetworkContext) != 0) {
      MqttSnForwarderDeinit(&mqttSnForwarder);
      ASSERT_TRUE(false) << "SHOULD NOT HAPPEN";
    }

    if (GatewayNetworkConnect(&mqttSnForwarder.gatewayNetwork, mqttSnForwarder.gatewayNetworkContext) != 0) {
      MqttSnForwarderDeinit(&mqttSnForwarder);
      ASSERT_TRUE(false) << "SHOULD NOT HAPPEN";
    }
*/
  }



  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  //MqttSnForwarderDeinit(&mqttSnForwarder);

}

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H
