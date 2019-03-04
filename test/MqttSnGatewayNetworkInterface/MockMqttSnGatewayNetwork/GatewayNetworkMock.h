//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAYNETWORKMOCK_H
#define CMQTTSNFORWARDER_GATEWAYNETWORKMOCK_H

#include "GatewayNetworkMockInterface.h"
#include <cstdint>
#include "../../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "FakeClientNetworkInterface.h"
#include <gmock/gmock-function-mocker.h>

using ::testing::_;
using ::testing::Invoke;

class GatewayNetworkMock : public GatewayNetworkMockInterface {
 public:
  virtual ~GatewayNetworkMock() {}
  MOCK_METHOD4(gateway_network_receive, int(
      struct MqttSnGatewayNetworkInterface *n,
      MqttSnFixedSizeRingBuffer *receiveBuffer,
      uint32_t timout_ms,
      void *context));
  MOCK_METHOD4(gateway_network_send, int(
      struct MqttSnGatewayNetworkInterface *n,
      MqttSnFixedSizeRingBuffer *sendBuffer,
      uint32_t timout_ms,
      void *context));
  MOCK_METHOD2(gateway_network_init, int(
      struct MqttSnGatewayNetworkInterface *n,
      void *context));
  MOCK_METHOD2(gateway_network_connect, int(
      struct MqttSnGatewayNetworkInterface *n,
      void *context));
  MOCK_METHOD2(gateway_network_disconnect, void(
      struct MqttSnGatewayNetworkInterface *n,
      void *context));
  void DelegateToFake() {
    ON_CALL(*this, gateway_network_init(_,_))
        .WillByDefault(Invoke(&fake_, &FakeGatewayNetworkInterface::gateway_network_init));
  }
 private:
  FakeGatewayNetworkInterface fake_;  // Keeps an instance of the fake in the mock.
};

#endif //CMQTTSNFORWARDER_GATEWAYNETWORKMOCK_H
