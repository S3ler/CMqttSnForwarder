//
// Created by SomeDude on 25.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACEMESSAGEDEFRAGMENTATIONTESTS_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACEMESSAGEDEFRAGMENTATIONTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock-nice-strict.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <MockClientNetwork/MockForwardLooper/ClientNetworkGatewayLooper.h>
#include <MockClientNetwork/MockClient/MockClient.h>
#include "MqttSnClientNetworkTcpNetworkDefragmentationTestParameter.h"
#include <ComparableClientMqttSnMessageData.h>
#include <list>

using testing::NiceMock;
using testing::StrictMock;
using testing::AtLeast;
using testing::Return;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class MqttSnClientNetworkInterfaceMessageDefragmentationTests :
    public ::testing::TestWithParam<MqttSnClientNetworkTcpNetworkDefragmentationTestParameter> {
 public:
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};
  ClientNetworkGatewayLooper clientNetworkGatewayLooper;
  MqttSnFixedSizeRingBuffer receiveBuffer = {0};
  MqttSnFixedSizeRingBuffer sendBuffer = {0};
  void *clientNetworkContext = nullptr;

 public:
  std::vector<std::shared_ptr<MockClient>> mockClients;
  std::vector<std::shared_ptr<MockClientNetworkReceiver>> mockClientNetworkReceiver;
  device_address (*getDeviceAddressFromNetworkContext)(uint16_t identifier, void *context) = nullptr;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;
  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;
  StrictMock<MqttSnFixedSizeRingBufferMock> mockReceiveBuffer;
  NiceMock<MqttSnFixedSizeRingBufferMock> mockSendBuffer;

  uint16_t toTestMessageLength;
  uint16_t toTestMessageCount;
  uint16_t toTestPacketSize;
  bool useIdentifier;

  volatile std::atomic<uint32_t> counter;
  std::vector<ComparableClientMqttSnMessageData> expectedMockClientSnMessageDatas;
  std::vector<ComparableClientMqttSnMessageData> actualMockClientSnMessageDatas;

  std::list<ComparableClientMqttSnMessageData> forwarderMqttSnMessageDataBuffer;

  virtual void SetUp() {
    counter = 0;
    ASSERT_TRUE(false) << "Test should fail!";

    MqttSnClientNetworkTcpNetworkDefragmentationTestParameter const &a = GetParam();
    MqttSnGatewayClientNetworkTestConfiguration p = a.mqttSnClientNetworkTestFixture;

    this->getDeviceAddressFromNetworkContext = p.getDeviceAddressFromMqttSnClientTcpNetworkContext;
    this->clientNetworkContext = p.clientNetworkContext;

    toTestMessageLength = a.messageLength;
    toTestMessageCount = a.messageCount;
    toTestPacketSize = a.packetSize;
    useIdentifier = p.useIdentifier;

    if (toTestMessageLength < 2 |
        useIdentifier && toTestMessageLength < (sizeof(mockClients[0]->getIdentifier())) + 1) {
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

    ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                                &p.forwarderAddress,
                                p.clientNetworkContext,
                                p.clientNetworkInit), 0);
    ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, p.clientNetworkContext), 0);
    ASSERT_TRUE(clientNetworkGatewayLooper.startNetworkLoop(mqttSnClientNetworkInterface.client_network_receive,
                                                            mqttSnClientNetworkInterface.client_network_send,
                                                            &mqttSnClientNetworkInterface,
                                                            &receiveBuffer,
                                                            &sendBuffer,
                                                            10,
                                                            clientNetworkContext));
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    // this->generateMessageData = a.messageDataGenerator;

    // connect each mockClient to the network - one after another. not multiple at once
    for (auto mockClientConfiguration : a.mockClientConfigurations) {
      std::shared_ptr<MockClientNetworkReceiver> receiver(new MockClientNetworkReceiver);
      std::shared_ptr<MockClient> mockClient(new MockClient(mockClientConfiguration.identifier,
                                                            &mockClientConfiguration.address,
                                                            &p.forwarderAddress,
                                                            mockClientConfiguration.mockClientNetworkInterface,
                                                            receiver.get()));

      ASSERT_TRUE(mockClient->start_loop());
      while (!mockClient->isNetworkConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }

      mockClients.push_back(mockClient);
      mockClientNetworkReceiver.push_back(receiver);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  }

  virtual void TearDown() {

    if (toTestMessageLength < 2 |
        useIdentifier && toTestMessageLength < (sizeof(mockClients[0]->getIdentifier())) + 1) {
      GTEST_SKIP();
    }

    for (auto &mockClient : mockClients) {
      mockClient->stop_loop();
    }

    for (auto &mockClient : mockClients) {
      while (!mockClient->getDone()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }

    clientNetworkGatewayLooper.stopNetworkLoop();
    while (!clientNetworkGatewayLooper.isStopped) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ClientNetworkDisconnect(&mqttSnClientNetworkInterface,
                            GetParam().mqttSnClientNetworkTestFixture.clientNetworkContext);

    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
    globalMqttSnFixedSizeRingBufferMock = nullptr;
    clientNetworkContext = nullptr;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  MqttSnClientNetworkInterfaceMessageDefragmentationTests() {

  }

  virtual ~MqttSnClientNetworkInterfaceMessageDefragmentationTests() {

  }

};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACEMESSAGEDEFRAGMENTATIONTESTS_H
