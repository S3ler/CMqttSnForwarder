//
// Created by SomeDude on 07.03.2019.
//

#include "MqttSnForwarderDeinitTests.h"

TEST_F(MqttSnForwarderDeinitTests, ClientNetworkDisconnected
) {
NiceMock<GatewayNetworkMock> gatewayNetworkMock;
globalGatewayNetworkMockObj = &gatewayNetworkMock;

EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1);

MqttSnForwarderDeinit(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderDeinitTests, GatewayNetworkDisconnected
) {
NiceMock<ClientNetworkMock> clientNetworkMock;
globalClientNetworkMockObj = &clientNetworkMock;

EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1);

MqttSnForwarderDeinit(&mqttSnForwarder);
}

TEST_F(MqttSnForwarderDeinitTests, GatewayAndClientNetworkDisconnected
) {
EXPECT_CALL(gatewayNetworkMock, gateway_network_disconnect(&mqttSnForwarder.gatewayNetwork, gatewayNetworkContext)
)
.Times(1);
EXPECT_CALL(clientNetworkMock, client_network_disconnect(&mqttSnForwarder.clientNetwork, clientNetworkContext)
)
.Times(1);

MqttSnForwarderDeinit(&mqttSnForwarder);
}