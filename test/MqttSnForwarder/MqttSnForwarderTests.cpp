//
// Created by SomeDude on 04.03.2019.
//

#include "MqttSnForwarderTests.h"

TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_MqttSnForwarderInit_NetworkBufferAreInitialized) {

  // happens during MqttSnForwarderInit, ClientNetworkConnect and GatewayNetworkConnect
  EXPECT_CALL(clientNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1);
  EXPECT_CALL(clientNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1);
  EXPECT_CALL(gatewayNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1);
  EXPECT_CALL(gatewayNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnForwarder.clientNetwork, clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(MqttSnForwarderInit(&mqttSnForwarder, clientNetworkContext, gatewayNetworkContext), 0);

  // happens during MqttSnForwarderLoop
  // receive step
  EXPECT_CALL(clientNetworkMock, client_network_receive(&mqttSnForwarder.clientNetwork,
                                                        &mqttSnForwarder.clientNetworkReceiveBuffer,
                                                        CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
                                                        clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));
  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(&mqttSnForwarder.gatewayNetwork,
                                                          &mqttSnForwarder.gatewayNetworkReceiveBuffer,
                                                          GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
                                                          gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  // AddForwardingHeaderToClientMessages
  // clientNetworkReceiveBuffer is empty
  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  // RemoveForwardingHeaderFromGatewayMessages
  // clientNetworkReceiveBuffer is empty
  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  // send step
  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(&mqttSnForwarder.gatewayNetwork,
                                                       &mqttSnForwarder.gatewayNetworkSendBuffer,
                                                       GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
                                                       gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));
  EXPECT_CALL(clientNetworkMock, client_network_send(&mqttSnForwarder.clientNetwork,
                                                     &mqttSnForwarder.clientNetworkSendBuffer,
                                                     CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT,
                                                     clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), 0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnForwarder.clientNetwork, clientNetworkContext))
      .Times(1);
  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
      .Times(1);

  MqttSnForwarderDeinit(&mqttSnForwarder);

}

TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_MqttSnForwarderInit_ReceivedMessageIsConverted1) {

  // happens during MqttSnForwarderInit, ClientNetworkConnect and GatewayNetworkConnect
  EXPECT_CALL(clientNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkReceiveBuffer))
      .Times(1);
  EXPECT_CALL(clientNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkSendBuffer))
      .Times(1);
  EXPECT_CALL(gatewayNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
      .Times(1);
  EXPECT_CALL(gatewayNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkSendBuffer))
      .Times(1);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnForwarder.clientNetwork, clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(MqttSnForwarderInit(&mqttSnForwarder, clientNetworkContext, gatewayNetworkContext), 0);

  // happens during MqttSnForwarderLoop
  // receive step
  EXPECT_CALL(clientNetworkMock, client_network_receive(&mqttSnForwarder.clientNetwork,
                                                        &mqttSnForwarder.clientNetworkReceiveBuffer,
                                                        CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
                                                        clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));
  EXPECT_CALL(gatewayNetworkMock, gateway_network_receive(&mqttSnForwarder.gatewayNetwork,
                                                          &mqttSnForwarder.gatewayNetworkReceiveBuffer,
                                                          GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
                                                          gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  // AddForwardingHeaderToClientMessages
  // clientNetworkReceiveBuffer is empty
  MqttSnMessageData a = {0};
  a.data_length = 1;
  auto f = [](MqttSnFixedSizeRingBuffer *a, MqttSnMessageData *m) -> int {
    memcpy(m, &a, sizeof(MqttSnMessageData));
    return 0;
  };
  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Invoke(f));
  //.WillOnce(Invoke(MqttSnForwarderTests::d))
  //.WillOnce(Invoke(this, MqttSnForwarderTests::d))
  //.WillOnce(Return(0))

  /*
  EXPECT_CALL(clientNetworkSendBuffer, put(&mqttSnForwarder.clientNetworkSendBuffer,&mqttSnMessageData))
      .Times(1)
      .WillOnce(Return(0));
  */
  // RemoveForwardingHeaderFromGatewayMessages
  // clientNetworkReceiveBuffer is empty
  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  // send step
  EXPECT_CALL(gatewayNetworkMock, gateway_network_send(&mqttSnForwarder.gatewayNetwork,
                                                       &mqttSnForwarder.gatewayNetworkSendBuffer,
                                                       GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
                                                       gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));
  EXPECT_CALL(clientNetworkMock, client_network_send(&mqttSnForwarder.clientNetwork,
                                                     &mqttSnForwarder.clientNetworkSendBuffer,
                                                     CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT,
                                                     clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(MqttSnForwarderLoop(&mqttSnForwarder), 0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnForwarder.clientNetwork, clientNetworkContext))
      .Times(1);
  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
      .Times(1);

  MqttSnForwarderDeinit(&mqttSnForwarder);

}

/*
TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_RemoveMqttSnForwardingHeader_CallsPopOnClientGatewayReceiveBuffer) {

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveMqttSnForwardingHeader(&mqttSnForwarder), 0);
}
 */