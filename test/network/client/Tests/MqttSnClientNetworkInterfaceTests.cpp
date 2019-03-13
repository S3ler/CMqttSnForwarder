//
// Created by bele on 26.02.19.
//

#include <deque>
#include <queue>
#include "MqttSnClientNetworkInterfaceTests.h"
// TODO probleme treten auf bei:
// we nee dsome identiy
// MessageSize < 2 && Identity => only 255 different clients can be destinquished by Id,
// this is crosschecked when the messageSize is >=2 and by the message counter (2 Message => 4xmessage with one message ID)
// Message

// send by client => received by MqttSnForwarder
// send by MqttSnForwarder => received by client
// send by clients => received by MqttSnForwarder => send back to clients
// send by MqttSnForwarder => received by clients => send back to MqttSnForwarder


TEST_P(MqttSnClientNetworkInterfaceTests, SendMultipleClientMultipleMessageVariableLength) {
  std::vector<CompareableMqttSnMessageData> expectedMockClientSnMessageDatas;
  std::vector<CompareableMqttSnMessageData> actualMockClientSnMessageDatas;

  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(toTestMessageCount * mockClients.size())
      .WillRepeatedly(Invoke(
          [this, &actualMockClientSnMessageDatas]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            CompareableMqttSnMessageData data(*receiveData, useIdentifier);
            actualMockClientSnMessageDatas.push_back(data);
            return 0;
          }
      ));

  for (uint16_t messageCount = 0; messageCount < toTestMessageCount; ++messageCount) {
    // we send message in RoundRobin for each client
    for (const auto &mockClient : mockClients) {
      CompareableMqttSnMessageData data(toTestMessageLength, mockClient->getIdentifier());
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
        *mockClient->getNetworkAddress() =
            getDeviceAddressFromNetworkContext(mockClient->getIdentifier(), clientNetworkContext);
      }
      CompareableMqttSnMessageData data(toTestMessageLength,
                                        mockClient->getNetworkAddress(),
                                        mockClient->getIdentifier());
      expectedMockClientSnMessageDatas.push_back(data);

      EXPECT_CALL((*mockClient->getMockClientNetworkReceiver()), receive_any_message(_, _, _))
          .WillRepeatedly(Invoke(
              [this]
                  (device_address *address,
                   uint8_t *data,
                   uint16_t data_length) -> void {
                CompareableMqttSnMessageData messageData(address, data, data_length, useIdentifier);
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
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));
}