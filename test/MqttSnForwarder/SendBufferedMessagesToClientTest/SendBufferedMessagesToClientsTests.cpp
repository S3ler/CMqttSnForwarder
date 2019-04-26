//
// Created by SomeDude on 07.03.2019.
//

#include "SendBufferedMessagesToClientsTests.h"
#include <shared/MqttSnMessageDataGenerator/MqttSnMessageDataGenerator.h>

TEST_F(SendBufferedMessagesToClientsTests,
       DISABLED_ClientNetworkSendDoesNotEmptyClientNetworkSendBuffer_ProgrammStucks) {
  // TODO write a test for the following situation:
  // client_network_send always return 0 but never pops any message from the clientNetworkSendBuffer
  ASSERT_TRUE(false) << "not implemented yet";
}

TEST_F(SendBufferedMessagesToClientsTests,
       ClientNetworkReceiveBufferIsEmptyAndGatewayNetworkSendBufferIsEmpty_ReturnsImmediately) {

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1)
      .WillOnce(Return(1));
  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(1));

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       ClientNetworkReceiveBufferIsEmptyAndGatewayNetworkSendBufferIsNotEmpty_ReturnsAfterTwoLoops) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(2)
      .WillOnce(Return(1))
      .WillOnce(Return(1));
  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(2)
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  ON_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       ClientNetworkReceiveBufferIsNotEmptyAndGatewayNetworkSendBufferIsEmpty_ReturnsAfterTwoLoops) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(2)
      .WillOnce(Return(0))
      .WillOnce(Return(1));
  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(2)
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  ON_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       AfterFirstCallClientNetworkSendReturnMinusOne_ReturnsImmediately) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1)
      .WillOnce(Return(0));
  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1)
      .WillOnce(Return(0));

  ON_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       AfterSecondCallClientNetworkSendReturnMinusOne_ReturnsImmediately) {

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(2)
      .WillRepeatedly(Return(0));


  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(2)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(2)
      .WillRepeatedly(Return(0));

  ON_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .WillByDefault(Return(-1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(2)
      .WillRepeatedly(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(2)
      .WillOnce(Return(0))
      .WillOnce(Return(-1));

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsZeroMessagesAndClientNetworkSendBufferContainsOneMessage_SendsOneMessage) {
  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = 0;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = 1;

  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(0);

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(gatewayNetworkReceiveBufferMqttSnMessageCount);

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsOneMessageAndClientNetworkSendBufferContainsZeroMessage_SendsOneMessage) {
  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = 1;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = 0;

  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;


  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(0);

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(gatewayNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsOneMessageAndClientNetworkSendBufferContainsOneMessage_SendsTwoMessages) {
  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = 1;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = 1;

  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(0);

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(gatewayNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsTwoMessagesAndClientNetworkSendBufferContainsZeroMessages_SendsTwoMessages) {
  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = 2;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = 0;

  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(0);

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(gatewayNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsZeroMessagesAndClientNetworkSendBufferContainsTwoMessages_SendsTwoMessages) {
  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = 0;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = 2;

  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(0);

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(gatewayNetworkReceiveBufferMqttSnMessageCount);

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsTwoMessagesAndClientNetworkSendBufferContainsTwoMessages_SendsFourMessages) {
  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = 2;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = 2;

  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  //uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(0);

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(gatewayNetworkReceiveBufferMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsMaximumMessageCountAndClientNetworkSendBufferContainsMaximumMessageCount_SendsAllMessages) {

  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  ON_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .WillByDefault(Return(1));

  ON_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .WillByDefault(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&clientNetworkSendBufferMqttSnMessageDataCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (clientNetworkSendBufferMqttSnMessageDataCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(1 + gatewayNetworkReceiveBufferMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&clientNetworkSendBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *b,
               MqttSnMessageData *m) -> int {
            if (clientNetworkSendBufferMqttSnMessageDataCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            clientNetworkSendBufferMqttSnMessageDataCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&clientNetworkSendBufferMqttSnMessageDataCounter]
              (MqttSnClientNetworkInterface *n,
               MqttSnFixedSizeRingBuffer *sendBuffer,
               uint32_t timeout_ms,
               void *context) -> int {
            if (clientNetworkSendBufferMqttSnMessageDataCounter > 0) {
              clientNetworkSendBufferMqttSnMessageDataCounter--;
            }
            return 0;
          }
      ));

  sendBufferedMessagesToClients(&mqttSnForwarder);
}

TEST_F(SendBufferedMessagesToClientsTests,
       GatewayNetworkReceiveBufferContainsMaximumMessageCountAndClientNetworkSendBufferContainsMaximumMessageCount_GatewayNetworkReceiveIsNotCalled) {

  const uint32_t gatewayNetworkReceiveBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  const uint32_t clientNetworkSendBufferMqttSnMessageCount = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  const uint32_t totalMqttSnMessageCount =
      gatewayNetworkReceiveBufferMqttSnMessageCount + clientNetworkSendBufferMqttSnMessageCount;

  uint32_t gatewayNetworkReceiveBufferMqttSnMessageDataCounter = gatewayNetworkReceiveBufferMqttSnMessageCount;
  uint32_t clientNetworkSendBufferMqttSnMessageDataCounter = clientNetworkSendBufferMqttSnMessageCount;

  EXPECT_CALL(clientNetworkSendBuffer, isFull(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Return(0));

  ON_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .WillByDefault(Return(1));

  ON_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .WillByDefault(Return(1));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1 + totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&clientNetworkSendBufferMqttSnMessageDataCounter]
              (const MqttSnFixedSizeRingBuffer *) -> int {
            if (clientNetworkSendBufferMqttSnMessageDataCounter == 0) {
              return 1;
            }
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter == 0) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter--;
            MqttSnMessageData mqttSnMessageData = MqttSnMessageDataGenerator::generateMqttSnMessageData(
                static_cast<uint16_t >(gatewayNetworkReceiveBufferMqttSnMessageDataCounter % UINT16_MAX));
            memcpy(m, &mqttSnMessageData, sizeof(MqttSnMessageData));
            return 0;
          }
      ));

  EXPECT_CALL(gatewayNetworkReceiveBuffer, put(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillRepeatedly(Invoke(
          [&gatewayNetworkReceiveBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *, MqttSnMessageData *m) -> int {
            if (gatewayNetworkReceiveBufferMqttSnMessageDataCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            gatewayNetworkReceiveBufferMqttSnMessageDataCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer, _))
      .Times(1 + gatewayNetworkReceiveBufferMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&clientNetworkSendBufferMqttSnMessageDataCounter]
              (MqttSnFixedSizeRingBuffer *b,
               MqttSnMessageData *m) -> int {
            if (clientNetworkSendBufferMqttSnMessageDataCounter
                == CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN) {
              return -1;
            }
            clientNetworkSendBufferMqttSnMessageDataCounter++;
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(totalMqttSnMessageCount)
      .WillRepeatedly(Invoke(
          [&clientNetworkSendBufferMqttSnMessageDataCounter]
              (MqttSnClientNetworkInterface *n,
               MqttSnFixedSizeRingBuffer *sendBuffer,
               uint32_t timeout_ms,
               void *context) -> int {
            if (clientNetworkSendBufferMqttSnMessageDataCounter > 0) {
              clientNetworkSendBufferMqttSnMessageDataCounter--;
            }
            return 0;
          }
      ));

  EXPECT_CALL(clientNetworkMock, client_network_receive(_, _, _, _))
      .Times(0);

  sendBufferedMessagesToClients(&mqttSnForwarder);
}
