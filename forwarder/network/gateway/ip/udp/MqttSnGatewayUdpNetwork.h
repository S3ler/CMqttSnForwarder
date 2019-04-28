//
// Created by bele on 26.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnGatewayNetworkInterface.h>

#define CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH 1024

typedef struct MqttSnGatewayUdpNetwork_ {
  int my_socket;
  char *ip;
  int port;
  char protocol[4];
} MqttSnGatewayUdpNetwork;

int GatewayLinuxUdpInit(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context);

void GatewayLinuxUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                           int32_t timeout_ms,
                           void *context);

int GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n,
                        MqttSnFixedSizeRingBuffer *sendBuffer,
                        int32_t timeout_ms,
                        void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H
