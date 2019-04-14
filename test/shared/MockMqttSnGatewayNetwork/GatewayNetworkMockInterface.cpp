//
// Created by SomeDude on 04.03.2019.
//

#include "GatewayNetworkMockInterface.h"
#include "GatewayNetworkMock.h"

extern GatewayNetworkMock *globalGatewayNetworkMockObj;

int mock_gateway_network_receive(struct MqttSnGatewayNetworkInterface *n,
                                 MqttSnFixedSizeRingBuffer *receiveBuffer,
                                 int timout_ms,
                                 void *context) {
  return globalGatewayNetworkMockObj->gateway_network_receive(n, receiveBuffer, timout_ms, context);
}

int mock_gateway_network_send(struct MqttSnGatewayNetworkInterface *n,
                              MqttSnFixedSizeRingBuffer *sendBuffer,
                              int timout_ms,
                              void *context) {
  return globalGatewayNetworkMockObj->gateway_network_send(n, sendBuffer, timout_ms, context);
}

int mock_gateway_network_init(struct MqttSnGatewayNetworkInterface *n, void *context) {
  return globalGatewayNetworkMockObj->gateway_network_init(n, context);
}

int mock_gateway_network_connect(struct MqttSnGatewayNetworkInterface *n, void *context) {
  return globalGatewayNetworkMockObj->gateway_network_connect(n, context);
}

void mock_gateway_network_disconnect(struct MqttSnGatewayNetworkInterface *n, void *context) {
  globalGatewayNetworkMockObj->gateway_network_disconnect(n, context);
}