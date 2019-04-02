//
// Created by SomeDude on 23.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACEMESSAGEDEFRAGMENTATIONTESTS_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACEMESSAGEDEFRAGMENTATIONTESTS_H

#include <gtest/gtest.h>
#include <MockForwardLooper/MockForwarderGatewayNetworkLooper.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <gmock/gmock-nice-strict.h>
#include <list>
#include "MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter.h"

using testing::Return;
using testing::AtLeast;
using testing::StrictMock;
using testing::NiceMock;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class MqttSnGatewayNetworkInterfaceMessageDefragmentationTests
    : public ::testing::TestWithParam<MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter> {
 public:
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface = {0};
  MockForwarderGatewayNetworkLooper gatewayNetworkForwarderLooper;
  MqttSnFixedSizeRingBuffer receiveBuffer = {0};
  MqttSnFixedSizeRingBuffer sendBuffer = {0};
  void *gatewayNetworkContext = nullptr;
  device_address mqttSnGatewayDeviceAddress = {0};

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
  uint16_t packetSize;
  bool useIdentifier;

 public:
  std::vector<ComparableGatewayMqttSnMessageData> expectedMockGatewayMqttSnMessageDatas;
  std::vector<ComparableGatewayMqttSnMessageData> actualMockGatewaySnMessageDatas;
  std::list<ComparableGatewayMqttSnMessageData> forwarderMqttSnMessageDataBuffer;

  virtual void SetUp() {

    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter const &a = GetParam();
    MqttSnForwarderGatewayNetworkTestConfiguration p = a.mqttSnForwarderGatewayNetworkTestConfiguration;

    this->getDeviceAddressFromNetworkContext = p.getDeviceAddressFromMqttSnGatewayNetworkContext;
    this->gatewayNetworkContext = p.gatewayNetworkContext;

    toTestMessageLength = a.messageLength;
    toTestMessageCount = a.messageCount;
    packetSize = a.packetSize;
    useIdentifier = p.useIdentifier;

    mqttSnGatewayDeviceAddress = a.gatewayToConnectAddress;

    if (toTestMessageLength < 2 |
        useIdentifier && toTestMessageLength < (sizeof(mockGateway->getIdentifier())) + 1) {
      GTEST_SKIP();
    }

    ON_CALL(mockSendBuffer, pop(&sendBuffer, _))
        .WillByDefault(Return(-1));

    globalMqttSnFixedSizeRingBufferMock = &defaultMqttSnFixedSizeRingBufferMock;
    globalMqttSnFixedSizeRingBufferMockMap = &mqttSnFixedSizeRingBufferMockMap;
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&receiveBuffer, &mockReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&sendBuffer, &mockSendBuffer));

    //MqttSnFixedSizeRingBufferInit(&receiveBuffer);
    //MqttSnFixedSizeRingBufferInit(&sendBuffer);

    if (a.mockGatewayConfigurations.size() > 1) {
      // TODO
      throw std::exception();
    }
    if (a.searchGateway) {
      // TODO
      throw std::exception();
    }
    for (auto mockGatewayConfiguration : a.mockGatewayConfigurations) {
      std::shared_ptr<MockGatewayNetworkReceiver> receiver(new MockGatewayNetworkReceiver);
      std::shared_ptr<MockGateway> mockGateway(new MockGateway(mockGatewayConfiguration.gatewayIdentifier,
                                                               &mockGatewayConfiguration.address,
                                                               &mqttSnGatewayDeviceAddress,
                                                               mockGatewayConfiguration.mockGatewayNetworkInterface,
                                                               receiver.get()));
      ASSERT_TRUE(mockGateway->start_loop());
      ASSERT_TRUE(mockGateway->isNetworkConnected());
      this->mockGateway = mockGateway;
      this->mockGatewayNetworkReceiver = receiver;
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (a.searchGateway) {
      ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                                   &p.forwarderAddress,
                                   NULL,
                                   p.gatewayNetworkContext,
                                   p.gateway_network_init), 0);
    } else {
      ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                                   &p.forwarderAddress,
                                   &mqttSnGatewayDeviceAddress,
                                   p.gatewayNetworkContext,
                                   p.gateway_network_init), 0);
    }
    ASSERT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, p.gatewayNetworkContext), 0);
    ASSERT_TRUE(gatewayNetworkForwarderLooper.startNetworkLoop(mqttSnGatewayNetworkInterface.gateway_receive,
                                                               mqttSnGatewayNetworkInterface.gateway_send,
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

    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
    globalMqttSnFixedSizeRingBufferMock = nullptr;
    gatewayNetworkContext = nullptr;

    mockGateway->stop_loop();
    while (!mockGateway->getDone()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  MqttSnGatewayNetworkInterfaceMessageDefragmentationTests() {}
  virtual ~MqttSnGatewayNetworkInterfaceMessageDefragmentationTests() {}
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACEMESSAGEDEFRAGMENTATIONTESTS_H
