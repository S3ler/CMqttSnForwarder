//
// Created by SomeDude on 07.03.2019.
//

#ifndef CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOGATEWAYTESTS_H
#define CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOGATEWAYTESTS_H

#include <gtest/gtest.h>
#include <MqttSnForwarder.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <gmock/gmock-nice-strict.h>
#include <GatewayNetworkMock.h>
#include "PlaceholderNetworkContext/FakeNetworkContext.h"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::StrictMock;

extern GatewayNetworkMock *globalGatewayNetworkMockObj;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

class SendBufferedMessagesToGatewayTests : public testing::Test {
 public:
  MqttSnForwarder mqttSnForwarder;

  StrictMock<GatewayNetworkMock> gatewayNetworkMock;

  device_address gateway_network_address;
  device_address mqtt_sn_gateway_address;
  PlaceholderContext gatewayNetworkPlaceholderContext;
  void *gatewayNetworkContext = &gatewayNetworkPlaceholderContext;

  std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> mqttSnFixedSizeRingBufferMockMap;

  StrictMock<MqttSnFixedSizeRingBufferMock> defaultMqttSnFixedSizeRingBufferMock;

  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> clientNetworkSendBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkReceiveBuffer;
  StrictMock<MqttSnFixedSizeRingBufferMock> gatewayNetworkSendBuffer;

  MqttSnMessageData clientMessageData = {0};
  MqttSnMessageData gatewayMessageData = {0};

  virtual void SetUp() {
    device_address mqtt_sn_gateway_address({0, 0, 0, 0, 0, 0});
    this->mqtt_sn_gateway_address = mqtt_sn_gateway_address;
    device_address gateway_network_address({0, 0, 0, 0, 0, 0});
    this->gateway_network_address = gateway_network_address;

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

    globalGatewayNetworkMockObj = &gatewayNetworkMock;

    mqttSnForwarder.gatewayNetwork.gateway_network_send = mock_gateway_network_send;
    mqttSnForwarder.gatewayNetworkContext = gatewayNetworkContext;

    ON_CALL(clientNetworkReceiveBuffer, isEmpty(&mqttSnForwarder.clientNetworkReceiveBuffer))
        .WillByDefault(Return(1));
    ON_CALL(gatewayNetworkSendBuffer, isEmpty(&mqttSnForwarder.gatewayNetworkSendBuffer))
        .WillByDefault(Return(1));
  }

  virtual void TearDown() {
    globalMqttSnFixedSizeRingBufferMockMap = nullptr;
    globalMqttSnFixedSizeRingBufferMock = nullptr;
    globalGatewayNetworkMockObj = nullptr;
  }

  static uint8_t *defaultMessageDataGenerator(uint8_t *arr, uint16_t len) {
    // TODO anpassen
    memset(arr, 0, len * sizeof(uint8_t));
    for (uint16_t i = 0; i < len; i++) {
      arr[i] = static_cast<uint8_t>(i % UINT8_MAX);
    }
    return arr;
  }

  static MqttSnMessageData generateM1MqttSnPublishMqttSnMessageData(uint16_t data_length,
                                                                    uint8_t *(*msgDataGenerator)(uint8_t *, uint16_t)
                                                                    = defaultMessageDataGenerator) {
    uint32_t MQTT_SN_SHORT_LENGTH = 1;
    uint32_t MQTT_SN_LONG_LENGTH = 3;
    uint32_t MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH = 6;
    uint8_t MQTT_SN_MSG_TYPE_PUBLISH = 0x0C;

    if ((data_length + MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH + MQTT_SN_SHORT_LENGTH) > 255) {

      // generate message with three byte header

      if (data_length > UINT16_MAX - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_LONG_LENGTH) {
        // if the data_length is too big, only create so much data we can
        data_length = UINT16_MAX - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_LONG_LENGTH;
        if (data_length > CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_LONG_LENGTH) {
          data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_LONG_LENGTH;
        }
      }

      MqttSnMessageData result = {0};

      result.data_length =
          static_cast<uint16_t>(data_length + MQTT_SN_LONG_LENGTH + MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH);

      result.data[0] = 0x01;
      result.data[1] = static_cast<uint8_t>(result.data_length >> 8);
      result.data[2] = static_cast<uint8_t>(result.data_length >> 0);
      result.data[3] = MQTT_SN_MSG_TYPE_PUBLISH; // MsgType
      result.data[4] = 0b00100010; //QoS -1 and pre-defined topic id - Flags
      result.data[5] = 0; //MSB TopicId
      result.data[6] = 0; //LSB TopicId
      result.data[7] = 0; //MSB MsgId
      result.data[8] = 0; //LSB MsgId
      msgDataGenerator(&result.data[9], data_length);
      return result;
    }

    // if ((data_length + MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH + MQTT_SN_SHORT_LENGTH) > 255) {}

    // generate message with one byte header

    if (data_length > UINT8_MAX - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_SHORT_LENGTH) {
      // if the data_length is too big, only create so much data we can
      data_length = UINT8_MAX - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_SHORT_LENGTH;
      if (data_length > CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_SHORT_LENGTH) {
        data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH - MQTT_SN_SHORT_LENGTH;
      }
    }

    MqttSnMessageData result = {0};

    result.data_length =
        static_cast<uint16_t>(data_length + MQTT_SN_SHORT_LENGTH + MQTT_SN_M1_PUBLISH_HEADER_LENGTH_WITHOUT_LENGTH);

    result.data[0] = static_cast<uint8_t>(result.data_length);
    result.data[MQTT_SN_SHORT_LENGTH+0] = MQTT_SN_MSG_TYPE_PUBLISH; // MsgType
    result.data[MQTT_SN_SHORT_LENGTH+1] = 0b00100010; //QoS -1 and pre-defined topic id - Flags
    result.data[MQTT_SN_SHORT_LENGTH+2] = 0; //MSB TopicId
    result.data[MQTT_SN_SHORT_LENGTH+3] = 0; //LSB TopicId
    result.data[MQTT_SN_SHORT_LENGTH+4] = 0; //MSB MsgId
    result.data[MQTT_SN_SHORT_LENGTH+5] = 0; //LSB MsgId
    msgDataGenerator(&result.data[MQTT_SN_SHORT_LENGTH+6], data_length);
    return result;

  }

  SendBufferedMessagesToGatewayTests() {};
  virtual ~SendBufferedMessagesToGatewayTests() {}

};

#endif //CMQTTSNFORWARDER_SENDBUFFEREDMESSAGESTOGATEWAYTESTS_H
