//
// Created by SomeDude on 06.03.2019.
//

#ifndef CMQTTSNFORWARDER_ADDFORWARDINGHEADERTOCLIENTMESSAGESTESTS_H
#define CMQTTSNFORWARDER_ADDFORWARDINGHEADERTOCLIENTMESSAGESTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <forwarder/MqttSnForwarder.h>
#include <test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h>

using ::testing::Return;
using ::testing::Invoke;
using ::testing::StrictMock;
using ::testing::_;

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
    std::cout << std::endl;
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkReceiveBuffer,
                                                           &clientNetworkReceiveBuffer));

    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkSendBuffer,
                                                           &clientNetworkSendBuffer));

    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkReceiveBuffer,
                                                           &gatewayNetworkReceiveBuffer));

    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkSendBuffer,
                                                           &gatewayNetworkSendBuffer));

    std::cout << "make_pair: &mqttSnForwarder.clientNetworkReceiveBuffer: "
              << std::hex << &mqttSnForwarder.clientNetworkReceiveBuffer
              << " | &clientNetworkReceiveBuffer: "
              << std::hex << &clientNetworkReceiveBuffer << std::endl;
    std::cout << "make_pair: &mqttSnForwarder.clientNetworkSendBuffer: "
              << std::hex << &mqttSnForwarder.clientNetworkSendBuffer
              << " | &clientNetworkSendBuffer: "
              << std::hex << &clientNetworkSendBuffer << std::endl;
    std::cout << "make_pair: &mqttSnForwarder.gatewayNetworkReceiveBuffer: "
              << std::hex << &mqttSnForwarder.gatewayNetworkReceiveBuffer
              << " | &gatewayNetworkReceiveBuffer: "
              << std::hex << &gatewayNetworkReceiveBuffer << std::endl;
    std::cout << "make_pair: &mqttSnForwarder.gatewayNetworkSendBuffer: "
              << std::hex << &mqttSnForwarder.gatewayNetworkSendBuffer
              << " | &gatewayNetworkSendBuffer: "
              << std::hex << &gatewayNetworkSendBuffer << std::endl;

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

    globalMqttSnFixedSizeRingBufferMock = nullptr;
    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
  }

  virtual ~AddForwardingHeaderToClientMessagesTests() {}
  AddForwardingHeaderToClientMessagesTests() {}

};

#endif //CMQTTSNFORWARDER_ADDFORWARDINGHEADERTOCLIENTMESSAGESTESTS_H
