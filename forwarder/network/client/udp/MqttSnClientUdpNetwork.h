#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../MqttSnClientNetworkInterface.h"
#include "../../../MqttSnFixedSizeRingBuffer.h"
#include <unistd.h>

#define CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_MAX_DATA_LENGTH 1024

typedef struct MqttSnClientUdpNetwork {
  int master_socket;
} MqttSnClientUdpNetwork;

int ClientLinuxUdpInit(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxUdpConnect(MqttSnClientNetworkInterface *n, void *context);

void ClientLinuxUdpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxUdpReceive(MqttSnClientNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                          uint32_t timeout_ms,
                          void *context);

int ClientLinuxUdpSend(MqttSnClientNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                       void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H