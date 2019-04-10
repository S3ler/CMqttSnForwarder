//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock-nice-strict.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include "../TestConfigurations/MqttSnGatewayNetworkValueParameter.h"
#include "../../../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../../../../forwarder/MqttSnGatewayNetworkInterface.h"
#include "../TestConfigurations/MqttSnForwarderGatewayNetworkTestConfiguration.h"
#include "TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h"
#include "../TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.cpp"
#include "../MockForwardLooper/MockForwarderGatewayNetworkLooper.h"
#include <list>

using testing::Return;
using testing::AtLeast;
using testing::StrictMock;
using testing::NiceMock;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class MqttSnGatewayNetworkInterfaceSendReceiveTests
    : public ::testing::TestWithParam<MqttSnGatewayNetworkValueParameter> {
 public:
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface = {0};
  MockForwarderGatewayNetworkLooper gatewayNetworkForwarderLooper;
  MqttSnFixedSizeRingBuffer receiveBuffer = {0};
  MqttSnFixedSizeRingBuffer sendBuffer = {0};
  void *gatewayNetworkContext = nullptr;
  device_address gatewayToConnectAddress = {0};

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;
  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;
  StrictMock<MqttSnFixedSizeRingBufferMock> mockReceiveBuffer;
  NiceMock<MqttSnFixedSizeRingBufferMock> mockSendBuffer;

 public:
  std::shared_ptr<MockGateway> mockGateway;
  std::shared_ptr<MockGatewayNetworkReceiver> mockGatewayNetworkReceiver;
  device_address (*getDeviceAddressFromNetworkContext)(uint16_t identifier, void *context) = nullptr;

 public:
  uint16_t toTestMessageLength;
  uint16_t toTestMessageCount;
  bool useIdentifier;

 public:
  std::vector<ComparableGatewayMqttSnMessageData> expectedMockGatewayMqttSnMessageDatas;
  std::vector<ComparableGatewayMqttSnMessageData> actualMockGatewaySnMessageDatas;
  std::list<ComparableGatewayMqttSnMessageData> forwarderMqttSnMessageDataBuffer;

  virtual void SetUp() {

    MqttSnGatewayNetworkValueParameter const &a = GetParam();
    MqttSnForwarderGatewayNetworkTestConfiguration p = a.mqttSnForwarderGatewayNetworkTestConfiguration;

    this->getDeviceAddressFromNetworkContext = p.getDeviceAddressFromMqttSnGatewayNetworkContext;
    this->gatewayNetworkContext = p.gatewayNetworkContext;

    toTestMessageLength = a.messageLength;
    toTestMessageCount = a.messageCount;
    useIdentifier = p.useIdentifier;
    gatewayToConnectAddress = a.gatewayToConnectAddress;

    if (toTestMessageLength < 2 |
        useIdentifier && toTestMessageLength < (sizeof(mockGateway->getIdentifier())) + 1) {
      GTEST_SKIP();
    }

    ON_CALL(mockSendBuffer, pop(&sendBuffer, _))
        .WillByDefault(Return(-1));
    ON_CALL(mockSendBuffer, put(&sendBuffer, _))
        .WillByDefault(Return(-1));

    ON_CALL(mockReceiveBuffer, pop(&receiveBuffer, _))
        .WillByDefault(Return(-1));
    ON_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
        .WillByDefault(Return(-1));

    // std::cout << "&sendBuffer: 0x" << std::hex << (uintptr_t) &sendBuffer << std::endl;
    // std::cout << "&receiveBuffer: 0x" << std::hex << (uintptr_t) &receiveBuffer << std::endl;

    globalMqttSnFixedSizeRingBufferMock = &defaultMqttSnFixedSizeRingBufferMock;
    globalMqttSnFixedSizeRingBufferMockMap = &mqttSnFixedSizeRingBufferMockMap;
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&receiveBuffer, &mockReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&sendBuffer, &mockSendBuffer));



    //MqttSnFixedSizeRingBufferInit(&receiveBuffer);
    //MqttSnFixedSizeRingBufferInit(&sendBuffer);

    if (a.searchGateway) {
      // TODO
      throw std::exception();
    }

    // TODO exchange later with:
    /*
    for (auto mockGatewayConfiguration : a.mockGatewayConfigurations) {
      std::shared_ptr<MockGatewayNetworkReceiver> receiver(new MockGatewayNetworkReceiver);
      std::shared_ptr<MockGateway> mockGateway(new MockGateway(mockGatewayConfiguration.gatewayIdentifier,
                                                               &mockGatewayConfiguration.address,
                                                               mockGatewayConfiguration.mockGatewayNetworkInterface,
                                                               receiver.get()));
      ASSERT_TRUE(mockGateway->start_loop());
      ASSERT_TRUE(mockGateway->isNetworkConnected());
      this->mockGateway = mockGateway;
      this->mockGatewayNetworkReceiver = receiver;
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    */
    {
      auto mockGatewayConfiguration = a.mockGatewayConfigurations.front();
      std::shared_ptr<MockGatewayNetworkReceiver> receiver(new MockGatewayNetworkReceiver);
      std::shared_ptr<MockGateway> mockGateway(new MockGateway(mockGatewayConfiguration.gatewayIdentifier,
                                                               &mockGatewayConfiguration.address,
                                                               &p.forwarderAddress,
                                                               mockGatewayConfiguration.mockGatewayNetworkInterface,
                                                               receiver.get()));
      ASSERT_TRUE(mockGateway->start_loop());
      ASSERT_TRUE(mockGateway->isNetworkConnected());
      this->mockGateway = mockGateway;
      this->mockGatewayNetworkReceiver = receiver;
      std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (a.searchGateway) {
      ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                                   NULL,
                                   &p.forwarderAddress,
                                   p.gatewayNetworkContext,
                                   p.gateway_network_init), 0);
    } else {
      ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                                   &gatewayToConnectAddress,
                                   &p.forwarderAddress,
                                   p.gatewayNetworkContext,
                                   p.gateway_network_init), 0);
    }
    ASSERT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, p.gatewayNetworkContext), 0);
    ASSERT_TRUE(gatewayNetworkForwarderLooper.startNetworkLoop(mqttSnGatewayNetworkInterface.gateway_network_receive,
                                                               mqttSnGatewayNetworkInterface.gateway_network_send,
                                                               &mqttSnGatewayNetworkInterface,
                                                               &receiveBuffer,
                                                               &sendBuffer,
                                                               1000,
                                                               p.gatewayNetworkContext));
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  }

  virtual void TearDown() {

    if (toTestMessageLength < 2 |
        useIdentifier && toTestMessageLength < (sizeof(mockGateway->getIdentifier())) + 1) {
      GTEST_SKIP();
    }

    // TODO Teardown
    gatewayNetworkForwarderLooper.stopNetworkLoop();
    while (!gatewayNetworkForwarderLooper.isStopped) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    GatewayNetworkDisconnect(&mqttSnGatewayNetworkInterface,
                             GetParam().mqttSnForwarderGatewayNetworkTestConfiguration.gatewayNetworkContext);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    mockGateway->stop_loop();
    while (!mockGateway->getDone()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));


    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
    globalMqttSnFixedSizeRingBufferMock = nullptr;
    gatewayNetworkContext = nullptr;

  }

  MqttSnGatewayNetworkInterfaceSendReceiveTests() {

  }

  virtual ~MqttSnGatewayNetworkInterfaceSendReceiveTests() {

  }
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
