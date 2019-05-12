//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <network/MqttSnClientNetworkInterface.h>
#if defined(WIFIUDP_H) || defined(WITH_UDP_BROADCAST_CLIENT)
#include <WiFiUdp.h>
extern WiFiUDP *clientUdp;
extern WiFiUDP *clientBCUdp;
#elif defined(WITH_UDP_BROADCAST_CLIENT)
#error "Cannot use WITH_UDP_BROADCAST_CLIENT without WIFIUDP_H"
#else
#include <Udp.h>
extern UDP *clientUdp;
#endif

#define CMQTTSNFORWARDER_MQTTSNCLIENTARDUINOUDPNETWORKPROTOCOL "udp"

typedef struct MqttSnClientUdpNetwork_ {
  uint16_t port;
#if defined(WIFIUDP_H) || defined(WITH_UDP_BROADCAST_CLIENT)
  WiFiUDP *unicast_socket;
  WiFiUDP *multicast_socket;
#elif defined(WITH_UDP_BROADCAST_CLIENT)
#error "Cannot use WITH_UDP_BROADCAST_CLIENT without WIFIUDP_H"
#else
  UDP *unicast_socket;
#endif
  char protocol[sizeof("CMQTTSNFORWARDER_MQTTSNCLIENTARDUINOUDPNETWORKPROTOCOL")];
} MqttSnClientUdpNetwork;

int ClientArduinoUdpInit(MqttSnClientNetworkInterface *n, void *context);

int ClientArduinoUdpConnect(MqttSnClientNetworkInterface *n, void *context);

void ClientArduinoUdpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int ClientArduinoUdpReceive(MqttSnClientNetworkInterface *n,
                            MqttSnFixedSizeRingBuffer *receiveBuffer,
                            int32_t timeout_ms,
                            void *context);

int ClientArduinoUdpSend(MqttSnClientNetworkInterface *n,
                         MqttSnFixedSizeRingBuffer *sendBuffer,
                         int32_t timeout_ms,
                         void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_
