//
// Created by SomeDude on 07.03.2019.
//

#ifndef CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOGATEWAYTESTS_H
#define CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOGATEWAYTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <forwarder/MqttSnForwarder.h>
#include <test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMock.h>
#include <test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h>
#include <test/shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h>

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::StrictMock;
using ::testing::Invoke;
using ::testing::_;

extern GatewayNetworkMock *globalGatewayNetworkMockObj;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class SendBufferedMessagesToGatewayTests : public testing::Test {
 public:
  MqttSnForwarder mqttSnForwarder;

  StrictMock<GatewayNetworkMock> gatewayNetworkMock;

  device_address gateway_network_address;
  device_address mqtt_sn_gateway_address;
  PlaceholderNetworkContext gatewayNetworkPlaceholderContext;
  void *gatewayNetworkContext = &gatewayNetworkPlaceholderContext;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  MqttSnMessageData clientMessageData;
  MqttSnMessageData gatewayMessageData;

  int32_t CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT = 0;
  int32_t CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT = 0;

  int32_t GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT = 0;
  int32_t GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT = 0;

  virtual void SetUp() {
    memset(&mqtt_sn_gateway_address, 0, sizeof(device_address));
    memset(&gateway_network_address, 0, sizeof(device_address));

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

    globalGatewayNetworkMockObj = &gatewayNetworkMock;

    mqttSnForwarder.gatewayNetwork.gateway_network_send = mock_gateway_network_send;
    mqttSnForwarder.gatewayNetworkContext = gatewayNetworkContext;

    ON_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
        .WillByDefault(Return(1));
    ON_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
        .WillByDefault(Return(1));
  }

  virtual void TearDown() {
    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
    globalMqttSnFixedSizeRingBufferMock = nullptr;
    globalGatewayNetworkMockObj = nullptr;
  }

  SendBufferedMessagesToGatewayTests() {};
  virtual ~  SendBufferedMessagesToGatewayTests() {}

};

#endif //CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOGATEWAYTESTS_H
