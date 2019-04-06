//
// Created by SomeDude on 07.03.2019.
//

#ifndef CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOCLIENTSTESTS_H
#define CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOCLIENTSTESTS_H

#include <gtest/gtest.h>

#include <ClientNetworkMock.h>
#include <gmock/gmock-nice-strict.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <MqttSnForwarder.h>
#include "PlaceholderNetworkContext/PlaceholderNetworkContext.h"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::StrictMock;

extern ClientNetworkMock *globalClientNetworkMockObj;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class SendBufferedMessagesToClientsTests : public ::testing::Test {

 public:
  MqttSnForwarder mqttSnForwarder;

  StrictMock<ClientNetworkMock> clientNetworkMock;
  device_address client_network_address;
  PlaceholderContext clientNetworkPlaceholderContext;
  void *clientNetworkContext = &clientNetworkPlaceholderContext;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  virtual void SetUp() {
    device_address client_network_address({0, 0, 0, 0, 0, 0});
    this->client_network_address = client_network_address;

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

    globalClientNetworkMockObj = &clientNetworkMock;


    mqttSnForwarder.clientNetwork.client_network_send = mock_client_network_send;
    mqttSnForwarder.clientNetworkContext = clientNetworkContext;

    ON_CALL(gatewayNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkReceiveBuffer))
        .WillByDefault(Return(1));
    ON_CALL(clientNetworkSendBuffer, isEmpty(&mqttSnForwarder.clientNetworkSendBuffer))
        .WillByDefault(Return(1));
  }
  virtual void TearDown() {
    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
    globalMqttSnFixedSizeRingBufferMock = nullptr;
    globalClientNetworkMockObj = nullptr;
  }

  static uint8_t *defaultMessageDataGenerator(uint8_t *arr, uint16_t len) {
    // TODO anpassen
    memset(arr, 0, len * sizeof(uint8_t));
    for (uint16_t i = 0; i < len; i++) {
      arr[i] = static_cast<uint8_t>(i % UINT8_MAX);
    }
    return arr;
  }

  static MqttSnMessageData generateMqttSnMessageData(uint16_t data_length,
                                              uint8_t *(*msgDataGenerator)(uint8_t *, uint16_t)
                                              = defaultMessageDataGenerator) {
    MqttSnMessageData result = {0};
    uint16_t a = FORWARDER_HEADER_LEN + sizeof(device_address);
    result.data_length = a + data_length;
    msgDataGenerator(&result.data[a], data_length);
    result.data[0] = static_cast<uint8_t>(result.data_length);     // TODO akutell nur bis zu 255 daten
    result.data[1] = Encapsulated_message;
    return result;
  }

  SendBufferedMessagesToClientsTests() {}
  virtual ~SendBufferedMessagesToClientsTests() {}
};

#endif //CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOCLIENTSTESTS_H
