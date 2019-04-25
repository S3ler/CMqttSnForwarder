//
// Created by SomeDude on 06.03.2019.
//

#include "AddForwardingHeaderToClientMessagesTests.h"

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_ReturnsZero) {

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(0);

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData), 0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_GatewayMessageIsZeroed) {

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(0);

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_ClientMessageIsZeroed) {

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(0);

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_GatewayMessageIsNotCreated) {

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(0);

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_ClientMessageIsEmpty_GatewayMessageIsNotCreated) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 0;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(0);

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);

  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)), 0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_GatewayMessageIsNotCreated_GatewayMessageIsNotPutIntoGatewayNetworkSendBuffer) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 0;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(0);

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_GatewayMessageIsNotCreatedAndGatewayMessageIsNotPutIntoGatewayNetworkSendBuffer_ReturnsZero) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 0;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(0);

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData), 0);
  // TODO rework
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_GatewayMessageIsCreated_GatewayMessageIsPutIntoGatewayNetworkSendBuffer) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 1;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_GatewayMessageIsCreatedAndGatewayMessageIsPutIntoGatewayNetworkSendBuffer_ReturnsZero) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 1;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(0);

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData), 0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_GatewayNetworkSendBufferIsFull_MessageIsPutBackIntoClientNetworkReceiveBuffer) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 1;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(0));

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_GatewayNetworkSendBufferIsFullAndMessageIsPutBackIntoClientNetworkReceiveBuffer_PutBackReturnValueIsNotReturned) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 1;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
       AddForwardingHeaderTests_GatewayNetworkSendBufferIsFull_ReturnsZero) {

  auto setMqttSnMessageDataDataLengthToOne =
      [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
        MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
        mqttSnMessageDataDataLengthOne.data_length = 1;
        memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
        return 0;
      };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData), 0);
}
