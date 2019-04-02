//
// Created by bele on 26.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNUDPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNUDPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../MqttSnGatewayNetworkInterface.h"

#define CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH 1024

typedef struct MqttSnGatewayUdpNetwork {
  int my_socket;
  char *ip;
  int port;
} MqttSnGatewayUdpNetwork;

int GatewayLinuxUdpInit(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context);

void GatewayLinuxUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                           uint32_t timeout_ms,
                           void *context);

int GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                        void *context);

int save_receive_gateway_message_from_udp_socket_into_receive_buffer(
    MqttSnGatewayNetworkInterface *networkInterface,
    MqttSnGatewayUdpNetwork *gatewayUdpNetwork,
    MqttSnFixedSizeRingBuffer *receiveBuffer);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNUDPNETWORK_H
