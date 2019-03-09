//
// Created by SomeDude on 06.03.2019.
//

#include "RemoveForwardingHeaderFromGatewayMessagesTests.h"

#include <MqttSnFixedSizeRingBuffer.h>
#include <gmock/gmock-actions.h>

using ::testing::Return;
using ::testing::Invoke;

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_ReturnsZero) {
  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_GatewayMessageIsZeroed) {
  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_ClientMessageIsZeroed) {
  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_ClientMessageIsNotCreated) {
  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_GatewayMessageIsNotCreated) {
  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayMessageIsEmpty_ClientMessageIsNotCreated) {

  auto setMqttSnMessageDataDataLengthToZero =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthZero = {0};
        mqttSnMessageDataDataLengthZero.data_length = 0;
        memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthZero, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToZero));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientMessageIsNotCreated_ClientMessageIsNotPutIntoClientNetworkSendBuffer) {

  auto setMqttSnMessageDataDataLengthToZero =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthZero = {0};
        mqttSnMessageDataDataLengthZero.data_length = 0;
        memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthZero, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToZero));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &clientMessageData))
      .Times(0);

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientMessageIsNotCreatedAndClientMessageIsNotPutIntoClientNetworkSendBuffer_ReturnsZero) {

  auto setMqttSnMessageDataDataLengthToZero =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthZero = {0};
        mqttSnMessageDataDataLengthZero.data_length = 0;
        memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthZero, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToZero));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &clientMessageData))
      .Times(0);

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  ASSERT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientMessageIsCreated_ClientMessageIsPutIntoClientNetworkSendBuffer) {

  auto setMqttSnMessageDataDataLengthToMinimumSize =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
        mqttSnMessageDataDataLengthMinimumSize.data_length = FORWARDER_HEADER_LEN + sizeof(device_address);
        mqttSnMessageDataDataLengthMinimumSize.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address);
        memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToMinimumSize));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(0));

  RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData);

}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientMessageIsCreatedAndClientMessageIsPutIntoClientNetworkSendBuffer_ReturnsZero) {

  auto setMqttSnMessageDataDataLengthToMinimumSize =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
        mqttSnMessageDataDataLengthMinimumSize.data_length = FORWARDER_HEADER_LEN + sizeof(device_address);
        mqttSnMessageDataDataLengthMinimumSize.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address);
        memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToMinimumSize));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientNetworkSendBufferIsFullAndMessageIsPutBackIntoClientNetworkReceiveBuffer_PutBackReturnValueIsNotReturned) {

  auto setMqttSnMessageDataDataLengthToMinimumSize =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
        mqttSnMessageDataDataLengthMinimumSize.data_length = FORWARDER_HEADER_LEN + sizeof(device_address);
        mqttSnMessageDataDataLengthMinimumSize.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address);
        memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToMinimumSize));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData);

}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientNetworkSendBufferIsFull_ReturnsZero) {

  auto setMqttSnMessageDataDataLengthToMinimumSize =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
        mqttSnMessageDataDataLengthMinimumSize.data_length = FORWARDER_HEADER_LEN + sizeof(device_address);
        mqttSnMessageDataDataLengthMinimumSize.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address);
        memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToMinimumSize));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);
}
