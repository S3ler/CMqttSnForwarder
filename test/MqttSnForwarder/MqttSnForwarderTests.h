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
#include <gmock/gmock-nice-strict.h>
#include "MqttSnForwarderTestsGlobalVariables.h"

using ::testing::Return;
using ::testing::Invoke;
using ::testing::StrictMock;

class MqttSnForwarderTests : public ::testing::Test {
 public:
  StrictMock<ClientNetworkMock> clientNetworkMock;
  StrictMock<GatewayNetworkMock> gatewayNetworkMock;

  MqttSnForwarder mqttSnForwarder;
  device_address client_network_address;
  void *clientNetworkContext = nullptr;

  device_address gateway_network_address;
  device_address mqtt_sn_gateway_address;
  void *gatewayNetworkContext = nullptr;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  virtual void SetUp() {

    globalClientNetworkMockObj = &clientNetworkMock;
    globalGatewayNetworkMockObj = &gatewayNetworkMock;

    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkReceiveBuffer,
                                                           &clientNetworkReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkSendBuffer,
                                                           &clientNetworkSendBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkReceiveBuffer,
                                                           &gatewayNetworkReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkSendBuffer,
                                                           &gatewayNetworkSendBuffer));
    globalMqttSnFixedSizeRingBufferMockMap = &mqttSnFixedSizeRingBufferMockMap;

    globalMqttSnFixedSizeRingBufferMock = &defaultMqttSnFixedSizeRingBufferMock;
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, MqttSnFixedSizeRingBufferInit(_))
        .Times(0);
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, put(_, _))
        .Times(0);
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, pop(_, _))
        .Times(0);
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, isEmpty(_))
        .Times(0);

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

    globalMqttSnFixedSizeRingBufferMockMap = nullptr;

  }
  /*
  static MqttSnMessageData *clientMessageData;
  static MqttSnMessageData *toCopyMessageData;

  static int d(MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) {
    if (toCopyMessageData != nullptr) {
      memcpy(clientMessageData, toCopyMessageData, sizeof(MqttSnMessageData));
    }
    MqttSnForwarderTests::clientMessageData = clientMessageData;
    return 0;
  }
  */
  MqttSnForwarderTests() {}
  virtual ~MqttSnForwarderTests() {}
};


#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H
