//
// Created by SomeDude on 07.03.2019.
//


#include "SendBufferedMessagesToGatewayTests.h"
#include <test/shared/MqttSnMessageDataGenerator/MqttSnMessageDataGenerator.h>
// TODO exchange clientNetworkReceiveBufferMqttSnMessageCounter for data generation with CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH
// TODO tests only run with a minimal CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH of MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false)
// TODO tests only run with a minimal CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN of 3

TEST_F(SendBufferedMessagesToGatewayTests,
       DISABLED_GatewayNetworkSendDoesNotEmptyGatewayNetworkSendBuffer_ProgrammStucks) {
  // TODO write a test for the following situation:
  // gateway_network_send always return 0 but never pops any message from the gatewayNetworkSendBuffer
  ASSERT_TRUE(false) << "not implemented yet";
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferIsEmptyAndGatewayNetworkSendBufferIsEmpty_ReturnsImmediately) {

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1)
      .WillOnce(Return(1));
  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(1));

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferIsEmptyAndGatewayNetworkSendBufferIsNotEmpty_ReturnsAfterTwoLoops) {

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(2)
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(2)
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  ON_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferIsNotEmptyAndGatewayNetworkSendBufferIsEmpty_ReturnsAfterTwoLoops) {

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1).
      WillRepeatedly(Return(0));


  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(2)
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(2)
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  ON_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       AfterFirstCallGatewayNetworkSendReturnMinusOne_ReturnsImmediately) {

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  ON_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       AfterSecondCallGatewayNetworkSendReturnMinusOne_ReturnsImmediately) {

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(2).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(2)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(2)
      .WillRepeatedly(Return(0));

  ON_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(2)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(2)
      .WillOnce(Return(0))
      .WillOnce(Return(-1));

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsZeroMessagesAndGatewayNetworkSendBufferContainsOneMessage_SendsOneMessage) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = 0;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = 1;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(clientNetworkReceiveBufferMqttSnMessageCount);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsOneMessagesAndGatewayNetworkSendBufferContainsOneMessage_SendsTwoMessages) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = 1;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = 1;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(clientNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsOneMessagesAndGatewayNetworkSendBufferContainsZeroMessage_SendsOneMessage) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = 1;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = 0;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(clientNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsTwoMessagesAndGatewayNetworkSendBufferContainsZeroMessage_SendsTwoMessages) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = 2;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = 0;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(clientNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsZeroMessagesAndGatewayNetworkSendBufferContainsTwoMessages_SendsTwoMessages) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = 0;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = 2;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(clientNetworkReceiveBufferMqttSnMessageCount);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsTwoMessagesAndGatewayNetworkSendBufferContainsTwoMessages_SendsFourMessages) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = 2;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = 2;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(clientNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsMaximumMessageCountAndGatewayNetworkSendBufferContainsMaximumMessageCount_SendsAllMessages) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkSendBufferMqttSnMessageCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (gatewayNetworkSendBufferMqttSnMessageCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(1 + clientNetworkReceiveBufferMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkSendBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *b,
               MqttSnMessageData *m) -> int {
            if (gatewayNetworkSendBufferMqttSnMessageCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            gatewayNetworkSendBufferMqttSnMessageCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkSendBufferMqttSnMessageCounter]
              (MqttSnGatewayNetworkInterface *n,
               MqttSnFixedSizeRingBuffer *sendBuffer,
               uint32_t timeout_ms,
               void *context) -> int {
            if (gatewayNetworkSendBufferMqttSnMessageCounter > 0) {
              gatewayNetworkSendBufferMqttSnMessageCounter--;
            }
            return 0;
          }
      ));

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToGatewayTests,
       ClientNetworkReceiveBufferContainsMaximumMessageCountAndGatewayNetworkSendBufferContainsMaximumMessageCount_GatewayNetworkReceiveIsNotCalled) {
  const uint32_t clientNetworkReceiveBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  const uint32_t gatewayNetworkSendBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;

  const uint32_t totalMqttSnMessageCounter =
      clientNetworkReceiveBufferMqttSnMessageCount + gatewayNetworkSendBufferMqttSnMessageCount;

  uint32_t clientNetworkReceiveBufferMqttSnMessageCounter = clientNetworkReceiveBufferMqttSnMessageCount;
  uint32_t gatewayNetworkSendBufferMqttSnMessageCounter = gatewayNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(totalMqttSnMessageCounter).
      WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkSendBufferMqttSnMessageCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (gatewayNetworkSendBufferMqttSnMessageCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter == 0) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateM1MqttSnPublishMqttSnMessageData(
                static_cast<uint16_t >(clientNetworkReceiveBufferMqttSnMessageCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Invoke(
          [&clientNetworkReceiveBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (clientNetworkReceiveBufferMqttSnMessageCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            clientNetworkReceiveBufferMqttSnMessageCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(1 + clientNetworkReceiveBufferMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkSendBufferMqttSnMessageCounter]
              (MqttSnFixedSizeRingBuffer *b,
               MqttSnMessageData *m) -> int {
            if (gatewayNetworkSendBufferMqttSnMessageCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            gatewayNetworkSendBufferMqttSnMessageCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      gatewayNetworkContext))
      .Times(totalMqttSnMessageCounter)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkSendBufferMqttSnMessageCounter]
              (MqttSnGatewayNetworkInterface *n,
               MqttSnFixedSizeRingBuffer *sendBuffer,
               uint32_t timeout_ms,
               void *context) -> int {
            if (gatewayNetworkSendBufferMqttSnMessageCounter > 0) {
              gatewayNetworkSendBufferMqttSnMessageCounter--;
            }
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToGateway(&mqttSnForwarder);
}
