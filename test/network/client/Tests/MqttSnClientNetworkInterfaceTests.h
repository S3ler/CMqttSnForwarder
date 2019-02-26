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
#include "../MockClientNetwork/MockClient/MockClientMqttSnMessageData.h"
#include "gtest/gtest.h"
#include "../TestConfigurations/MqttSnClientNetworkTestValueParameter.h"
#include "../../../../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"
#include "../TestConfigurations/MockClientConfiguration.h"
#include <list>

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

  virtual void SetUp() {
    MqttSnFixedSizeRingBufferInit(&receiveBuffer);
    MqttSnFixedSizeRingBufferInit(&sendBuffer);

    MqttSnClientNetworkTestValueParameter const &a = GetParam();
    MqttSnGatewayClientNetworkTestConfiguration p = a.mqttSnClientNetworkTestFixture;

    ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                                &p.forwarderAddress,
                                p.clientNetworkContext,
                                p.clientNetworkInit), 0);
    ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, p.clientNetworkContext), 0);
    ASSERT_TRUE(clientNetworkGatewayLooper.startNetworkLoop(mqttSnClientNetworkInterface.client_receive,
                                                            &mqttSnClientNetworkInterface,
                                                            &receiveBuffer,
                                                            1000,
                                                            p.clientNetworkContext));

    this->generateMessageData = a.messageDataGenerator;

    std::vector<MockClientConfiguration> mockClientConfigurations = a.mockClientConfigurations;
    for (auto &mockClientConfiguration : mockClientConfigurations) {
      std::shared_ptr<MockClientNetworkReceiver> receiver(new MockClientNetworkReceiver);
      std::shared_ptr<MockClient> mockClient(new MockClient(mockClientConfiguration.identifier,
                                                            &mockClientConfiguration.address,
                                                            &p.forwarderAddress,
                                                            receiver.get()));
      mockClient->setNetworkAddress(&mockClientConfiguration.address);
      mockClient->setForwarderAddress(&p.forwarderAddress);
      mockClient->setMockClientNetworkInterface(mockClientConfiguration.mockClientNetworkInterface);

      ASSERT_TRUE(mockClient->start_loop());
      ASSERT_TRUE(mockClient->isNetworkConnected());

      mockClients.push_back(mockClient);
      mockClientNetworkReceiver.push_back(receiver);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

  }

  virtual void TearDown() {

    for (auto &mockClient : mockClients) {
      mockClient.get()->stop_loop();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    clientNetworkGatewayLooper.stopNetworkLoop();
    while (!clientNetworkGatewayLooper.isStopped) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ClientNetworkDisconnect(&mqttSnClientNetworkInterface, GetParam().mqttSnClientNetworkTestFixture.clientNetworkContext);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  MqttSnClientNetworkInterfaceTests() {

  }

  virtual ~MqttSnClientNetworkInterfaceTests() {

  }
};



#endif //CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H
