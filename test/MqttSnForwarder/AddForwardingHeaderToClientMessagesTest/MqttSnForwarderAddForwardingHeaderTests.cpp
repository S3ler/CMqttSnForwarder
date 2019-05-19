//
// Created by SomeDude on 06.03.2019.
//

#include "MqttSnForwarder/MqttSnForwarderTests.h"

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_CallsPopOnClientNetworkReceiveBuffer
) {

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Return(-1)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL
);
}

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsMinusOne_OnlyPopIsCalled
) {

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _)
)
.Times(0);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL
);
}

TEST_F(MqttSnForwarderTests, MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsMinusOne_ReturnsZero
) {

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _)
)
.Times(0);

EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL),
0);
}

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataIsZeroed_PutIsNotCalled
) {

auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  memcpy(clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData));
  return 0;
};

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Invoke(f)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _)
)
.Times(0);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL
);
}

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataIsZeroed_ReturnsZero
) {

auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
  MqttSnMessageData mqttSnMessageDataZeroed = {0};
  memcpy(clientMessageData, &mqttSnMessageDataZeroed, sizeof(MqttSnMessageData));
  return 0;
};

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Invoke(f)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _)
)
.Times(0);

EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL),
0);
}

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataDataLengthIsOne_PutIsCalled
) {

auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
  MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
  mqttSnMessageDataDataLengthOne.data_length = 1;
  memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
  return 0;
};

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Invoke(f)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _)
)
.Times(1)
.
WillOnce(Return(0)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL
), 0;
}

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroAndClientMessageDataDataLengthIsOneAndPutIsCalled_ReturnsZero
) {

auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
  MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
  mqttSnMessageDataDataLengthOne.data_length = 1;
  memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
  return 0;
};

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Invoke(f)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _)
)
.Times(1)
.
WillOnce(Return(0)
);

EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL),
0);
}

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroPutReturnsMinusOne_MessageIsPutBackIntoClientNetworkReceiveBuffer
) {

auto f = [](MqttSnFixedSizeRingBuffer *clientNetworkReceiveBuffer, MqttSnMessageData *clientMessageData) -> int {
  MqttSnMessageData mqttSnMessageDataDataLengthOne = {0};
  mqttSnMessageDataDataLengthOne.data_length = 1;
  memcpy(clientMessageData, &mqttSnMessageDataDataLengthOne, sizeof(MqttSnMessageData));
  return 0;
};

EXPECT_CALL(clientNetworkReceiveBuffer, pop(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Invoke(f)
);

EXPECT_CALL(gatewayNetworkSendBuffer, put(&mqttSnForwarder.gatewayNetworkSendBuffer, _)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(clientNetworkReceiveBuffer, put(&mqttSnForwarder.clientNetworkReceiveBuffer, _)
)
.Times(1)
.
WillOnce(Return(0)
);

AddForwardingHeaderToClientMessages(&mqttSnForwarder, NULL, NULL
);
}

TEST_F(MqttSnForwarderTests,
    MqttSnForwarderTests_AddForwardingHeaderToClientMessages_PopReturnsZeroPutReturnsMinusOne_MessageIsPutBackIntoClientNetworkReceiveBuffer_ReturnsZero
) {

MqttSnMessageData clientMessageData = {0};
MqttSnMessageData gatewayMessageData = {0};

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
WillOnce(Return(0)
);

EXPECT_EQ(AddForwardingHeaderToClientMessages(&mqttSnForwarder, &clientMessageData, &gatewayMessageData),
0);
}


