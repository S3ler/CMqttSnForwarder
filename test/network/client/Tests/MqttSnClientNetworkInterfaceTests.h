//
// Created by bele on 11.02.19.
//

#ifndef CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H
#define CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H

#include <gtest/gtest.h>
#include "../../../../forwarder/MqttSnClientNetworkInterface.h"
#include "../MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.h"
#include "../MockClientNetwork/MockClient/MockClient.h"
#include "../MockClientNetwork/MockClient/MockClientNetworkReceiver.h"
#include "../MockClientNetwork/MockClient/CompareableMqttSnMessageData.h"
#include "../TestConfigurations/MqttSnClientNetworkTestValueParameter.h"
#include "../../../../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"
#include "../TestConfigurations/MockClientConfiguration.h"
#include <list>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <gmock/gmock-nice-strict.h>

using testing::StrictMock;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class MqttSnClientNetworkInterfaceTests : public ::testing::TestWithParam<MqttSnClientNetworkTestValueParameter> {
 public:
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};
  ClientNetworkGatewayLooper clientNetworkGatewayLooper;
  MqttSnFixedSizeRingBuffer receiveBuffer = {0};
  MqttSnFixedSizeRingBuffer sendBuffer = {0};

 public:
  std::vector<std::shared_ptr<MockClient>> mockClients;
  std::vector<std::shared_ptr<MockClientNetworkReceiver>> mockClientNetworkReceiver;
  uint8_t *(*generateMessageData)(uint8_t *arr, uint16_t len) = nullptr;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;
  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;
  StrictMock<MqttSnFixedSizeRingBufferMock> mockReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> mockSendBuffer;

  uint16_t toTestMessageLength;
  uint16_t toTestMessageCount;
  bool useIdentifier;

  virtual void SetUp() {
    MqttSnClientNetworkTestValueParameter const &a = GetParam();
    MqttSnGatewayClientNetworkTestConfiguration p = a.mqttSnClientNetworkTestFixture;

    globalMqttSnFixedSizeRingBufferMock = &defaultMqttSnFixedSizeRingBufferMock;
    globalMqttSnFixedSizeRingBufferMockMap = &mqttSnFixedSizeRingBufferMockMap;
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&receiveBuffer, &mockReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&sendBuffer, &mockSendBuffer));

    ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                                &p.forwarderAddress,
                                p.clientNetworkContext,
                                p.clientNetworkInit), 0);
    ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, p.clientNetworkContext), 0);
    ASSERT_TRUE(clientNetworkGatewayLooper.startNetworkLoop(mqttSnClientNetworkInterface.client_network_receive,
                                                            &mqttSnClientNetworkInterface,
                                                            &receiveBuffer,
                                                            1000,
                                                            p.clientNetworkContext));

    this->generateMessageData = a.messageDataGenerator;

    for (auto &mockClientConfiguration : a.mockClientConfigurations) {
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

    toTestMessageLength = a.messageLength;
    toTestMessageCount = a.messageCount;
    useIdentifier = a.mqttSnClientNetworkTestFixture.useIdentifier;

  }

  virtual void TearDown() {

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

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  MqttSnClientNetworkInterfaceTests() {

  }

  virtual ~MqttSnClientNetworkInterfaceTests() {

  }

};

#endif //CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H
