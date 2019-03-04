//
// Created by SomeDude on 03.03.2019.
//

#include "ClientNetworkMockInterface.h"
#include "ClientNetworkMock.h"

extern ClientNetworkMock *globalClientNetworkMockObj;

int mock_client_network_receive(struct MqttSnClientNetworkInterface *n,
                                MqttSnFixedSizeRingBuffer *receiveBuffer,
                                uint32_t timeout_ms,
                                void *context) {
  return globalClientNetworkMockObj->client_network_receive(n, receiveBuffer, timeout_ms, context);
}

int mock_client_network_send(struct MqttSnClientNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *sendBuffer,
                             uint32_t timeout_ms,
                             void *context) {
  return globalClientNetworkMockObj->client_network_send(n, sendBuffer, timeout_ms, context);
}

int mock_client_network_init(struct MqttSnClientNetworkInterface *n, void *context) {
  return globalClientNetworkMockObj->client_network_init(n, context);
}

int mock_client_network_connect(struct MqttSnClientNetworkInterface *n, void *context) {
  return globalClientNetworkMockObj->client_network_connect(n, context);
}

void mock_client_network_disconnect(struct MqttSnClientNetworkInterface *n, void *context) {
  globalClientNetworkMockObj->client_network_disconnect(n, context);
}
