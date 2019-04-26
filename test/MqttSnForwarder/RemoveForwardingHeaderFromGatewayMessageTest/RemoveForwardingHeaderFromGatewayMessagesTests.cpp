//
// Created by SomeDude on 06.03.2019.
//

#include <MqttSnMessageParser.h>
#include "RemoveForwardingHeaderFromGatewayMessagesTests.h"

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_ReturnsZero) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_GatewayMessageIsZeroed) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_ClientMessageIsZeroed) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests, GatewayNetworkReceiveBufferIsEmpty_EmptyClientMessageIsCreated) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientMessageIsNotCreated_ClientMessageIsNotPutIntoClientNetworkSendBuffer) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(
          [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
            MqttSnMessageData mqttSnMessageDataDataLengthZero = {0};
            mqttSnMessageDataDataLengthZero.data_length = 0;
            memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthZero, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &clientMessageData))
      .Times(0);

  ASSERT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       ClientMessageIsNotCreatedAndClientMessageIsNotPutIntoClientNetworkSendBuffer_ReturnsZero) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(
          [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
            MqttSnMessageData mqttSnMessageDataDataLengthZero = {0};
            mqttSnMessageDataDataLengthZero.data_length = 0;
            memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthZero, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  ASSERT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       MinimalEncapsulatedMessageEmptyMqttSnMessage_ClientMessageIsCreated_ClientMessageIsPutIntoClientNetworkSendBuffer) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(
          [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
            MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
            mqttSnMessageDataDataLengthMinimumSize.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
            mqttSnMessageDataDataLengthMinimumSize.data[0] = mqttSnMessageDataDataLengthMinimumSize.data_length;
            mqttSnMessageDataDataLengthMinimumSize.data[1] = ENCAPSULATED_MESSAGE;
            memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(0));

  RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData);

}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       MinimalEncapsulatedMessageEmptyMqttSnMessage_ClientMessageIsCreatedAndClientMessageIsPutIntoClientNetworkSendBuffer_ReturnsZero) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(
          [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
            MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
            mqttSnMessageDataDataLengthMinimumSize.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
            mqttSnMessageDataDataLengthMinimumSize.data[0] = mqttSnMessageDataDataLengthMinimumSize.data_length;
            mqttSnMessageDataDataLengthMinimumSize.data[1] = ENCAPSULATED_MESSAGE;
            memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);

}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       MinimalEncapsulatedMessageEmptyMqttSnMessage_ClientNetworkSendBufferIsFullAndMessageIsPutBackIntoClientNetworkReceiveBuffer_PutBackReturnValueIsNotReturned) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(
          [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
            MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
            mqttSnMessageDataDataLengthMinimumSize.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
            mqttSnMessageDataDataLengthMinimumSize.data[0] = mqttSnMessageDataDataLengthMinimumSize.data_length;
            mqttSnMessageDataDataLengthMinimumSize.data[1] = ENCAPSULATED_MESSAGE;
            memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData);

}

TEST_F(RemoveForwardingHeaderFromGatewayMessagesTests,
       MinimalEncapsulatedMessageEmptyMqttSnMessage_ClientNetworkSendBufferIsFull_ReturnsZero) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Invoke(
          [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *gatewayMessageData) -> int {
            MqttSnMessageData mqttSnMessageDataDataLengthMinimumSize = {0};
            mqttSnMessageDataDataLengthMinimumSize.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
            mqttSnMessageDataDataLengthMinimumSize.data[0] = mqttSnMessageDataDataLengthMinimumSize.data_length;
            mqttSnMessageDataDataLengthMinimumSize.data[1] = ENCAPSULATED_MESSAGE;
            memcpy(gatewayMessageData, &mqttSnMessageDataDataLengthMinimumSize, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(RemoveForwardingHeaderFromGatewayMessages(&mqttSnForwarder, &gatewayMessageData, &clientMessageData), 0);
}
