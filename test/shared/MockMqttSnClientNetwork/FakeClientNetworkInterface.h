//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_FAKECLIENTNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_FAKECLIENTNETWORKINTERFACE_H

#include "ClientNetworkMockInterface.h"
#include "MqttSnClientNetworkInterface.h"

class FakeClientNetworkInterface : public ClientNetworkMockInterface {
 public:
  virtual int client_network_receive(struct MqttSnClientNetworkInterface *n,
                                     MqttSnFixedSizeRingBuffer *receiveBuffer,
                                     uint32_t timeout_ms,
                                     void *context) {
    return 0;
  }
  virtual int client_network_send(struct MqttSnClientNetworkInterface *n,
                                  MqttSnFixedSizeRingBuffer *sendBuffer,
                                  uint32_t timeout_ms,
                                  void *context) {
    return 0;
  }
  virtual int client_network_init(struct MqttSnClientNetworkInterface *n, void *context) {
    n->client_network_init = mock_client_network_init;
    n->client_network_connect = mock_client_network_connect;
    n->client_network_disconnect = mock_client_network_disconnect;
    n->client_network_receive = mock_client_network_receive;
    n->client_network_send = mock_client_network_send;
    return 0;
  }
  virtual int client_network_connect(struct MqttSnClientNetworkInterface *n, void *context) {
    return 0;
  }
  virtual void client_network_disconnect(struct MqttSnClientNetworkInterface *n, void *context) {
  }
};

#endif //CMQTTSNFORWARDER_FAKECLIENTNETWORKINTERFACE_H
