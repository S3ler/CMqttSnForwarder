//
// Created by bele on 11.02.19.
//

#ifndef CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H
#define CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H

#include <gtest/gtest.h>
#include "../../../forwarder/MqttSnClientNetworkInterface.h"
#include "MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.h"
#include "MockClientNetwork/MockClient/MockClient.h"
#include "MockClientNetwork/MockClient/MockClientNetworkReceiver.h"
#include "MockClientNetwork/MockClient/MockClientMqttSnMessageData.h"
#include "../../../cmake-build-debug/googletest-src/googletest/include/gtest/gtest.h"
#include "TestConfigurations/MqttSnClientNetworkTestValueParameter.h"
#include "../../../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"
#include "MockClientNetwork/tcp/MockClientLinuxTcpNetworkImplementation.h"
#include "TestConfigurations/MockClientConfiguration.h"
#include <list>

class BaseMqttSnClientNetworkTests : public ::testing::TestWithParam<MqttSnClientNetworkTestValueParameter> {
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

  BaseMqttSnClientNetworkTests() {

  }

  virtual ~BaseMqttSnClientNetworkTests() {

  }
};

// TODO test what happens with more than 255 clients with 1 byte message and 2 messag
TEST_P(BaseMqttSnClientNetworkTests, SendReceiveMultipleClientMultipleMessageVariableLength) {

  MqttSnClientNetworkTestValueParameter const &p = GetParam();

  std::vector<MockClientMqttSnMessageData> expectedMockClientSnMessageDatas;

  uint16_t toTestMessageLength = p.messageLength;
  uint8_t maximumByteData[toTestMessageLength];
  generateMessageData(maximumByteData, toTestMessageLength);

  for (uint16_t messageCount = 0; messageCount < p.messageCount; ++messageCount) {
  // we send message in RoundRobin for each client
    for (const auto &mockClient : mockClients) {

      MqttSnMessageData toSendMessageData = {0};
      toSendMessageData.address = {0};
      toSendMessageData.data_length = toTestMessageLength;
      memcpy(toSendMessageData.data, maximumByteData, toSendMessageData.data_length);

      if (p.mqttSnClientNetworkTestFixture.useIdentifier) {
        // add identifier if we cannot identify clients by device_addresses during EXPECTED
        toSendMessageData.data[0] = (uint8_t) mockClient->getIdentifier() >> 0;
        if (toSendMessageData.data_length > 1) { // with more than 1 byte there is space for the msb of the identity
          toSendMessageData.data[1] = (uint8_t) mockClient->getIdentifier() >> 8;
        }
      }

      // send
      ASSERT_EQ(mockClient->send(toSendMessageData.data, toSendMessageData.data_length), toSendMessageData.data_length);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      // add the sent message to the expected messages
      MockClientMqttSnMessageData
          mockClientMqttSnMessageData(toSendMessageData, p.mqttSnClientNetworkTestFixture.useIdentifier);
      expectedMockClientSnMessageDatas.push_back(mockClientMqttSnMessageData);

    }

  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::vector<MockClientMqttSnMessageData> actualMockClientSnMessageDatas;
  MqttSnMessageData messageData;
  while (pop(&receiveBuffer, &messageData) == 0) {
    MockClientMqttSnMessageData
        actualMockClientMqttSnMessageData(messageData, p.mqttSnClientNetworkTestFixture.useIdentifier);
    actualMockClientSnMessageDatas.push_back(actualMockClientMqttSnMessageData);
  }

  EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));
}

MqttSnClientTcpNetwork clientNetworkContext = {0};
device_address forwarderAddress({127, 0, 0, 1, (uint8_t) ((uint16_t) 9999 >> 8), (uint8_t) ((uint16_t) 9999 >> 0)});
std::vector<std::shared_ptr<MockClientLinuxTcpNetworkImplementation>> mockClientTcpNetworkInterfaces;
MqttSnGatewayClientNetworkTestConfiguration mqttSnGatewayClientNetworkTestConfiguration(forwarderAddress,
                                                                                        &clientNetworkContext,
                                                                                        ClientLinuxTcpInit,
                                                                                        true);
device_address generateMockClientTcpNetworkAddress(uint16_t mockClientIdentifier) {
  // a zeroed device address tells the test set to use the given device address
  device_address mockClientNetworkAddress({0, 0, 0, 0, 0, 0});
  return mockClientNetworkAddress;
}

typedef device_address(*MockClientNetworkAddressGenerator)(uint16_t);
template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTestValueParameter> getParameterMqttSnClientNetworkTestTypeParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces) {
  ::std::vector<MqttSnClientNetworkTestValueParameter> result;

  uint16_t min_message_length = 1;
  uint16_t max_message_length = MAX_MESSAGE_LENGTH;

  uint16_t min_client_count = 1;
  uint16_t max_client_count = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS;

  uint16_t min_message_count = 1;
  uint16_t max_message_counter = 2;

  for (uint16_t client_count = min_client_count; client_count <= max_client_count; ++client_count) {

    for (uint16_t message_count = min_message_count; message_count <= max_message_counter; ++message_count) {
      for (uint16_t message_length = min_message_length; message_length <= max_message_length;
           message_length += 255 / 2) {
        std::vector<MockClientConfiguration> mockClientIdentifiers;

        for (uint16_t i = min_client_count; i <= client_count; ++i) {
          std::shared_ptr<MockClientNetworkInterfaceType>
              mockClientNetworkInterfaceType(new MockClientNetworkInterfaceType);
          mockClientNetworkInterfaces.push_back(mockClientNetworkInterfaceType);
          MockClientConfiguration
              mockClientIdentifier
              (generateNetworkAddress(client_count), client_count, mockClientNetworkInterfaceType.get());
          mockClientIdentifiers.push_back(mockClientIdentifier);
        }
        MqttSnClientNetworkTestValueParameter mqttSnClientNetworkTestTypeParameter
            (mockClientIdentifiers, message_count, message_length, mqttSnClientNetworkTestFixture);
        result.push_back(mqttSnClientNetworkTestTypeParameter);
      }
    }
  }
  return result;
}



::std::vector<MqttSnClientNetworkTestValueParameter>
    clientNetworkTestParameter = getParameterMqttSnClientNetworkTestTypeParameter<generateMockClientTcpNetworkAddress,
                                                                                  MockClientLinuxTcpNetworkImplementation>(
    mqttSnGatewayClientNetworkTestConfiguration,
    mockClientTcpNetworkInterfaces
);

struct PrintToStringMqttSnClientNetworkTestValueParameterParamName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = std::to_string(info.index);
    std::stringstream buffer;
    buffer << info.param;
    result.append(buffer.str());
    return result;
  }
};

INSTANTIATE_TEST_SUITE_P(SendReceiveTests,
                         BaseMqttSnClientNetworkTests,
                         ::testing::ValuesIn(clientNetworkTestParameter.begin(), clientNetworkTestParameter.end()),
                         PrintToStringMqttSnClientNetworkTestValueParameterParamName());

#endif //CMQTTSNFORWARDER_BASEMQTTSNCLIENTNETWORKTESTS_H
