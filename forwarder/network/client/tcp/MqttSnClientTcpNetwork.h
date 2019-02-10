//
// Created by bele on 27.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>
#include <sys/socket.h>
#include "../../../MqttSnFixedSizeRingBuffer.h"
#include "../../../MqttSnClientNetworkInterface.h"

#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS 10
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH 1024


typedef struct MqttSnClientTcpNetwork {
    int master_socket;
    int client_socket[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS];
    int max_clients;
    int port;
} MqttSnClientTcpNetwork;

int ClientLinuxTcpInit(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxTcpConnect(MqttSnClientNetworkInterface *n, void *context);

void ClientLinuxTcpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxTcpReceive(MqttSnClientNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                          uint32_t timeout_ms,
                          void *context);

int ClientLinuxTcpSend(MqttSnClientNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                       void *context);


void MqttSnClientNetworkInitReadFdSet(const MqttSnClientTcpNetwork *clientTcpNetwork, int *max_sd, fd_set *readfds);

int MqttSnClientHandleMasterSocket(MqttSnClientTcpNetwork *clientTcpNetwork, fd_set *readfds);

void MqttSnClientHandleClientSockets(MqttSnClientTcpNetwork *clientTcpNetwork, MqttSnFixedSizeRingBuffer *receiveBuffer,
                                     fd_set *readfds);

int compareDeviceAddress(int peer_fd, device_address *peer_adress);

void close_client_connection(MqttSnClientTcpNetwork *clientTcpNetwork, int i);

device_address getClientDeviceAddress(int client_file_descriptor);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H
