//
// Created by bele on 26.02.19.
//

#include <deque>
#include <queue>
#include "MqttSnClientNetworkInterfaceTests.h"

TEST_P(MqttSnClientNetworkInterfaceTests, SendMultipleClientMultipleMessageVariableLength) {
  std::vector<ComparableClientMqttSnMessageData> expectedMockClientSnMessageDatas;
  std::vector<ComparableClientMqttSnMessageData> actualMockClientSnMessageDatas;

  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(toTestMessageCount * mockClients.size())
      .WillRepeatedly(Invoke(
          [this, &actualMockClientSnMessageDatas]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            ComparableClientMqttSnMessageData data(*receiveData, useIdentifier);
            actualMockClientSnMessageDatas.push_back(data);
            return 0;
          }
      ));

  for (uint16_t messageCount = 0; messageCount < toTestMessageCount; ++messageCount) {
    // we send message in RoundRobin for each client
    for (const auto &mockClient : mockClients) {
      ComparableClientMqttSnMessageData data(toTestMessageLength,
                                             mockClient->getClientDeviceAddress(),
                                             mockClient->getIdentifier());
      expectedMockClientSnMessageDatas.push_back(data);

      ASSERT_EQ(mockClient->send(&data), data.data_length);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));
}

TEST_P(MqttSnClientNetworkInterfaceTests, ReceiveMultipleClientMultipleMessageVariableLength) {
  ASSERT_TRUE(clientNetworkGatewayLooper.pauseLoop());

  for (uint16_t messageCount = 0; messageCount < toTestMessageCount; ++messageCount) {
    // we send message in RoundRobin for each client
    for (const auto &mockClient : mockClients) {
      if (useIdentifier) {
        *mockClient->getClientDeviceAddress() =
            getDeviceAddressFromNetworkContext(mockClient->getIdentifier(), clientNetworkContext);
      }
      ComparableClientMqttSnMessageData data(toTestMessageLength,
                                             mockClient->getClientDeviceAddress(),
                                             mockClient->getIdentifier());
      expectedMockClientSnMessageDatas.push_back(data);

      EXPECT_CALL((*mockClient->getMockClientNetworkReceiver()), receive_any_message(_, _, _))
          .WillRepeatedly(Invoke(
              [this]
                  (device_address *address,
                   uint8_t *data,
                   uint16_t data_length) -> void {
                ComparableClientMqttSnMessageData messageData(address, data, data_length, useIdentifier);
                actualMockClientSnMessageDatas.push_back(messageData);
              }
          ));
    }
  }

  EXPECT_CALL(mockSendBuffer, pop(&sendBuffer, _))
      .Times(AtLeast(mockClients.size() * toTestMessageCount))
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *queue,
               MqttSnMessageData *messageData) -> int {
            memset(messageData, 0, sizeof(MqttSnMessageData));
            if (counter < expectedMockClientSnMessageDatas.size()) {
              messageData->address = expectedMockClientSnMessageDatas[counter].address;
              messageData->data_length = expectedMockClientSnMessageDatas[counter].data_length;
              memcpy(&messageData->data,
                     &expectedMockClientSnMessageDatas[counter].data[0],
                     expectedMockClientSnMessageDatas[counter].data_length);
              counter++;
              return 0;
            }
            return -1;
          }
      ));

  ASSERT_TRUE(clientNetworkGatewayLooper.resumeLoop());
  //   std::this_thread::sleep_for(std::chrono::milliseconds(mockClients.size() * toTestMessageCount * 1000));
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));
}

TEST_P(MqttSnClientNetworkInterfaceTests, SendToClientsReceiveOnForwarderTests) {
  // forwarder sends data to clients
  // clients send data back to forwarder
  ASSERT_TRUE(clientNetworkGatewayLooper.pauseLoop());

  for (uint16_t messageCount = 0; messageCount < toTestMessageCount; ++messageCount) {
    // we send message in RoundRobin for each client
    for (const auto &mockClient : mockClients) {
      if (useIdentifier) {
        *mockClient->getClientDeviceAddress() =
            getDeviceAddressFromNetworkContext(mockClient->getIdentifier(), clientNetworkContext);
      }
      ComparableClientMqttSnMessageData data(toTestMessageLength,
                                             mockClient->getClientDeviceAddress(),
                                             mockClient->getIdentifier());
      expectedMockClientSnMessageDatas.push_back(data);

      EXPECT_CALL((*mockClient->getMockClientNetworkReceiver()), receive_any_message(_, _, _))
          .WillRepeatedly(Invoke(
              [&mockClient]
                  (device_address *address,
                   uint8_t *data,
                   uint16_t data_length) -> void {
                ASSERT_EQ(mockClient->send(address, data, data_length), data_length);
              }
          ));
    }
  }

  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(toTestMessageCount * mockClients.size())
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            ComparableClientMqttSnMessageData data(*receiveData, useIdentifier);
            actualMockClientSnMessageDatas.push_back(data);
            return 0;
          }
      ));

  EXPECT_CALL(mockSendBuffer, pop(&sendBuffer, _))
      .Times(AtLeast(toTestMessageCount * mockClients.size()))
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *queue,
               MqttSnMessageData *messageData) -> int {
            memset(messageData, 0, sizeof(MqttSnMessageData));
            if (counter < expectedMockClientSnMessageDatas.size()) {
              messageData->address = expectedMockClientSnMessageDatas[counter].address;
              messageData->data_length = expectedMockClientSnMessageDatas[counter].data_length;
              memcpy(&messageData->data,
                     &expectedMockClientSnMessageDatas[counter].data[0],
                     expectedMockClientSnMessageDatas[counter].data_length);
              counter++;
              return 0;
            }
            return -1;
          }
      ));

  ASSERT_TRUE(clientNetworkGatewayLooper.resumeLoop());
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));

  // list of to send messages to clients / expected on server
  // list of receive message on forwarder / actual on forwarder

  // clients immediately send back
}

TEST_P(MqttSnClientNetworkInterfaceTests, ReceiveOnForwarderSendToClientTests) {
  // list of to send message to forwarder / expected on clients
  // list of receive message on clients / actual on clients
  ASSERT_TRUE(clientNetworkGatewayLooper.pauseLoop());

  // forwarder "immediately" sends back
  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(toTestMessageCount * mockClients.size())
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            ComparableClientMqttSnMessageData data(*receiveData, useIdentifier);
            forwarderMqttSnMessageDataBuffer.push_back(data);
            return 0;
          }
      ));

  EXPECT_CALL(mockSendBuffer, pop(&sendBuffer, _))
      .Times(AtLeast(toTestMessageCount * mockClients.size()))
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *queue,
               MqttSnMessageData *messageData) -> int {
            memset(messageData, 0, sizeof(MqttSnMessageData));
            if (!forwarderMqttSnMessageDataBuffer.empty()) {
              messageData->address = forwarderMqttSnMessageDataBuffer.front().address;
              messageData->data_length = forwarderMqttSnMessageDataBuffer.front().data_length;
              memcpy(&messageData->data,
                     &forwarderMqttSnMessageDataBuffer.front().data[0],
                     forwarderMqttSnMessageDataBuffer.front().data_length);
              forwarderMqttSnMessageDataBuffer.pop_front();
              return 0;
            }
            return -1;
          }
      ));

  for (uint16_t messageCount = 0; messageCount < toTestMessageCount; ++messageCount) {
    // we send message in RoundRobin for each client
    for (const auto &mockClient : mockClients) {
      ComparableClientMqttSnMessageData data(toTestMessageLength,
                                             mockClient->getClientDeviceAddress(),
                                             mockClient->getIdentifier());
      expectedMockClientSnMessageDatas.push_back(data);

      EXPECT_CALL((*mockClient->getMockClientNetworkReceiver()), receive_any_message(_, _, _))
          .WillRepeatedly(Invoke(
              [this]
                  (device_address *address,
                   uint8_t *data,
                   uint16_t data_length) -> void {
                ComparableClientMqttSnMessageData messageData(address, data, data_length, useIdentifier);
                actualMockClientSnMessageDatas.push_back(messageData);
              }
          ));

      ASSERT_EQ(mockClient->send(&data), data.data_length);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  }

  ASSERT_TRUE(clientNetworkGatewayLooper.resumeLoop());
  std::this_thread::sleep_for(std::chrono::milliseconds(15000));

  EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));

}
