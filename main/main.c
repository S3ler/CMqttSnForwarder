#include <stdio.h>
#include "../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../forwarder/MqttSnForwarder.h"
#include "../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"
#include "../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"

int main() {

    MqttSnForwarder mqttSnForwarder;

    MqttSnGatewayTcpNetwork gatewayNetworkContext = {0};
    gatewayNetworkContext.my_socket = 0;
    gatewayNetworkContext.ip = "127.0.0.1";
    gatewayNetworkContext.port = 8888;
    GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork, &gatewayNetworkContext, GatewayLinuxTcpInit);


    MqttSnClientTcpNetwork clientNetworkContext = {0};
    uint16_t clientNetworkPort = 9999;
    device_address clientNetworkAddress = {127,0,0,1, (uint8_t) clientNetworkPort >> 8, (uint8_t) clientNetworkPort >> 0};
    ClientNetworkInit(&mqttSnForwarder.clientNetwork, &clientNetworkAddress, &clientNetworkContext, ClientLinuxTcpInit);

    MqttSnForwarderStart (&mqttSnForwarder, &clientNetworkContext, &gatewayNetworkContext);

    return 0;
}

