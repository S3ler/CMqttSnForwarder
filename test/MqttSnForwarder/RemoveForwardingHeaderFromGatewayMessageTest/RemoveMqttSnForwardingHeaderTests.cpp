//
// Created by SomeDude on 06.03.2019.
//

#include "RemoveMqttSnForwardingHeaderTests.h"

TEST_F(RemoveMqttSnForwardingHeaderTests, MinimalNotEncpsulatedMessageGatewayMessageData_ReturnsMinusOne1) {
  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false) + MQTT_SN_MINIMAL_MESSAGE_LENGTH;
  gatewayMessageData.data[0] = gatewayMessageData.data_length;
  gatewayMessageData.data[1] = PINGRESP;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests, ZeroDataLengthGatewayMessageData_ReturnsMinusOne) {
  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests,
       GreaterThanCMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTHDataLengthGatewayMessageData_ReturnsMinusOne) {
  gatewayMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH + 1;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests,
       DataLengthGatewayMessageDataDoesNotEqualToMqttSnMessageLength_ReturnsMinusOne) {

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
  memset(&gatewayMessageData.data, 0, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH);
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false) + MQTT_SN_MINIMAL_MESSAGE_LENGTH;
  gatewayMessageData.data[0] = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), -1);
}

TEST_F(RemoveMqttSnForwardingHeaderTests, DataLengthGatewayMessageDataDoesEqualToMqttSnMessageLength_ReturnsZero) {

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
  gatewayMessageData.data[0] = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
  gatewayMessageData.data[1] = ENCAPSULATED_MESSAGE;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), 0);

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false) + MQTT_SN_MINIMAL_MESSAGE_LENGTH;
  gatewayMessageData.data[0] = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false) + MQTT_SN_MINIMAL_MESSAGE_LENGTH;
  gatewayMessageData.data[1] = ENCAPSULATED_MESSAGE;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), 0);
}

TEST_F(RemoveMqttSnForwardingHeaderTests,
       MQTTSNENCAPSULATIONMESSAGEContainedMqttSnMessageIsGreater255_ReturnsMinusOne) {
  // TODO: no validation if the MQTT-SN Encapsulation Message really contains a MQTT-SN Message
  GTEST_SKIP() << "Not such tests implemented yet.";
}

TEST_F(RemoveMqttSnForwardingHeaderTests, MQTTSNENCAPSULATIONMESSAGEDoesNotContainMqttSnMessage_ReturnsMinusOne) {
  // TODO: no validation if the MQTT-SN Encapsulation Message really contains a MQTT-SN Message
  GTEST_SKIP() << "Not such tests implemented yet.";
}

TEST_F(RemoveMqttSnForwardingHeaderTests, MinimalGatewayMessageData_ReturnsZero) {

  memset(&gatewayMessageData, 0, sizeof(MqttSnMessageData));
  gatewayMessageData.data_length = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
  gatewayMessageData.data[0] = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
  gatewayMessageData.data[1] = ENCAPSULATED_MESSAGE;
  EXPECT_EQ(RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData), 0);
}

TEST_F(RemoveMqttSnForwardingHeaderTests, MinimalGatewayMessageData_HeaderIsRemoved) {
  // TODO: implement me
  GTEST_SKIP() << "Not such tests implemented yet.";
  ASSERT_TRUE(false) << "No output validation tests implemented yet";
}