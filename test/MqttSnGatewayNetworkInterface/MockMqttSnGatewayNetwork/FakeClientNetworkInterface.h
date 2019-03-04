//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_FAKEGATEWAYNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_FAKEGATEWAYNETWORKINTERFACE_H

#include <cstdint>
#include "GatewayNetworkMockInterface.h"
#include "../../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../../../forwarder/MqttSnGatewayNetworkInterface.h"

class FakeGatewayNetworkInterface : public GatewayNetworkMockInterface {
 public:
  virtual int gateway_network_receive(struct MqttSnGatewayNetworkInterface *n,
                                      MqttSnFixedSizeRingBuffer *receiveBuffer,
                                      uint32_t timeout_ms,
                                      void *context) {
    return 0;
  }
  virtual int gateway_network_send(struct MqttSnGatewayNetworkInterface *n,
                                   MqttSnFixedSizeRingBuffer *sendBuffer,
                                   uint32_t timeout_ms,
                                   void *context) {
    return 0;
  }
  virtual int gateway_network_init(struct MqttSnGatewayNetworkInterface *n, void *context) {
    n->gateway_receive = mock_gateway_network_receive;
    n->gateway_send = mock_gateway_network_send;
    n->gateway_network_init = mock_gateway_network_init;
    n->gateway_network_connect = mock_gateway_network_connect;
    n->gateway_network_disconnect = mock_gateway_network_disconnect;
    return 0;
  }
  virtual int gateway_network_connect(struct MqttSnGatewayNetworkInterface *n, void *context) {
    return 0;
  }
  virtual void gateway_network_disconnect(struct MqttSnGatewayNetworkInterface *n, void *context) {
  }
};

#endif //CMQTTSNFORWARDER_FAKEGATEWAYNETWORKINTERFACE_H
