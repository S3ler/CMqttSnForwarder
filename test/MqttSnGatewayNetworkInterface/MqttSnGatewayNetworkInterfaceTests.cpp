//
// Created by SomeDude on 04.03.2019.
//

#include "MqttSnGatewayNetworkInterfaceTests.h"

TEST_F(MqttSnGatewayNetworkInterfaceTests, MqttSnGatewayNetworkInterfaceTests_AfterInitSucess_StatusIsOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, 1);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterInitSucess_GatewayNetworkAddressIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &mqtt_sn_gateway_network_address,
                               &forwarder_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.gateway_network_address, &forwarder_gateway_network_address);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterInitSucess_MqttSnGatewayNetworkAddressIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &mqtt_sn_gateway_network_address,
                               &forwarder_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.mqtt_sn_gateway_address, &mqtt_sn_gateway_network_address);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterInitSucess_GatewayNetworkInitIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.gateway_network_init, mock_gateway_network_init);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterInitSucess_GatewayNetworkInitIsCalled) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, MqttSnGatewayNetworkInterfaceTests_InitSucess_ReturnsZero) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  EXPECT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterGatewayNetworkInitFails_ReturnsMinusOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitFail));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), -1);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterGatewayNetworkInitFails_StatusIsMinusOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitFail));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), -1);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, -1);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterGatewayNetworkInitIsCalledSuccess_GatewayNetworkInitIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.gateway_network_init, mock_gateway_network_init);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterGatewayNetworkInitIsCalledSuccess_GatewayNetworkConnectIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.gateway_network_connect, mock_gateway_network_connect);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests, AfterGatewayNetworkInitIsCalledSuccess_GatewayNetworkDisconnectIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.gateway_network_disconnect, mock_gateway_network_disconnect);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       AfterGatewayNetworkInitIsCalledSuccess_GatewayNetworkReceiveIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.gateway_network_receive, mock_gateway_network_receive);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       AfterGatewayNetworkInitIsCalledSuccess_GatewayNetworkSendIsSet) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.gateway_network_send, mock_gateway_network_send);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectSuccess_GatewayNetworkConnectIsCalled) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  ASSERT_EQ(mqttSnGatewayNetworkInterface.gateway_network_connect, mock_gateway_network_connect);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectSuccess_ReturnsZero) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  ASSERT_EQ(mqttSnGatewayNetworkInterface.gateway_network_connect, mock_gateway_network_connect);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext), 0);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectSuccess_StatusIsTwo) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  ASSERT_EQ(mqttSnGatewayNetworkInterface.gateway_network_connect, mock_gateway_network_connect);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, 2);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectFails_ReturnsMinusOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  ASSERT_EQ(mqttSnGatewayNetworkInterface.gateway_network_connect, mock_gateway_network_connect);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext), -1);

}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectFails_StatusIsMinusOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  ASSERT_EQ(mqttSnGatewayNetworkInterface.gateway_network_connect, mock_gateway_network_connect);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext), -1);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, -1);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectCalledBeforeInit_GatewayNetworkConnectIsNotCalled) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(0);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(0);

  GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectCalledBeforeInit_ReturnsMinusOne) {

  GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(0);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(0);

  EXPECT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext), -1);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkConnectCalledBeforeInit_StatusIsZero) {

  GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(0);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnGatewayNetworkInterface,
                                                          gatewayNetworkContext))
      .Times(0);

  ASSERT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext), -1);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, 0);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkDisonnectCalledBeforeInit_GatewayNetworkDisconnectIsNotCalled) {

  GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(0);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnGatewayNetworkInterface,
                                                             gatewayNetworkContext))
      .Times(0);

  GatewayNetworkDisconnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);

}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkDisonnectCalledBeforeInit_StatusIsMinusOne) {

  GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface,
                                                       gatewayNetworkContext))
      .Times(0);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnGatewayNetworkInterface,
                                                             gatewayNetworkContext))
      .Times(0);

  GatewayNetworkDisconnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, -1);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkDisonnectCalled_GatewayNetworkDisconnectIsCalled) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnGatewayNetworkInterface,
                                                             gatewayNetworkContext))
      .Times(1);

  GatewayNetworkDisconnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);
}

TEST_F(MqttSnGatewayNetworkInterfaceTests,
       GatewayNetworkDisonnectCalled_StatusIsMinusOne) {

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, gatewayNetworkContext))
      .Times(1)
      .WillOnce(Invoke(fakeGatewayNetworkInitSuccess));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &forwarder_gateway_network_address,
                               &mqtt_sn_gateway_network_address, NULL,
                               gatewayNetworkContext,
                               mock_gateway_network_init), 0);

  EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnGatewayNetworkInterface,
                                                             gatewayNetworkContext))
      .Times(1);

  GatewayNetworkDisconnect(&mqttSnGatewayNetworkInterface, gatewayNetworkContext);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, -1);
}
