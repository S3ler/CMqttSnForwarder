//
// Created by SomeDude on 07.03.2019.
//

#include "MqttSnForwarderInitTests.h"

TEST_F(MqttSnForwarderInitTests, ClientNetworkContextIsSet
) {
MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);

EXPECT_EQ(mqttSnForwarder
.clientNetworkContext, clientNetworkContext);
}

TEST_F(MqttSnForwarderInitTests, ClientNetworkReceiveBufferInitIsCalled
) {
EXPECT_CALL(clientNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkReceiveBuffer)
)
.Times(1);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, ClientNetworkSendBufferInitIsCalled
) {
EXPECT_CALL(clientNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkSendBuffer)
)
.Times(1);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, GatewayNetworkContextIsSet
) {
MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);

EXPECT_EQ(mqttSnForwarder
.gatewayNetworkContext, gatewayNetworkContext);
}
TEST_F(MqttSnForwarderInitTests, GatewayNetworkReceiveBufferInitIsCalled
) {
EXPECT_CALL(gatewayNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkReceiveBuffer)
)
.Times(1);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, GatewayNetworkSendBufferInitIsCalled
) {
EXPECT_CALL(gatewayNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, ClientNetworkConnectedIsCalled
) {
EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1)
.
WillOnce(Return(0)
);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, ClientNetworkConnectFailed_MqttSnForwarderDeinitialized
) {
EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1);
EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, ClientNetworkConnectFailedAndMqttSnForwarderDeinitialized_ReturnsMinusOne
) {
EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_EQ(MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext),
-1);
}

TEST_F(MqttSnForwarderInitTests, GatewayNetworkConnectedIsCalled
) {
EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1)
.
WillOnce(Return(0)
);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, GatewayNetworkConnectFailed_MqttSnForwarderDeinitialized
) {
EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1);
EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1);

MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext
);
}

TEST_F(MqttSnForwarderInitTests, GatewayNetworkConnectFailedAndMqttSnForwarderDeinitialized_ReturnsMinusOne
) {
EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1)
.
WillOnce(Return(-1)
);

EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1);
EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1);

EXPECT_EQ(MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext),
-1);
}

TEST_F(MqttSnForwarderInitTests, ContextsAndBuffersInitializedAndNetworksConnected_ReturnsZero
) {

EXPECT_CALL(clientNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkReceiveBuffer)
)
.Times(1);
EXPECT_CALL(clientNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.clientNetworkSendBuffer)
)
.Times(1);
EXPECT_CALL(gatewayNetworkReceiveBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkReceiveBuffer)
)
.Times(1);
EXPECT_CALL(gatewayNetworkSendBuffer, MqttSnFixedSizeRingBufferInit(&mqttSnForwarder.gatewayNetworkSendBuffer)
)
.Times(1);

EXPECT_CALL(clientNetworkMock, client_network_connect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1)
.
WillOnce(Return(0)
);
EXPECT_CALL(gatewayNetworkMock, gateway_network_connect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1)
.
WillOnce(Return(0)
);

int returnValue = MqttSnForwarderInit(&mqttSnForwarder, LOG_LEVEL_QUIET, clientNetworkContext, gatewayNetworkContext);

ASSERT_EQ(mqttSnForwarder
.gatewayNetworkContext, gatewayNetworkContext);
ASSERT_EQ(mqttSnForwarder
.clientNetworkContext, clientNetworkContext);

EXPECT_EQ(returnValue,
0);
}
