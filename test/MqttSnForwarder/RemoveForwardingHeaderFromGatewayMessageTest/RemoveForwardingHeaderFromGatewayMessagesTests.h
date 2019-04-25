//
// Created by SomeDude on 06.03.2019.
//

#ifndef CMQTTSNFORWARDER_REMOVEFORWARDINGHEADERFROMGATEWAYMESSAGESTESTS_H
#define CMQTTSNFORWARDER_REMOVEFORWARDINGHEADERFROMGATEWAYMESSAGESTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MqttSnForwarder.h>
#include <shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h>

using ::testing::Return;
using ::testing::Invoke;
using ::testing::StrictMock;
using ::testing::_;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class RemoveForwardingHeaderFromGatewayMessagesTests : public ::testing::Test {
 public:
  MqttSnForwarder mqttSnForwarder;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  MqttSnMessageData gatewayMessageData = {0};
  MqttSnMessageData clientMessageData = {0};

  virtual void SetUp() {

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

  }

  virtual void TearDown() {

  }

  virtual ~RemoveForwardingHeaderFromGatewayMessagesTests() {}
  RemoveForwardingHeaderFromGatewayMessagesTests() {}

};

#endif //CMQTTSNFORWARDER_REMOVEFORWARDINGHEADERFROMGATEWAYMESSAGESTESTS_H
