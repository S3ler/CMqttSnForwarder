//
// Created by SomeDude on 03.03.2019.
//

#include "MqttSnClientNetworkInterfaceTests.h"

using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSucess_StatusIsZero) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSucess_ClientNetworkAddressIsSet) {
  device_address client_network_address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &client_network_address,
                              context,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_address, &client_network_address);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSucess_ClientNetworkInitIsSet) {
  device_address client_network_address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface,
                              &client_network_address,
                              context,
                              mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_init, mock_client_network_init);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSucess_ClientNetworkInitIsCalled) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitSucess_ReturnsZero) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitFails_ReturnsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests, MqttSnClientNetworkInterfaceTests_AfterInitFails_StatusIsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), -1);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkInitIsSet) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_init, mock_client_network_init);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkConnectIsSet) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkDisconnectIsSet) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_disconnect, mock_client_network_disconnect);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkReceiveIsSet) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_receive, mock_client_network_receive);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_AfterNetworkInitIsCalledSuccess_ClientNetworkSendIsSet) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.client_network_send, mock_client_network_send);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectSuccess_ClientNetworkConnectIsCalled) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ClientNetworkConnect(&mqttSnClientNetworkInterface, context);

}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectSuccess_ReturnsZero) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, context), 0);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectSuccess_StatusIsOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, context), 0);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, 1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectFail_ReturnsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, context), -1);

}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectFail_StatusIsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  ASSERT_EQ(mqttSnClientNetworkInterface.client_network_connect, mock_client_network_connect);

  EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnClientNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, context), -1);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectCalledBeforeInit_ClientNetworkConnectIsNotCalled) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(0);

  ClientNetworkConnect(&mqttSnClientNetworkInterface, context);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectCalledBeforeInit_ReturnsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(0);

  EXPECT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, context), -1);

}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkConnectCalledBeforeInit_StatusIsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(0);

  ASSERT_EQ(ClientNetworkConnect(&mqttSnClientNetworkInterface, context), -1);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalledBeforeInit_ClientNetworkDisconnectIsNotCalled) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, context))
      .Times(0);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, context);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalledBeforeInit_StatusIsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, context))
      .Times(0);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, context);

  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalled_ClientNetworkDisconnectIsCalled) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, context);

}

TEST_F(MqttSnClientNetworkInterfaceTests,
       MqttSnClientNetworkInterfaceTests_ClientNetworkDisconnectCalled_StatusIsMinusOne) {
  device_address address({0, 0, 0, 0, 0, 0});
  void *context = nullptr;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface = {0};

  clientNetworkMock.DelegateToFake();
  EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ASSERT_EQ(ClientNetworkInit(&mqttSnClientNetworkInterface, &address, context, mock_client_network_init), 0);

  EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnClientNetworkInterface, context))
      .Times(1);

  ClientNetworkDisconnect(&mqttSnClientNetworkInterface, context);
  EXPECT_EQ(mqttSnClientNetworkInterface.status, -1);
}
