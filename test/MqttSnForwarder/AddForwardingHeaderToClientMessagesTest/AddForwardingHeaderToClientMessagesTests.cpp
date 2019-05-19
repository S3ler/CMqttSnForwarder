//
// Created by SomeDude on 06.03.2019.
//

#include "AddForwardingHeaderToClientMessagesTests.h"
#include <test/shared/ComparableMqttSnForwaderStructures/ComparableMqttSnMessageData.h>

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_ReturnsZero
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData),
0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_GatewayMessageIsZeroed
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData
);

MqttSnMessageData mqttSnMessageDataZeroed = {0};
EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)),
0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_ClientMessageIsZeroed
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData
);

MqttSnMessageData mqttSnMessageDataZeroed = {0};
EXPECT_EQ(memcmp(&clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)),
0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_ClientNetworkReceiveBufferIsEmpty_GatewayMessageIsNotCreated
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData
);

MqttSnMessageData mqttSnMessageDataZeroed = {0};
EXPECT_EQ(memcmp(&gatewayMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData)),
0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_ClientMessageIsEmpty_MinimalGatewayMessageIsCreated
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Invoke(
    [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer,
       MqttSnMessageData *clientMessageData) -> int {
      MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
      mqttSnMessageDataDataLengthOne.data_length = 0;
      memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
      return 0;
    }
)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData)
)
.Times(1)
.
WillOnce(Return(0)
);

ASSERT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData),
0);
ComparableMqttSnMessageData actualComparableMqttSnData(gatewayMessageData);

MqttSnMessageData mqttSnMessageDataZeroed = {0};
mqttSnMessageDataZeroed.
data_length = 3 + sizeof(device_address);
mqttSnMessageDataZeroed.data[0] = mqttSnMessageDataZeroed.
data_length;
mqttSnMessageDataZeroed.data[1] = 0xFE;
ComparableMqttSnMessageData expectedComparableMqttSnData(mqttSnMessageDataZeroed);

EXPECT_EQ(actualComparableMqttSnData
.data_length, expectedComparableMqttSnData.data_length);
EXPECT_EQ(actualComparableMqttSnData
.address, expectedComparableMqttSnData.address);
EXPECT_EQ(actualComparableMqttSnData
.data, expectedComparableMqttSnData.data);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_GatewayMessageIsCreated_GatewayMessageIsPutIntoGatewayNetworkSendBuffer
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Invoke(
    [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
      MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
      mqttSnMessageDataDataLengthOne.data_length = 1;
      memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
      return 0;
    }
)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(0);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData
);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_GatewayMessageIsCreatedAndGatewayMessageIsPutIntoGatewayNetworkSendBuffer_ReturnsZero
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Invoke(
    [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
      MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
      mqttSnMessageDataDataLengthOne.data_length = 1;
      memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
      return 0;
    }
)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(0);

EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData),
0);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_GatewayNetworkSendBufferIsFull_MessageIsPutBackIntoClientNetworkReceiveBuffer
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Invoke(
    [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
      MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
      mqttSnMessageDataDataLengthOne.data_length = 1;
      memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
      return 0;
    }
)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Return(0)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData
);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_GatewayNetworkSendBufferIsFullAndMessageIsPutBackIntoClientNetworkReceiveBuffer_PutBackReturnValueIsNotReturned
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

auto setMqttSnMessageDataDataLengthToOne =
    [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
      MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
      mqttSnMessageDataDataLengthOne.data_length = 1;
      memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
      return 0;
    };

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Invoke(setMqttSnMessageDataDataLengthToOne)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData
);
}

TEST_F(AddForwardingHeaderToClientMessagesTests,
    AddForwardingHeaderTests_GatewayNetworkSendBufferIsFull_ReturnsZero
) {

EXPECT_CALL(gatewayNetworkSendBuffer, isFull(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Invoke(
    [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
      MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
      mqttSnMessageDataDataLengthOne.data_length = 1;
      memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
      return 0;
    }
)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, &gatewayMessageData)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, &clientMessageData)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData),
0);
}
