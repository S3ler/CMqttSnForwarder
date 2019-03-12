//
// Created by bele on 26.02.19.
//

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


TEST_P(MqttSnClientNetworkInterfaceTests, SendReceiveMultipleClientMultipleMessageVariableLength) {
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