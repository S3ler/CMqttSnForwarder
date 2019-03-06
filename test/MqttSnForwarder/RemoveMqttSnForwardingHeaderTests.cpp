//
// Created by SomeDude on 06.03.2019.
//

#include "RemoveMqttSnForwardingHeaderTests.h"


TEST_F(RemoveMqttSnForwardingHeaderTests, ZeroDataLengthGatewayMessageData_ReturnsMinusOne) {
  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests,
       GreaterThanCLIENT_NETWORK_MAX_DATA_LENDataLengthGatewayMessageData_ReturnsMinusOne) {
  if (CLIENT_NETWORK_MAX_DATA_LEN >= UINT16_MAX) {
    ASSERT_TRUE(false)
                  << "GreaterThanCLIENT_NETWORK_MAX_DATA_LENDataLengthGatewayMessageData_ReturnsMinusOne is not testable.";
  }
  gatewayMessageData.data_length = CLIENT_NETWORK_MAX_DATA_LEN + 1;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests,
       SmallerThanCLIENT_NETWORK_MAX_DATA_LENDataLengthGatewayMessageData_ReturnsMinusOne) {
  gatewayMessageData.data_length = FORWARDER_HEADER_LEN + sizeof(device_address) - 1;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests,
       DataLengthGatewayMessageDataDoesNotEqualToMqttSnMessageLength_ReturnsMinusOne) {

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = FORWARDER_HEADER_LEN + sizeof(device_address);
  memset(&gatewayMessageData.data, 0, MAX_MESSAGE_LENGTH);
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = FORWARDER_HEADER_LEN + sizeof(device_address) + 2;
  gatewayMessageData.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address);
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests, DataLengthGatewayMessageDataDoesEqualToMqttSnMessageLength_ReturnsZero) {

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = FORWARDER_HEADER_LEN + sizeof(device_address);
  gatewayMessageData.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address);
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), 0);

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = FORWARDER_HEADER_LEN + sizeof(device_address) + 2;
  gatewayMessageData.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address) + 2;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), 0);
}

TEST_F(RemoveMqttSnForwardingHeaderTests, MQTTSNENCAPSULATIONMESSAGEContainedMqttSnMessageIsGreater255_ReturnsMinusOne) {
  // TODO: no validation if the MQTT-SN Encapsulation Message really contains a MQTT-SN Message
  GTEST_SKIP() << "Not such tests implemented yet.";
}

TEST_F(RemoveMqttSnForwardingHeaderTests, MQTTSNENCAPSULATIONMESSAGEDoesNotContainMqttSnMessage_ReturnsMinusOne) {
  // TODO: no validation if the MQTT-SN Encapsulation Message really contains a MQTT-SN Message
  GTEST_SKIP() << "Not such tests implemented yet.";
}

TEST_F(RemoveMqttSnForwardingHeaderTests, MinimalGatewayMessageData_ReturnsZero) {

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = FORWARDER_HEADER_LEN + sizeof(device_address);
  gatewayMessageData.data[0] = FORWARDER_HEADER_LEN + sizeof(device_address);
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), 0);
}

TEST_F(RemoveMqttSnForwardingHeaderTests, MinimalGatewayMessageData_HeaderIsRemoved) {
  // TODO: implement me
  GTEST_SKIP() << "Not such tests implemented yet.";
}