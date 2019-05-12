//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAYNETWORKMOCK_H
#define CMQTTSNFORWARDER_GATEWAYNETWORKMOCK_H

#include <gmock/gmock.h>
#include "GatewayNetworkMockInterface.h"

class GatewayNetworkMock : public GatewayNetworkMockInterface {
 public:
  virtual ~GatewayNetworkMock() {}
  MOCK_METHOD4(gateway_network_receive,
  int(
      MqttSnGatewayNetworkInterface
  *n,
  MqttSnFixedSizeRingBuffer *receiveBuffer,
      uint32_t
  timout_ms,
  void *context
  ));
  MOCK_METHOD4(gateway_network_send,
  int(
      MqttSnGatewayNetworkInterface
  *n,
  MqttSnFixedSizeRingBuffer *sendBuffer,
      uint32_t
  timout_ms,
  void *context
  ));
  MOCK_METHOD2(gateway_network_init,
  int(
      MqttSnGatewayNetworkInterface
  *n,
  void *context
  ));
  MOCK_METHOD2(gateway_network_connect,
  int(
      MqttSnGatewayNetworkInterface
  *n,
  void *context
  ));
  MOCK_METHOD2(gateway_network_disconnect,
  void(
      MqttSnGatewayNetworkInterface
  *n,
  void *context
  ));
};

#endif //CMQTTSNFORWARDER_GATEWAYNETWORKMOCK_H
