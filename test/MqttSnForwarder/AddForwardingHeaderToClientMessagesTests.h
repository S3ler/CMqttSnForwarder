//
// Created by SomeDude on 06.03.2019.
//

#ifndef CMQTTSNFORWARDER_ADDFORWARDINGHEADERTOCLIENTMESSAGESTESTS_H
#define CMQTTSNFORWARDER_ADDFORWARDINGHEADERTOCLIENTMESSAGESTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock-nice-strict.h>
#include <ClientNetworkMock.h>
#include <GatewayNetworkMock.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <MqttSnForwarder.h>

using ::testing::StrictMock;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class AddForwardingHeaderToClientMessagesTests : public ::testing::Test {
 public:

  MqttSnForwarder mqttSnForwarder;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  MqttSnMessageData clientMessageData = {0};
  MqttSnMessageData gatewayMessageData = {0};

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

  virtual ~AddForwardingHeaderToClientMessagesTests() {}
  AddForwardingHeaderToClientMessagesTests() {}

};

#endif //CMQTTSNFORWARDER_ADDFORWARDINGHEADERTOCLIENTMESSAGESTESTS_H
