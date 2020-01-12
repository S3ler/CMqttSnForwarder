//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAYNETWORKMOCKINTERFACE_H
#define CMQTTSNFORWARDER_GATEWAYNETWORKMOCKINTERFACE_H

#include <network/MqttSnGatewayNetworkInterface.h>

class GatewayNetworkMockInterface {
 public:
  virtual ~GatewayNetworkMockInterface() {}
  virtual int gateway_network_receive(MqttSnGatewayNetworkInterface *n,
                                      MqttSnFixedSizeRingBuffer *receiveBuffer,
                                      uint32_t timout_ms,
                                      void *context) = 0;
  virtual int gateway_network_send(MqttSnGatewayNetworkInterface *n,
                                   MqttSnFixedSizeRingBuffer *sendBuffer,
                                   uint32_t timout_ms,
                                   void *context) = 0;
  virtual int gateway_network_init(MqttSnGatewayNetworkInterface *n, void *context) = 0;
  virtual int gateway_network_connect(MqttSnGatewayNetworkInterface *n, void *context) = 0;
  virtual void gateway_network_disconnect(MqttSnGatewayNetworkInterface *n, void *context) = 0;
};

int mock_gateway_network_receive(MqttSnGatewayNetworkInterface *n,
                                 MqttSnFixedSizeRingBuffer *receiveBuffer,
                                 int timout_ms,
                                 void *context);
int mock_gateway_network_send(MqttSnGatewayNetworkInterface *n,
                              MqttSnFixedSizeRingBuffer *sendBuffer,
                              int timout_ms,
                              void *context);
int mock_gateway_network_init(MqttSnGatewayNetworkInterface *n, void *context);
int mock_gateway_network_connect(MqttSnGatewayNetworkInterface *n, void *context);
void mock_gateway_network_disconnect(MqttSnGatewayNetworkInterface *n, void *context);

#endif //CMQTTSNFORWARDER_GATEWAYNETWORKMOCKINTERFACE_H
