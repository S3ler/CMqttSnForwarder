//
// Created by SomeDude on 06.03.2019.
//

#include "AddMqttSnForwardingHeaderTests.h"

TEST_F(AddMqttSnForwardingHeaderTests, ZeroDataLengthClientMessageData_ReturnsMinusOne
) {
memset(&clientMessageData,
0, sizeof(MqttSnMessageData));
EXPECT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
-1);
}

TEST_F(AddMqttSnForwardingHeaderTests, ZeroDataLengthClientMessageDataAndReturnsMinusOne_ClientMessageDataNotModified
) {
memset(&clientMessageData,
0, sizeof(MqttSnMessageData));
ASSERT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
-1);
MqttSnMessageData mqttSnMessageDataZeroed = {0};
EXPECT_EQ(memcmp(&clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)),
0);
}

TEST_F(AddMqttSnForwardingHeaderTests,
    ZeroDataLengthClientMessageDataAndReturnsMinusOne_GatewayMessageDataNotModified
) {
memset(&gatewayMessageData,
0, sizeof(MqttSnMessageData));
ASSERT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
-1);
MqttSnMessageData mqttSnMessageDataZeroed = {0};
EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)),
0);
}

TEST_F(AddMqttSnForwardingHeaderTests, MinimumDataLengthClientMessageData_ReturnsZero
) {
clientMessageData.
data_length = 1;
EXPECT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
0);
}

TEST_F(AddMqttSnForwardingHeaderTests, MaximumDataLengthClientMessageData_ReturnsZero
) {
clientMessageData.
data_length =
CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false);
EXPECT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
0);
}

TEST_F(AddMqttSnForwardingHeaderTests, AboveMaximumDataLengthClientMessageData_ReturnsMinusOne
) {
clientMessageData.
data_length =
CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false) + 1;
EXPECT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
-1);
}

TEST_F(AddMqttSnForwardingHeaderTests,
    AboveMaximumDataLengthClientMessageDataAndReturnsMinusOne_ClientMessageDataNotModified
) {
clientMessageData.
data_length =
CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false) + 1;
MqttSnMessageData expectedMqttSnMessageData = {0};
memcpy(&expectedMqttSnMessageData, &clientMessageData,
sizeof(MqttSnMessageData));

ASSERT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
-1);
EXPECT_EQ(memcmp(&clientMessageData, &expectedMqttSnMessageData, sizeof(MqttSnMessageData)),
0);
}

TEST_F(AddMqttSnForwardingHeaderTests,
    AboveMaximumDataLengthClientMessageDataAndReturnsMinusOne_GatewayMessageDataNotModified
) {
memset(&gatewayMessageData,
0, sizeof(MqttSnMessageData));
clientMessageData.
data_length =
CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false) + 1;
ASSERT_EQ(AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData),
-1);
MqttSnMessageData mqttSnMessageDataZeroed = {0};
EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)),
0);
}

TEST_F(AddMqttSnForwardingHeaderTests, MinimumDataLengthClientMessageData_HeaderIsAddedCorrectly
) {
// TODO: implement me
GTEST_SKIP()
<< "Not such tests implemented yet.";
ASSERT_TRUE(false) << "No output validation tests implemented yet";
}
