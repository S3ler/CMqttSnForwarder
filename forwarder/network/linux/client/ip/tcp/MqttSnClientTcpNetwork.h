//
// Created by bele on 27.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnClientNetworkInterface.h>
#include <sys/select.h>
#include <forwarder/network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>

#define CMQTTSNFORWARDER_MQTTSNCLIENTLINUXTCPNETWORKPROTOCOL "tcp"
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS 2
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH 1024

#define CMQTTSNFORWARDER_MAXIMUM_PENDING_CONNECTIONS 3

typedef struct MqttSnClientTcpNetwork_ {
  int listen_socket_fd;
  int client_socket_fds[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS];
  uint8_t client_buffer[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS]
  [CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH];
  uint16_t client_buffer_bytes[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS];
  int max_clients;
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNCLIENTLINUXTCPNETWORKPROTOCOL)];
#ifdef WITH_TCP_BROADCAST
  MqttSnClientUdpNetwork udp_multicast_network;
#endif
} MqttSnClientTcpNetwork;

int ClientLinuxTcpInit(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxTcpConnect(MqttSnClientNetworkInterface *n, void *context);

void ClientLinuxTcpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxTcpReceive(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int32_t timeout_ms,
                          void *context);

int ClientLinuxTcpSend(MqttSnClientNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int32_t timeout_ms,
                       void *context);

int MqttSnClientHandleMasterSocket(MqttSnClientNetworkInterface *n, MqttSnClientTcpNetwork *tcpNetwork);

void MqttSnClientHandleClientSockets(MqttSnClientNetworkInterface *n,
                                     MqttSnClientTcpNetwork *clientTcpNetwork,
                                     MqttSnFixedSizeRingBuffer *receiveBuffer,
                                     fd_set *read_fds);

void close_client_connection(MqttSnClientNetworkInterface *n, MqttSnClientTcpNetwork *tcpNetwork, int i);

int save_received_messages_from_tcp_socket_into_receive_buffer(MqttSnClientTcpNetwork *tcpNetwork,
                                                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                               int client_socket_position);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H
