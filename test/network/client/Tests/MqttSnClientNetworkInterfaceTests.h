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

using testing::NiceMock;
using testing::StrictMock;
using testing::AtLeast;
using testing::Return;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class MqttSnClientNetworkInterfaceTests : public ::testing::TestWithParam<MqttSnClientNetworkTestValueParameter> {
 public:
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};
  ClientNetworkGatewayLooper clientNetworkGatewayLooper;
  MqttSnFixedSizeRingBuffer receiveBuffer = {0};
  MqttSnFixedSizeRingBuffer sendBuffer = {0};
  void *clientNetworkContext = nullptr;

 public:
  std::vector<std::shared_ptr<MockClient>> mockClients;
  std::vector<std::shared_ptr<MockClientNetworkReceiver>> mockClientNetworkReceiver;
  uint8_t *(*generateMessageData)(uint8_t *arr, uint16_t len) = nullptr;
  device_address (*getDeviceAddressFromNetworkContext)(uint16_t identifier, void *context) = nullptr;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;
  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;
  StrictMock<MqttSnFixedSizeRingBufferMock> mockReceiveBuffer;
  NiceMock<MqttSnFixedSizeRingBufferMock> mockSendBuffer;

  uint16_t toTestMessageLength;
  uint16_t toTestMessageCount;
  bool useIdentifier;

  volatile std::atomic<uint32_t> counter;
  std::vector<CompareableMqttSnMessageData> expectedMockClientSnMessageDatas;
  std::vector<CompareableMqttSnMessageData> actualMockClientSnMessageDatas;


  virtual void SetUp() {
    counter=0;
    MqttSnClientNetworkTestValueParameter const &a = GetParam();
    MqttSnGatewayClientNetworkTestConfiguration p = a.mqttSnClientNetworkTestFixture;

    this->getDeviceAddressFromNetworkContext = p.getDeviceAddressFromMqttSnClientTcpNetworkContext;
    this->clientNetworkContext = p.clientNetworkContext;

    ON_CALL(mockSendBuffer, pop(&sendBuffer, _))
        .WillByDefault(Return(-1));

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
                                                            mqttSnClientNetworkInterface.client_network_send,
                                                            &mqttSnClientNetworkInterface,
                                                            &receiveBuffer,
                                                            &sendBuffer,
                                                            10,
                                                            clientNetworkContext));
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    this->generateMessageData = a.messageDataGenerator;

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
    clientNetworkContext = nullptr;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  MqttSnClientNetworkInterfaceTests() {

  }

  virtual ~MqttSnClientNetworkInterfaceTests() {

  }

  /*
  device_address getDeviceAddressFromNetworkContext(uint16_t identifier, void *context) {
    // TODO problem describe: we only have the address at the forwarder side, which is not equal to the combination ip+port on the client side
    // TODO this function must be a function pointer within the MqttSnGatewayClientNetworkTestConfiguration
    MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
    device_address peer_address = {0};
    if (clientTcpNetwork->client_socket[identifier] <= 0) {
      throw std::bad_exception();
    }
    getDeviceAddressFromFileDescriptor(clientTcpNetwork->client_socket[identifier], &peer_address);
    return peer_address;
  }
  */
};

#endif //CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H
