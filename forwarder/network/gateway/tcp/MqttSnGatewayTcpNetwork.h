//
// Created by bele on 26.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../MqttSnGatewayNetworkInterface.h"
#include <sys/select.h>

#define CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH 1024

typedef struct MqttSnGatewayTcpNetwork {
  int mqtt_sg_gateway_fd;
  uint8_t gateway_buffer[CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH];
  uint16_t gateway_buffer_bytes;
} MqttSnGatewayTcpNetwork;

int GatewayLinuxTcpInit(struct MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context);

void GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                           int32_t timeout_ms,
                           void *context);

int GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n,
                        MqttSnFixedSizeRingBuffer *sendBuffer,
                        int32_t timeout_ms,
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

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_H
