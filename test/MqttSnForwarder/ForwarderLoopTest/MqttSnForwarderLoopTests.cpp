//
// Created by SomeDude on 07.03.2019.
//

#include "MqttSnForwarderLoopTests.h"

TEST_F(MqttSnForwarderLoopTests,
       ClientAndGatewayNetworkReturnsZeroAndAddAndRemoveForwardingHeaderFromGatewayMessagesReturnZeroAndGatewayAndClientNetworkSendReturnsZero_ReturnsZero) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkReceiveBuffer,
      mqttSnForwarder.gatewayNetworkReceiveTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_receive(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkReceiveBuffer,
      mqttSnForwarder.clientNetworkReceiveTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  // AddForwardingHeaderToClientMessages always returns 0
  // RemoveForwardingHeaderFromGatewayMessages always returns 0

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      mqttSnForwarder.gatewayNetworkSendTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      mqttSnForwarder.clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), 0);
}

TEST_F(MqttSnForwarderLoopTests, ClientNetworkReceiveIsCalled) {

  EXPECT_CALL(clientNetworkMock, client_network_receive(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkReceiveBuffer,
      clientNetworkReceiveTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, GatewayNetworkReceiveIsCalled) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkReceiveBuffer,
      gatewayNetworkReceiveTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, GatewayNetworkReceiveFailed_GatewayNetworkDeinitialized) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkReceiveBuffer,
      gatewayNetworkReceiveTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(
      &mqttSnForwarder.gatewayNetwork,
      gatewayNetworkContext))
      .Times(AtLeast(1));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, GatewayNetworkReceiveFailed_ClientNetworkDeinitialized) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkReceiveBuffer,
      gatewayNetworkReceiveTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(
      &mqttSnForwarder.clientNetwork,
      clientNetworkContext))
      .Times(1);

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, ClientyNetworkReceiveFailed_GatewayNetworkDeinitialized) {

  EXPECT_CALL(clientNetworkMock, client_network_receive(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkReceiveBuffer,
      clientNetworkReceiveTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(
      &mqttSnForwarder.gatewayNetwork,
      gatewayNetworkContext))
      .Times(1);

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, ClientNetworkReceiveFailed_ClientNetworkDeinitialized) {

  EXPECT_CALL(clientNetworkMock, client_network_receive(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkReceiveBuffer,
      clientNetworkReceiveTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(
      &mqttSnForwarder.clientNetwork,
      clientNetworkContext))
      .Times(AtLeast(1));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, AddForwardingHeaderToClientMessagesFailed_GatewayNetworkDeinitialized) {
  GTEST_SKIP() << "not testable - AddForwardingHeaderToClientMessages always returns 0";
}

TEST_F(MqttSnForwarderLoopTests, AddForwardingHeaderToClientMessagesFailed_ClientNetworkDeinitialized) {
  GTEST_SKIP() << "not testable - AddForwardingHeaderToClientMessages always returns 0";
}

TEST_F(MqttSnForwarderLoopTests, RemoveForwardingHeaderFromGatewayMessagesFailed_GatewayNetworkDeinitialized) {
  GTEST_SKIP() << "not testable - RemoveForwardingHeaderFromGatewayMessages always returns 0";
}

TEST_F(MqttSnForwarderLoopTests, RemoveForwardingHeaderFromGatewayMessagesFailed_ClientNetworkDeinitialized) {
  GTEST_SKIP() << "not testable - RemoveForwardingHeaderFromGatewayMessages always returns 0";
}

TEST_F(MqttSnForwarderLoopTests, GatewayNetworkSendIsCalled) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      gatewayNetworkSendTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, ClientNetworkSendIsCalled) {

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, GatewayNetworkSendFailed_GatewayNetworkDeinitialized) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      gatewayNetworkSendTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(
      &mqttSnForwarder.gatewayNetwork,
      gatewayNetworkContext))
      .Times(AtLeast(1));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, GatewayNetworkSendFailed_ClientNetworkDeinitialized) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      gatewayNetworkSendTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(
      &mqttSnForwarder.clientNetwork,
      clientNetworkContext))
      .Times(AtLeast(1));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, ClientNetworkSendFailed_GatewayNetworkDeinitialized) {

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(
      &mqttSnForwarder.gatewayNetwork,
      gatewayNetworkContext))
      .Times(AtLeast(1));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderLoopTests, ClientNetworkSendFailed_ClientNetworkDeinitialized) {

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(
      &mqttSnForwarder.clientNetwork,
      clientNetworkContext))
      .Times(AtLeast(1));

  MqttSnForwarderLoop(&mqttSnForwarder);
}

// mqttSnForwarder.gatewayNetwork.status == -1 when (already tested by Disconnect):
// mqttSnForwarder.clientNetwork.status == -1 when (already tested by Disconnect):
// MqttSnForwarderLoop returns -1 when:
// mqttSnForwarder.clientNetwork.status == -1 and mqttSnForwarder.gatewayNetwork.status == -1 before loop
TEST_F(MqttSnForwarderLoopTests,
       BeforeLoopIsClientNetworkStatusMinusOneAndGatewayNetworkStatusMinusOne_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = -1;
  mqttSnForwarder.gatewayNetwork.status = -1;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// mqttSnForwarder.clientNetwork.status == -1 and mqttSnForwarder.gatewayNetwork.status == 0 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusMinusOneAndGatewayNetworkStatusZero_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = -1;
  mqttSnForwarder.gatewayNetwork.status = 0;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// mqttSnForwarder.clientNetwork.status == -1 and mqttSnForwarder.gatewayNetwork.status == 1 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusMinusOneAndGatewayNetworkStatusOne_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = -1;
  mqttSnForwarder.gatewayNetwork.status = 1;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}
// mqttSnForwarder.clientNetwork.status == 0 and mqttSnForwarder.gatewayNetwork.status == -1 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusZeroAndGatewayNetworkStatusMinusOne_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = 0;
  mqttSnForwarder.gatewayNetwork.status = -1;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// mqttSnForwarder.clientNetwork.status == 0 and mqttSnForwarder.gatewayNetwork.status == 0 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusZeroAndGatewayNetworkStatusZero_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = 0;
  mqttSnForwarder.gatewayNetwork.status = 0;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// mqttSnForwarder.clientNetwork.status == 0 and mqttSnForwarder.gatewayNetwork.status == 1 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusZeroAndGatewayNetworkStatusOne_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = 0;
  mqttSnForwarder.gatewayNetwork.status = 1;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// mqttSnForwarder.clientNetwork.status == 1 and mqttSnForwarder.gatewayNetwork.status == -1 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusOneAndGatewayNetworkStatusMinusOne_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = 1;
  mqttSnForwarder.gatewayNetwork.status = -1;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// mqttSnForwarder.clientNetwork.status == 1 and mqttSnForwarder.gatewayNetwork.status == 0 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusOneAndGatewayNetworkStatusZero_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = 1;
  mqttSnForwarder.gatewayNetwork.status = 0;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// mqttSnForwarder.clientNetwork.status == 1 and mqttSnForwarder.gatewayNetwork.status == 1 before loop
TEST_F(MqttSnForwarderLoopTests, BeforeLoopIsClientNetworkStatusOneAndGatewayNetworkStatusOne_ReturnsMinusOne) {
  mqttSnForwarder.clientNetwork.status = 1;
  mqttSnForwarder.gatewayNetwork.status = 1;

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}

// GatewayNetworkReceiveFailed
TEST_F(MqttSnForwarderLoopTests, GatewayNetworkReceiveFailed_ReturnsMinusOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkReceiveBuffer,
      gatewayNetworkReceiveTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}
// ClientNetworkReceiveFailed
TEST_F(MqttSnForwarderLoopTests, ClientNetworkReceiveFailed_ReturnsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_receive(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkReceiveBuffer,
      clientNetworkReceiveTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}
// AddForwardingHeaderToClientMessagesFailed
TEST_F(MqttSnForwarderLoopTests, AddForwardingHeaderToClientMessagesFailed_ReturnsMinusOne) {
  GTEST_SKIP() << "not testable - AddForwardingHeaderToClientMessages always returns 0";
}

// RemoveForwardingHeaderFromGatewayMessagesFailed - not testable
TEST_F(MqttSnForwarderLoopTests, RemoveForwardingHeaderFromGatewayMessagesFailed_ReturnsMinusOne) {
  GTEST_SKIP() << "not testable - RemoveForwardingHeaderFromGatewayMessages always returns 0";
}

// GatewayNetworkSendFailed
TEST_F(MqttSnForwarderLoopTests, GatewayNetworkSendFailed_ReturnsMinusOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(
      &mqttSnForwarder.gatewayNetwork,
      &mqttSnForwarder.gatewayNetworkSendBuffer,
      gatewayNetworkSendTimeout,
      gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}
// ClientNetworkSendFailed
TEST_F(MqttSnForwarderLoopTests, ClientNetworkSendFailed_ReturnsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_send(
      &mqttSnForwarder.clientNetwork,
      &mqttSnForwarder.clientNetworkSendBuffer,
      clientNetworkSendTimeout,
      clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), -1);
}
