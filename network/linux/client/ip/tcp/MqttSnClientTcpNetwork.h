//
// Created by bele on 27.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/MqttSnClientNetworkInterface.h>
#include <sys/select.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>

#define CMQTTSNFORWARDER_MQTTSNCLIENTLINUXTCPNETWORKPROTOCOL "tcp"
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_TCP_ENDPOINTS 2
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH 1024

#define CMQTTSNFORWARDER_MAXIMUM_PENDING_CONNECTIONS 3

typedef struct MqttSnClientTcpNetwork_ {
  int listen_socket_fd;
  int client_socket_fds[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_TCP_ENDPOINTS];
  uint8_t client_buffer[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_TCP_ENDPOINTS]
  [CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH];
  uint16_t client_buffer_bytes[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_TCP_ENDPOINTS];
  uint32_t client_to_drop_bytes[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_TCP_ENDPOINTS];
  uint64_t received_messages;
  int max_clients;
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNCLIENTLINUXTCPNETWORKPROTOCOL)];
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  MqttSnClientUdpNetwork udp_multicast_network;
#endif
} MqttSnClientTcpNetwork;

int32_t ClientLinuxTcpInitialize(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientLinuxTcpDeinitialize(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientLinuxTcpConnect(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientLinuxTcpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientLinuxTcpSend(MqttSnClientNetworkInterface *n,
                           const device_address *from,
                           const device_address *to,
                           const uint8_t *data,
                           uint16_t data_length,
                           uint8_t signal_strength,
                           int32_t timeout_ms,
                           void *context);
int32_t ClientLinuxTcpReceive(MqttSnClientNetworkInterface *n,
                              device_address *from,
                              device_address *to,
                              uint8_t *data,
                              uint16_t max_data_length,
                              uint8_t *signal_strength,
                              int32_t timeout_ms,
                              void *context);

int MqttSnClientHandleMasterSocket(MqttSnClientNetworkInterface *n, MqttSnClientTcpNetwork *tcpNetwork);

int32_t MqttSnClientHandleClientSockets(MqttSnClientNetworkInterface *n,
                                        MqttSnClientTcpNetwork *tcpNetwork,
                                        fd_set *read_fds,
                                        device_address *from,
                                        device_address *to,
                                        uint8_t *data,
                                        uint16_t max_data_length);

void close_client_connection(MqttSnClientNetworkInterface *n, MqttSnClientTcpNetwork *tcpNetwork, int i);
/*
int save_received_messages_from_tcp_socket_into_receive_buffer(MqttSnClientTcpNetwork *tcpNetwork,
                                                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                               int client_socket_position);
*/
#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H
