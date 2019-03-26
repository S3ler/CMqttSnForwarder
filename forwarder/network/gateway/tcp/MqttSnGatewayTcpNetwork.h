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

#define CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH 1024

typedef struct MqttSnGatewayTcpNetwork {
  int my_socket;
  char *ip;
  int port;
  uint8_t gateway_buffer[CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH];
  uint16_t gateway_buffer_bytes;
} MqttSnGatewayTcpNetwork;

int GatewayLinuxTcpInit(struct MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context);

void GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                           uint32_t timeout_ms,
                           void *context);

int GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                        void *context);

void MqttSnGatewaytNetworkInitReadFdSet(MqttSnGatewayTcpNetwork *clientTcpNetwork, int *max_sd,
                                        fd_set *readfds);

int MqttSnGatewayHandleMasterSocket(MqttSnGatewayTcpNetwork *clientTcpNetwork,
                                    MqttSnFixedSizeRingBuffer *receiveBuffer,
                                    fd_set *readfds);

int save_receive_gateway_message_from_tcp_socket_into_receive_buffer(
    MqttSnGatewayTcpNetwork *gatewayTcpNetwork,
    MqttSnFixedSizeRingBuffer *receiveBuffer);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNTCPNETWORK_H
