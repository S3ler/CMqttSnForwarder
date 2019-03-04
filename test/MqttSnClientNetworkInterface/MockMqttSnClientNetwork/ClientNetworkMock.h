//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENTNETWORKMOCK_H
#define CMQTTSNFORWARDER_CLIENTNETWORKMOCK_H

#include <gmock/gmock-function-mocker.h>
#include "ClientNetworkMockInterface.h"
#include "FakeClientNetworkInterface.h"

using ::testing::_;
using ::testing::Invoke;

class ClientNetworkMock : public ClientNetworkMockInterface {
 public:
  virtual ~ClientNetworkMock(){}
  MOCK_METHOD4(client_network_receive, int(struct MqttSnClientNetworkInterface * n, MqttSnFixedSizeRingBuffer* receiveBuffer, uint32_t timeout_ms, void* context));
  MOCK_METHOD4(client_network_send, int(struct MqttSnClientNetworkInterface * n, MqttSnFixedSizeRingBuffer* sendBuffer, uint32_t timeout_ms, void* context));
  MOCK_METHOD2(client_network_init, int(struct MqttSnClientNetworkInterface* n, void* context));
  MOCK_METHOD2(client_network_connect, int(struct MqttSnClientNetworkInterface* n, void* context));
  MOCK_METHOD2(client_network_disconnect, void(struct MqttSnClientNetworkInterface* n, void* context));

  void DelegateToFake() {
    ON_CALL(*this, client_network_init(_,_))
        .WillByDefault(Invoke(&fake_, &FakeClientNetworkInterface::client_network_init));
  }
 private:
  FakeClientNetworkInterface fake_;  // Keeps an instance of the fake in the mock.
};

#endif //CMQTTSNFORWARDER_CLIENTNETWORKMOCK_H
