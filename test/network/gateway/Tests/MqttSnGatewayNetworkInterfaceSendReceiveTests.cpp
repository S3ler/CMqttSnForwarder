//
// Created by bele on 27.02.19.
//

#include "MqttSnGatewayNetworkInterfaceSendReceiveTests.h"

// send by MqttSnForwarder => received by gateway
TEST_P(MqttSnGatewayNetworkInterfaceSendReceiveTests, SendToGateway) {
  ASSERT_TRUE(gatewayNetworkForwarderLooper.pauseLoop());

  for (uint16_t messageCounter = 0; messageCounter < toTestMessageCount; ++messageCounter) {
    if (useIdentifier) {
      *mockGateway->getGatewayDeviceAddress() = getDeviceAddressFromNetworkContext(mockGateway->getIdentifier(),
                                                                                   gatewayNetworkContext);
    }
    ComparableGatewayMqttSnMessageData data(toTestMessageLength,
                                            mockGateway->getGatewayDeviceAddress(),
                                            mockGateway->getIdentifier(),
                                            useIdentifier);
    expectedMockGatewayMqttSnMessageDatas.push_back(data);
    forwarderMqttSnMessageDataBuffer.push_back(data);
  }

  ON_CALL(mockSendBuffer, pop(&sendBuffer, _))
      .WillByDefault(Return(-1));
  EXPECT_CALL(mockSendBuffer, pop(&sendBuffer, _))
      .Times(AtLeast(toTestMessageCount))
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
  EXPECT_CALL(*mockGateway->getReceiver(), receive_any_message(_, _, _))
      .Times(toTestMessageCount)
      .WillRepeatedly(Invoke(
          [this]
              (device_address *address,
               uint8_t *data,
               uint16_t data_length) -> void {
            ComparableGatewayMqttSnMessageData
                comparableGatewayMqttSnMessageData(address, data, data_length, useIdentifier);
            actualMockGatewaySnMessageDatas.push_back(comparableGatewayMqttSnMessageData);
          }
      ));

  ASSERT_TRUE(gatewayNetworkForwarderLooper.resumeLoop());
  std::this_thread::sleep_for(std::chrono::milliseconds(toTestMessageCount * 1500));
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  EXPECT_THAT(actualMockGatewaySnMessageDatas,
              testing::UnorderedElementsAreArray(expectedMockGatewayMqttSnMessageDatas));
}

// send by gateway => received by MqttSnForwarder
TEST_P(MqttSnGatewayNetworkInterfaceSendReceiveTests, ReceiveFromGateway) {
  ASSERT_TRUE(gatewayNetworkForwarderLooper.pauseLoop());
  for (uint16_t messageCounter = 0; messageCounter < toTestMessageCount; ++messageCounter) {
    if (useIdentifier) {
      *mockGateway->getGatewayDeviceAddress() = getDeviceAddressFromNetworkContext(mockGateway->getIdentifier(),
                                                                                   gatewayNetworkContext);
    }
    ComparableGatewayMqttSnMessageData data(toTestMessageLength,
                                            mockGateway->getGatewayDeviceAddress(),
                                            mockGateway->getIdentifier(),
                                            useIdentifier);
    expectedMockGatewayMqttSnMessageDatas.push_back(data);
  }

  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(toTestMessageCount)
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            ComparableGatewayMqttSnMessageData messageData(*receiveData, useIdentifier);
            actualMockGatewaySnMessageDatas.push_back(messageData);
            return 0;
          }
      ));

  ASSERT_TRUE(gatewayNetworkForwarderLooper.resumeLoop());

  for (auto data : expectedMockGatewayMqttSnMessageDatas) {
    device_address forwarderDeviceAddress = mockGateway->getForwarderDeviceAddress();
    ASSERT_EQ(mockGateway->send(&forwarderDeviceAddress,
                                &data.data[0],
                                data.data_length),
              data.data_length);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  EXPECT_THAT(actualMockGatewaySnMessageDatas,
              testing::UnorderedElementsAreArray(expectedMockGatewayMqttSnMessageDatas));
}


// send by MqttSnForwarder => received by gateway => send back to MqttSnForwarder
TEST_P(MqttSnGatewayNetworkInterfaceSendReceiveTests, SendToGatewayReceiveOnForwarder) {
  ASSERT_TRUE(gatewayNetworkForwarderLooper.pauseLoop());

  for (uint16_t messageCounter = 0; messageCounter < toTestMessageCount; ++messageCounter) {
    if (useIdentifier) {
      *mockGateway->getGatewayDeviceAddress() = getDeviceAddressFromNetworkContext(mockGateway->getIdentifier(),
                                                                                   gatewayNetworkContext);
    }
    ComparableGatewayMqttSnMessageData data(toTestMessageLength,
                                            mockGateway->getGatewayDeviceAddress(),
                                            mockGateway->getIdentifier(),
                                            useIdentifier);
    expectedMockGatewayMqttSnMessageDatas.push_back(data);
    forwarderMqttSnMessageDataBuffer.push_back(data);
  }

  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(expectedMockGatewayMqttSnMessageDatas.size())
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            ComparableGatewayMqttSnMessageData messageData(*receiveData,
                                                           useIdentifier);
            actualMockGatewaySnMessageDatas.push_back(messageData);
            return 0;
          }
      ));

  EXPECT_CALL(mockSendBuffer, pop(&sendBuffer, _))
      .Times(AtLeast(toTestMessageCount))
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

  EXPECT_CALL(*mockGateway->getReceiver(), receive_any_message(_, _, _))
      .Times(expectedMockGatewayMqttSnMessageDatas.size())
      .WillRepeatedly(Invoke(
          [this]
              (device_address *address,
               uint8_t *data,
               uint16_t data_length) -> void {
            ASSERT_EQ(mockGateway->send(address, data, data_length), data_length);
          }
      ));

  ASSERT_TRUE(gatewayNetworkForwarderLooper.resumeLoop());
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  EXPECT_THAT(actualMockGatewaySnMessageDatas,
              testing::UnorderedElementsAreArray(expectedMockGatewayMqttSnMessageDatas));
}


// send by gateway => received by MqttSnForwarder => send back to gateway
TEST_P(MqttSnGatewayNetworkInterfaceSendReceiveTests, ReceiveFromGatewaySendToGateway) {
  ASSERT_TRUE(gatewayNetworkForwarderLooper.pauseLoop());

  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(toTestMessageCount)
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            ComparableGatewayMqttSnMessageData data(*receiveData, useIdentifier);
            forwarderMqttSnMessageDataBuffer.push_back(data);
            return 0;
          }
      ));

  EXPECT_CALL(mockSendBuffer, pop(&sendBuffer, _))
      .Times(AtLeast(toTestMessageCount))
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

  EXPECT_CALL(*mockGateway->getReceiver(), receive_any_message(_, _, _))
      .WillRepeatedly(Invoke(
          [this]
              (device_address *address,
               uint8_t *data,
               uint16_t data_length) -> void {
            ComparableGatewayMqttSnMessageData messageData(address, data, data_length, useIdentifier);
            actualMockGatewaySnMessageDatas.push_back(messageData);
          }
      ));

  ASSERT_TRUE(gatewayNetworkForwarderLooper.resumeLoop());

  for (uint16_t messageCounter = 0; messageCounter < toTestMessageCount; ++messageCounter) {

    device_address forwarderDeviceAddress = mockGateway->getForwarderDeviceAddress();
    ComparableGatewayMqttSnMessageData data(toTestMessageLength,
                                            &forwarderDeviceAddress,
                                            mockGateway->getIdentifier(),
                                            useIdentifier);
    expectedMockGatewayMqttSnMessageDatas.push_back(data);

    ASSERT_EQ(mockGateway->send(&data), data.data_length);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  EXPECT_THAT(actualMockGatewaySnMessageDatas,
              testing::UnorderedElementsAreArray(expectedMockGatewayMqttSnMessageDatas));

}
