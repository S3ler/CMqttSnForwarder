//
// Created by bele on 26.01.19.
//

#include <sys/socket.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <memory.h>
#include "MqttSnGatewayUdpNetwork.h"
#include <network/udphelper/MqttSnUdpNetworkMessageParser.h>
#include "../../../MqttSnFixedSizeRingBuffer.h"

int GatewayLinuxUdpInit(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  udpNetwork->my_socket = 0;
  n->gateway_receive = GatewayLinuxUdpReceive;
  n->gateway_send = GatewayLinuxUdpSend;
  n->gateway_network_connect = GatewayLinuxUdpConnect;
  n->gateway_network_disconnect = GatewayLinuxUdpDisconnect;
  return 0;
}

int GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

  if (networkInterface->mqtt_sn_gateway_network_address == NULL) {
    // TODO implement searching for gateway
  }

  int master_socket_fd;

  uint16_t port = (((uint16_t) networkInterface->mqtt_sn_gateway_network_address->bytes[4]) << 8)
      + ((uint16_t) networkInterface->mqtt_sn_gateway_network_address->bytes[5]);

  if ((master_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    close(master_socket_fd);
    return -1;
  }

  int option = true;
  if (setsockopt(master_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    close(master_socket_fd);
    return -1;
  }

  // type of socket created
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(port);

  if (bind(master_socket_fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    close(master_socket_fd);
    return -1;
  }

  udpNetwork->my_socket = master_socket_fd;
  return 0;
}

void GatewayLinuxUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  if (udpNetwork->my_socket != 0) {
    close(udpNetwork->my_socket);
    udpNetwork->my_socket = 0;
  }
}

int GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                           uint32_t timeout_ms,
                           void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  fd_set readfds;
  FD_ZERO(&readfds);

  FD_SET(udpNetwork->my_socket, &readfds);
  int max_sd = udpNetwork->my_socket;

  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);

  if ((activity < 0) && (errno != EINTR)) {
    return -1;
  }
  if (activity == 0) {
    return 0;
  }
  if (FD_ISSET(udpNetwork->my_socket, &readfds) == 0) {
    return 0;
  }

  if (save_receive_gateway_message_from_udp_socket_into_receive_buffer(n, udpNetwork, receiveBuffer) != 0) {
    GatewayLinuxUdpDisconnect(n, context);
    return -1;
  }

  return 0;
}

int GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                        void *context) {
  // TODO: implement later: sendNetwork more messages until timeout runs out
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  MqttSnMessageData gatewaySendMessageData = {0};

  if (pop(sendBuffer, &gatewaySendMessageData) == 0) {
    struct timeval tv;
    tv.tv_sec = 0;  /* 30 Secs Timeout */
    tv.tv_usec = timeout_ms * 1000;  // Not Init'ing this can cause strange errors

    uint8_t *buffer = gatewaySendMessageData.data;
    uint16_t len = gatewaySendMessageData.data_length;
    struct sockaddr_in to_sockaddr;

    sa_family_t family = AF_INET;
    uint16_t port = (((uint16_t) gatewaySendMessageData.address.bytes[4]) << 8)
        + ((uint16_t) gatewaySendMessageData.address.bytes[5]);
    uint32_t ip = (((uint32_t) gatewaySendMessageData.address.bytes[0]) << 24)
        + (((uint32_t) gatewaySendMessageData.address.bytes[1]) << 16)
        + (((uint32_t) gatewaySendMessageData.address.bytes[2]) << 8)
        + (((uint32_t) gatewaySendMessageData.address.bytes[3]) << 0);

    to_sockaddr.sin_family = family;
    to_sockaddr.sin_port = htons(port);
    to_sockaddr.sin_addr.s_addr = htonl(ip);

    socklen_t to_sockaddr_socklen = sizeof(to_sockaddr);
    int flags = 0;

    ssize_t send_bytes =
        sendto(udpNetwork->my_socket, buffer, len, flags, (const struct sockaddr *) &to_sockaddr, to_sockaddr_socklen);
    if (send_bytes == -1) {
      return -1;
    }
    if (send_bytes != len) {
      // TODO check if a udp buffer can return different values and why
      put(sendBuffer, &gatewaySendMessageData);
      return -1;
    }
  }

  return 0;
}

int save_receive_gateway_message_from_udp_socket_into_receive_buffer(
    MqttSnGatewayNetworkInterface *networkInterface,
    MqttSnGatewayUdpNetwork *clientUdpNetwork,
    MqttSnFixedSizeRingBuffer *receiveBuffer) {

  int sd = clientUdpNetwork->my_socket;
  uint8_t buffer[CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH];
  int buffer_length = CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH;
  int read_bytes;

  struct sockaddr_in forwarder_sockaddr;
  memset(&forwarder_sockaddr, 0, sizeof(forwarder_sockaddr));
  socklen_t forwarder_sockaddr_socklen = sizeof(forwarder_sockaddr);

  // TODO what happens if too much data are send aka packetsize > MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE
  // TODO read out first 3 bytes, peek datasize, if too long remove next datasize bytes out of buffer
  // TODO write a testcase for this behaviour

  if ((read_bytes = recvfrom(sd, buffer, buffer_length, MSG_WAITALL,
                             (struct sockaddr *) &forwarder_sockaddr, &forwarder_sockaddr_socklen)) <= 0) {
    GatewayLinuxUdpDisconnect(networkInterface, (void *) clientUdpNetwork);
    return -1;
  }

  device_address gateway_address = {0};

  /*
  unsigned char *ip = (unsigned char *) &forwarder_sockaddr.sin_addr.s_addr;
  gateway_address.bytes[0] = ip[0];
  gateway_address.bytes[1] = ip[1];
  gateway_address.bytes[2] = ip[2];
  gateway_address.bytes[3] = ip[3];
  */
  uint32_t ip = ntohl(forwarder_sockaddr.sin_addr.s_addr);
  gateway_address.bytes[0] = (uint8_t) (ip >> 24);
  gateway_address.bytes[1] = (uint8_t) (ip >> 16);
  gateway_address.bytes[2] = (uint8_t) (ip >> 8);
  gateway_address.bytes[3] = (uint8_t) (ip >> 0);

  uint16_t port_as_number = (uint16_t) ntohs(forwarder_sockaddr.sin_port);
  gateway_address.bytes[4] = (uint8_t) (port_as_number >> 8);
  gateway_address.bytes[5] = (uint8_t) (port_as_number >> 0);

  return save_udp_messages_into_receive_buffer(buffer,
                                               read_bytes,
                                               gateway_address,
                                               receiveBuffer);

}
