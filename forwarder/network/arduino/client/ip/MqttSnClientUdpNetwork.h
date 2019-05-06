//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnClientNetworkInterface.h>
#include <Udp.h>

extern UDP *clientUdp;

typedef struct MqttSnClientUdpNetwork_ {
  uint16_t port;
  UDP *udp;
  char protocol[sizeof("udp")];
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
