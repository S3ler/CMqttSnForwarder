//
// Created by bele on 26.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNTCPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNTCPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../MqttSnGatewayNetworkInterface.h"
#include <sys/select.h>

typedef struct MqttSnGatewayTcpNetwork {
    int my_socket;
    char* ip;
    int port;
} MqttSnGatewayTcpNetwork;

int GatewayLinuxTcpInit(struct MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context) ;

void GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) ;

int GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                           uint32_t timeout_ms,
                           void *context) ;

int GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                        void *context) ;

void MqttSnGatewaytNetworkInitReadFdSet(MqttSnGatewayTcpNetwork *clientTcpNetwork, int *max_sd,
                                        fd_set *readfds);

int MqttSnGatewayHandleMasterSocket(MqttSnGatewayTcpNetwork *clientTcpNetwork,
                                    MqttSnFixedSizeRingBuffer *receiveBuffer,
                                    fd_set *readfds) ;

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNTCPNETWORK_H
