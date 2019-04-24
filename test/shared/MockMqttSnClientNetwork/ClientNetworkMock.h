//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENTNETWORKMOCK_H
#define CMQTTSNFORWARDER_CLIENTNETWORKMOCK_H

#include <gmock/gmock-function-mocker.h>
#include "ClientNetworkMockInterface.h"

using ::testing::_;
using ::testing::Invoke;

class ClientNetworkMock : public ClientNetworkMockInterface {
 public:
  virtual ~ClientNetworkMock() {}
  MOCK_METHOD4(client_network_receive, int(
       MqttSnClientNetworkInterface *n,
      MqttSnFixedSizeRingBuffer *receiveBuffer,
      uint32_t timeout_ms,
      void *context));
  MOCK_METHOD4(client_network_send, int(
       MqttSnClientNetworkInterface *n,
      MqttSnFixedSizeRingBuffer *sendBuffer,
      uint32_t timeout_ms,
      void *context));
  MOCK_METHOD2(client_network_init, int(
       MqttSnClientNetworkInterface *n,
      void *context));
  MOCK_METHOD2(client_network_connect, int(
       MqttSnClientNetworkInterface *n,
      void *context));
  MOCK_METHOD2(client_network_disconnect, void(
       MqttSnClientNetworkInterface *n,
      void *context));
};

#endif //CMQTTSNFORWARDER_CLIENTNETWORKMOCK_H
