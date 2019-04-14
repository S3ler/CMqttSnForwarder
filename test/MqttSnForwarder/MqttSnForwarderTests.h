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
#include "../shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h"

using ::testing::Return;
using ::testing::Invoke;
using ::testing::StrictMock;
using ::testing::NiceMock;

class MqttSnForwarderTests : public ::testing::Test {
 public:
  NiceMock<ClientNetworkMock> clientNetworkMock;
  NiceMock<GatewayNetworkMock> gatewayNetworkMock;

  MqttSnForwarder mqttSnForwarder;
  device_address client_network_address;
  PlaceholderNetworkContext clientPlaceholderContext;
  void *clientNetworkContext = &clientPlaceholderContext;

  device_address gateway_network_address;
  device_address mqtt_sn_gateway_address;
  PlaceholderNetworkContext gatewayPlaceholderContext;
  void *gatewayNetworkContext = &gatewayPlaceholderContext;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  NiceMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  NiceMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  NiceMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  NiceMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  virtual void SetUp() {
    globalClientNetworkMockObj = &clientNetworkMock;
    globalGatewayNetworkMockObj = &gatewayNetworkMock;

    globalMqttSnFixedSizeRingBufferMock = &defaultMqttSnFixedSizeRingBufferMock;

    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkReceiveBuffer,
                                                           &clientNetworkReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkSendBuffer,
                                                           &clientNetworkSendBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkReceiveBuffer,
                                                           &gatewayNetworkReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkSendBuffer,
                                                           &gatewayNetworkSendBuffer));
    globalMqttSnFixedSizeRingBufferMockMap = &mqttSnFixedSizeRingBufferMockMap;

    {
      device_address client_network_address({0, 0, 0, 0, 0, 0});
      this->client_network_address = client_network_address;

      device_address gateway_network_address({0, 0, 0, 0, 0, 0});
      device_address mqtt_sn_gateway_address({0, 0, 0, 0, 0, 0});
      this->gateway_network_address = gateway_network_address;
      this->mqtt_sn_gateway_address = mqtt_sn_gateway_address;
    }

    auto local_client_network_init = [](MqttSnClientNetworkInterface *n, void *context) -> int {
      n->client_network_init = mock_client_network_init;
      n->client_network_connect = mock_client_network_connect;
      n->client_network_disconnect = mock_client_network_disconnect;
      n->client_network_receive = mock_client_network_receive;
      n->client_network_send = mock_client_network_send;
      return 0;
    };

    // clientNetworkMock.DelegateToFake();
    EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnForwarder.clientNetwork, clientNetworkContext))
        .Times(1)
        .WillOnce(Invoke(local_client_network_init));

    auto local_gateway_network_init = [](MqttSnGatewayNetworkInterface *n, void *context) -> int {
      n->gateway_network_receive = mock_gateway_network_receive;
      n->gateway_network_send = mock_gateway_network_send;
      n->gateway_network_init = mock_gateway_network_init;
      n->gateway_network_connect = mock_gateway_network_connect;
      n->gateway_network_disconnect = mock_gateway_network_disconnect;
      return 0;
    };

    //gatewayNetworkMock.DelegateToFake();
    EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
        .Times(1)
        .WillOnce(Invoke(local_gateway_network_init));

    ASSERT_EQ(ClientNetworkInit(&mqttSnForwarder.clientNetwork, NULL,
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

  MqttSnForwarderTests() {}
  virtual ~MqttSnForwarderTests() {}
};

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H
