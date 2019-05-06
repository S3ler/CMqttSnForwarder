//
// Created by SomeDude on 03.03.2019.
//

#include "MqttSnClientNetworkInterfaceTests.h"

using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSuccess_StatusIsOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, 1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterInitSuccess_ClientNetworkAddressIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_address, &forwarder_client_network_address);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterInitSuccess_MqttSnGatewayAddressIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.mqtt_sn_gateway_address, &mqtt_sn_gateway_network_address);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSuccess_ClientNetworkInitIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_init, mock_client_network_init);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterInitSuccess_ClientNetworkInitIsCalled) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSuccess_ReturnsZero) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  EXPECT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitFails_ReturnsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitFail));

  EXPECT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitFails_StatusIsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitFail));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), -1);
  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkInitIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_init, mock_client_network_init);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkConnectIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkDisconnectIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_disconnect, mock_client_network_disconnect);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkReceiveIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_receive, mock_client_network_receive);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkSendIsSet) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_send, mock_client_network_send);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectSuccess_ClientNetworkConnectIsCalled) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext);

}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectSuccess_ReturnsZero) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext), 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectSuccess_StatusIsTwo) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, 2);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectFail_ReturnsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext), -1);

}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectFail_StatusIsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext), -1);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectCalledBeforeInit_ClientNetworkConnectIsNotCalled) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(0);

  ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectCalledBeforeInit_ReturnsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(0);

  EXPECT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext), -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectCalledBeforeInit_StatusIsZero) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(0);

  ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, clientNetworkContext), -1);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalledBeforeInit_ClientNetworkDisconnectIsNotCalled) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(0);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, clientNetworkContext);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalledBeforeInit_StatusIsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(0);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, clientNetworkContext);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalled_ClientNetworkDisconnectIsCalled) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, clientNetworkContext);

}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalled_StatusIsMinusOne) {

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeClientNetworkInitSuccess));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &mqtt_sn_gateway_network_address,
                              &forwarder_client_network_address, NULL,
                              clientNetworkContext,
                              mock_client_network_init), 0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, clientNetworkContext))
      .Times(1);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, clientNetworkContext);
  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}
