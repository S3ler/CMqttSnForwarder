#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnClientNetworkInterface.h>

#define CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_MAX_DATA_LENGTH 1024

typedef struct MqttSnClientUdpNetwork_ {
  int master_socket;
  char protocol[4];
} MqttSnClientUdpNetwork;

int ClientLinuxUdpInit(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxUdpConnect(MqttSnClientNetworkInterface *n, void *context);

void ClientLinuxUdpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxUdpReceive(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int32_t timeout_ms,
                          void *context);

int ClientLinuxUdpSend(MqttSnClientNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int32_t timeout_ms,
                       void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H
