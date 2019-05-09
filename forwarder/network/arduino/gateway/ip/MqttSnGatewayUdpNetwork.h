//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnGatewayNetworkInterface.h>
#if defined(WIFIUDP_H) || defined(WITH_UDP_BROADCAST)
#include <WiFiUdp.h>
extern WiFiUDP *gatewayUdp;
#elif defined(WITH_UDP_BROADCAST)
#error "Cannot use WITH_UDP_BROADCAST without WIFIUDP_H"
#else
#include <Udp.h>
extern UDP *gatewayUdp;
#endif

#define CMQTTSNFORWARDER_MQTTSNGATEWAYARDUINOUDPNETWORKPROTOCOL "udp"

typedef struct MqttSnGatewayUdpNetwork_ {
  uint16_t port;
#ifdef WITH_UDP_BROADCAST
  WiFiUDP *udp;
#else
  UDP *udp;
#endif
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNGATEWAYARDUINOUDPNETWORKPROTOCOL)];
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
