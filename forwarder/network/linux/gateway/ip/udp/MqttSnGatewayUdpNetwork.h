//
// Created by bele on 26.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnGatewayNetworkInterface.h>

#define CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXUDPNETWORKPROTOCOL "udp"
#define CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH 1024

// TODO save delete
typedef struct MqttSnGatewayUdpNetwork_ {
  int unicast_socket;
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXUDPNETWORKPROTOCOL)];
#ifdef WITH_UDP_BROADCAST
  int multicast_socket;
#endif
} MqttSnGatewayUdpNetwork;

int GatewayLinuxUdpInit(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *n, void *context);

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
