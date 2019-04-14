//
// Created by SomeDude on 07.03.2019.
//

#ifndef CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOCLIENTSTESTS_H
#define CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOCLIENTSTESTS_H

#include <gtest/gtest.h>

#include <ClientNetworkMock.h>
#include <gmock/gmock-nice-strict.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <MqttSnForwarder.h>
#include "../shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::StrictMock;
using ::testing::_;
using ::testing::Invoke;

extern ClientNetworkMock *globalClientNetworkMockObj;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class SendBufferedMessagesToClientsTests : public ::testing::Test {

 public:
  MqttSnForwarder mqttSnForwarder;

  StrictMock<ClientNetworkMock> clientNetworkMock;
  device_address client_network_address;
  PlaceholderNetworkContext clientNetworkPlaceholderContext;
  void *clientNetworkContext = &clientNetworkPlaceholderContext;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  virtual void SetUp() {
    device_address client_network_address({0, 0, 0, 0, 0, 0});
    this->client_network_address = client_network_address;

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

    globalClientNetworkMockObj = &clientNetworkMock;


    mqttSnForwarder.clientNetwork.client_network_send = mock_client_network_send;
    mqttSnForwarder.clientNetworkContext = clientNetworkContext;

    ON_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
        .WillByDefault(Return(1));
    ON_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
        .WillByDefault(Return(1));
  }
  virtual void TearDown() {
    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
    globalMqttSnFixedSizeRingBufferMock = nullptr;
    globalClientNetworkMockObj = nullptr;
  }



  SendBufferedMessagesToClientsTests() {}
  virtual ~SendBufferedMessagesToClientsTests() {}
};

#endif //CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOCLIENTSTESTS_H
