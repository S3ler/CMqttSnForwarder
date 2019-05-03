//
// Created by SomeDude on 07.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOOPTESTS_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOOPTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <forwarder/MqttSnForwarder.h>
#include <test/shared/MockMqttSnClientNetwork/ClientNetworkMock.h>
#include <test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMock.h>
#include <test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h>
#include <test/shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h>

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::StrictMock;
using ::testing::NiceMock;
using ::testing::_;

extern ClientNetworkMock *globalClientNetworkMockObj;
extern GatewayNetworkMock *globalGatewayNetworkMockObj;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class MqttSnForwarderLoopTests : public ::testing::Test {
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

  int32_t clientNetworkSendTimeout = 0;
  int32_t clientNetworkReceiveTimeout = 0;

  int32_t gatewayNetworkSendTimeout = 0;
  int32_t gatewayNetworkReceiveTimeout = 0;

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

    EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
        .Times(1)
        .WillOnce(Invoke(local_gateway_network_init));

    ASSERT_EQ(ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                                &mqtt_sn_gateway_address,
                                &client_network_address,
                                clientNetworkContext,
                                mock_client_network_init), 0);
    EXPECT_EQ(GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                                 &mqtt_sn_gateway_address,
                                 &gateway_network_address,
                                 gatewayNetworkContext,
                                 mock_gateway_network_init), 0);

    EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(_))
        .WillRepeatedly(Return(1));
    EXPECT_CALL(clientNetworkSendBuffer, isEmpty(_))
        .WillRepeatedly(Return(1));
    EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(_))
        .WillRepeatedly(Return(1));
    EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(_))
        .WillRepeatedly(Return(1));

    ASSERT_EQ(MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext), 0);

    ON_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
        .WillByDefault(Return(1));
    ON_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
        .WillByDefault(Return(1));
    ON_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
        .WillByDefault(Return(1));
    ON_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
        .WillByDefault(Return(1));

  }
  virtual void TearDown() {

    globalClientNetworkMockObj = nullptr;
    globalGatewayNetworkMockObj = nullptr;

    globalMqttSnFixedSizeRingBufferMockMap = nullptr;

  }
  MqttSnForwarderLoopTests() {}
  virtual ~MqttSnForwarderLoopTests() {}
};

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERLOOPTESTS_H
