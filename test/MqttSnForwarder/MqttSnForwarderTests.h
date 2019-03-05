//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H

#include <gtest/gtest.h>
#include <ClientNetworkMock.h>
#include <GatewayNetworkMock.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <MqttSnForwarder.h>
#include <gmock/gmock-nice-strict.h>

using ::testing::Return;
using ::testing::StrictMock;

ClientNetworkMock *globalClientNetworkMockObj;
GatewayNetworkMock *globalGatewayNetworkMockObj;

MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock = nullptr;
std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap = nullptr;

class MqttSnForwarderTests : public ::testing::Test {
 public:
  StrictMock<ClientNetworkMock> clientNetworkMock;
  StrictMock<GatewayNetworkMock> gatewayNetworkMock;

  MqttSnForwarder mqttSnForwarder;
  device_address client_network_address;
  void *clientNetworkContext = nullptr;

  device_address gateway_network_address;
  device_address mqtt_sn_gateway_address;
  void *gatewayNetworkContext = nullptr;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  virtual void SetUp() {

    globalClientNetworkMockObj = &clientNetworkMock;
    globalGatewayNetworkMockObj = &gatewayNetworkMock;

    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkReceiveBuffer,
                                                           &clientNetworkReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.clientNetworkSendBuffer,
                                                           &clientNetworkSendBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkReceiveBuffer,
                                                           &gatewayNetworkReceiveBuffer));
    mqttSnFixedSizeRingBufferMockMap.insert(std::make_pair(&mqttSnForwarder.gatewayNetworkSendBuffer,
                                                           &gatewayNetworkSendBuffer));
    globalMqttSnFixedSizeRingBufferMockMap = &mqttSnFixedSizeRingBufferMockMap;

    globalMqttSnFixedSizeRingBufferMock = &defaultMqttSnFixedSizeRingBufferMock;
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, MqttSnFixedSizeRingBufferInit(_))
        .Times(0);
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, put(_, _))
        .Times(0);
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, pop(_, _))
        .Times(0);
    EXPECT_CALL(defaultMqttSnFixedSizeRingBufferMock, isEmpty(_))
        .Times(0);

    {
      device_address client_network_address({0, 0, 0, 0, 0, 0});
      this->client_network_address = client_network_address;

      device_address gateway_network_address({0, 0, 0, 0, 0, 0});
      device_address mqtt_sn_gateway_address({0, 0, 0, 0, 0, 0});
      this->gateway_network_address = gateway_network_address;
      this->mqtt_sn_gateway_address = mqtt_sn_gateway_address;
    }

    clientNetworkMock.DelegateToFake();
    EXPECT_CALL(clientNetworkMock, client_network_init(&mqttSnForwarder.clientNetwork, clientNetworkContext))
        .Times(1);

    gatewayNetworkMock.DelegateToFake();
    EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext))
        .Times(1);

    ASSERT_EQ(ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                                &client_network_address,
                                clientNetworkContext,
                                mock_client_network_init), 0);
    EXPECT_EQ(GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                                 &gateway_network_address,
                                 &mqtt_sn_gateway_address,
                                 gatewayNetworkContext,
                                 mock_gateway_network_init), 0);

  }
  virtual void TearDown() {

    globalClientNetworkMockObj = nullptr;
    globalGatewayNetworkMockObj = nullptr;

    globalMqttSnFixedSizeRingBufferMockMap = nullptr;

  }
  /*
  static MqttSnMessageData *clientMessageData;
  static MqttSnMessageData *toCopyMessageData;

  static int d(MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) {
    if (toCopyMessageData != nullptr) {
      memcpy(clientMessageData, toCopyMessageData, sizeof(MqttSnMessageData));
    }
    MqttSnForwarderTests::clientMessageData = clientMessageData;
    return 0;
  }
  */
  MqttSnForwarderTests() {}
  virtual ~MqttSnForwarderTests() {}
};

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

TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_MqttSnForwarderInit_ReceivedMessageIsConverted) {

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

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_CallsPopOnClientNetworkReceiveBuffer) {

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  AddForwardingHeaderToClientMessages(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsMinusOne_OnlyPopIsCalled) {

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsMinusOne_ReturnsZero) {

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(0);

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder), 0);
}

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataIsZeroed_PutIsNotCalled) {

  auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
    MqttSnMessageData mqttSnMessageDataZeroed = {0};
    memcpy(clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData));
    return 0;
  };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Invoke(f));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(0);

  AddForwardingHeaderToClientMessages(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataIsZeroed_ReturnsZero) {

  auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
    MqttSnMessageData mqttSnMessageDataZeroed = {0};
    memcpy(clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData));
    return 0;
  };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Invoke(f));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(0);

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder), 0);
}

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataDataLengthIsOne_PutIsCalled) {

  auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
    MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
    mqttSnMessageDataDataLengthOne.data_length = 1;
    memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
    return 0;
  };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Invoke(f));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(1)
      .WillOnce(Return(0));

  AddForwardingHeaderToClientMessages(&mqttSnForwarder), 0;
}

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataDataLengthIsOneAndPutIsCalled_ReturnsZero) {

  auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
    MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
    mqttSnMessageDataDataLengthOne.data_length = 1;
    memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
    return 0;
  };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Invoke(f));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder), 0);
}

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroPutReturnsMinusOne_MessageIsPutBackIntoClientNetworkReceiveBuffer) {

  auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
    MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
    mqttSnMessageDataDataLengthOne.data_length = 1;
    memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
    return 0;
  };

  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Invoke(f));

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(0));

  AddForwardingHeaderToClientMessages(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderTests,
       MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroPutReturnsMinusOne_MessageIsPutBackIntoClientNetworkReceiveBuffer_ReturnsZero) {

  ASSERT_TRUE(false) << "Not implemented yet";

  static MqttSnMessageData *clientMessageData;
  /*
  MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
  mqttSnMessageDataDataLengthOne.data_length = 1;
  MqttSnForwarderTests::toCopyMessageData = &mqttSnMessageDataDataLengthOne;
  EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Invoke(MqttSnForwarderTests::d));
  */

  EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder), 0);
}



/*
TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_RemoveMqttSnForwardingHeader_CallsPopOnClientGatewayReceiveBuffer) {

  EXPECT_CALL(gatewayNetworkReceiveBuffer, pop(&mqttSnForwarder.gatewayNetworkReceiveBuffer, _))
      .Times(1)
      .WillOnce(Return(-1));

  ASSERT_EQ(RemoveMqttSnForwardingHeader(&mqttSnForwarder), 0);
}
 */
#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERTESTS_H
