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

  if (networkInterface->forwarder_network_address == NULL) {
    // TODO implement searching for gateway
  }

  uint16_t port = (((uint16_t) networkInterface->forwarder_network_address->bytes[4]) << 8)
      + ((uint16_t) networkInterface->forwarder_network_address->bytes[5]);

  udpNetwork->my_socket = initialize_udp_socket(port);

  if (udpNetwork->my_socket == -1) {
    return -1;
  }
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

  int message_received = is_udp_message_received(udpNetwork->my_socket, timeout_ms);
  if (message_received == -1) {
    return -1;
  }

  if (message_received == 0) {
    return 0;
  }

  if (receive_udp_message(udpNetwork->my_socket,
                          receiveBuffer,
                          CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH) != 0) {
    return -1;
  }

  return 0;
}

int GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                        void *context) {
  // TODO: implement later: sendNetwork more messages until timeout runs out
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  MqttSnMessageData gatewaySendMessageData = {0};

  if (pop(sendBuffer, &gatewaySendMessageData) != 0) {
    return 0;
  }

  ssize_t send_bytes = send_udp_message(udpNetwork->my_socket,
                                        n->gateway_network_address,
                                        gatewaySendMessageData.data,
                                        gatewaySendMessageData.data_length);

  if (send_bytes == -1) {
    return -1;
  }
  if (send_bytes != gatewaySendMessageData.data_length) {
    // TODO check if a udp buffer can return different values and why
    put(sendBuffer, &gatewaySendMessageData);
    return -1;
  }

  return 0;
}
