//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnGatewayNetworkInterface.h>
#include <Udp.h>

extern UDP *gatewayUdp;

typedef struct MqttSnGatewayUdpNetwork_ {
  uint16_t port;
  UDP *udp;
  char protocol[sizeof("udp")];
} MqttSnGatewayUdpNetwork;

int GatewayArduinoUdpInit(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayArduinoUdpConnect(MqttSnGatewayNetworkInterface *n, void *context);

void GatewayArduinoUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayArduinoUdpReceive(MqttSnGatewayNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *receiveBuffer,
                             int32_t timeout_ms,
                             void *context);

int GatewayArduinoUdpSend(MqttSnGatewayNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *sendBuffer,
                          int32_t timeout_ms,
                          void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_
