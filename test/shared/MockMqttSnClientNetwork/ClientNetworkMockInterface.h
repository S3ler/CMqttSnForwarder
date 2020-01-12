//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENTNETWORKMOCKINTERFACE_H
#define CMQTTSNFORWARDER_CLIENTNETWORKMOCKINTERFACE_H

#include <cstdint>
#include <network/MqttSnClientNetworkInterface.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>

class ClientNetworkMockInterface {
 public:
  virtual ~ClientNetworkMockInterface() {}
  virtual int client_network_receive(MqttSnClientNetworkInterface *n,
                                     MqttSnFixedSizeRingBuffer *receiveBuffer,
                                     uint32_t timeout_ms,
                                     void *context) = 0;
  virtual int client_network_send(MqttSnClientNetworkInterface *n,
                                  MqttSnFixedSizeRingBuffer *sendBuffer,
                                  uint32_t timeout_ms,
                                  void *context) = 0;
  virtual int client_network_init(MqttSnClientNetworkInterface *n, void *context) = 0;
  virtual int client_network_connect(MqttSnClientNetworkInterface *, void *context) = 0;
  virtual void client_network_disconnect(MqttSnClientNetworkInterface *, void *context) = 0;

};

int mock_client_network_receive(MqttSnClientNetworkInterface *n,
                                MqttSnFixedSizeRingBuffer *receiveBuffer,
                                int32_t timeout_ms,
                                void *context);
int mock_client_network_send(MqttSnClientNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *sendBuffer,
                             int32_t timeout_ms,
                             void *context);
int mock_client_network_init(MqttSnClientNetworkInterface *n, void *context);
int mock_client_network_connect(MqttSnClientNetworkInterface *n, void *context);
void mock_client_network_disconnect(MqttSnClientNetworkInterface *, void *context);

#endif //CMQTTSNFORWARDER_CLIENTNETWORKMOCKINTERFACE_H
