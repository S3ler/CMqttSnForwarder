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


// TODO test what happens with more than 255 clients with 1 byte message and 2 messag
TEST_P(MqttSnClientNetworkInterfaceTests, SendReceiveMultipleClientMultipleMessageVariableLength) {

  MqttSnClientNetworkTestValueParameter const &p = GetParam();

  std::vector<MockClientMqttSnMessageData> expectedMockClientSnMessageDatas;

  uint16_t toTestMessageLength = p.messageLength;
  uint8_t maximumByteData[toTestMessageLength];
  this->generateMessageData(maximumByteData, toTestMessageLength);

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

  // TODO mock den RingBuffer, dann kannst du dir das hier sparen, und über EXPECTED_THAT auf den MockRingBuffer regeln
  std::vector<MockClientMqttSnMessageData> actualMockClientSnMessageDatas;
  MqttSnMessageData messageData;
  while (pop(&receiveBuffer, &messageData) == 0) {
    MockClientMqttSnMessageData
        actualMockClientMqttSnMessageData(messageData, p.mqttSnClientNetworkTestFixture.useIdentifier);
    actualMockClientSnMessageDatas.push_back(actualMockClientMqttSnMessageData);
  }

  EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));
}