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
#include <network/iphelper/MqttSnIpNetworkHelper.h>
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
  uint16_t port = get_port_from_device_address(networkInterface->forwarder_network_address);

  udpNetwork->my_socket = initialize_udp_socket(port);

  if (udpNetwork->my_socket == -1) {
    return -1;
  }
#ifdef WITH_LOGGING
  if (networkInterface->logger) {
    log_open_socket(networkInterface->logger,
                    networkInterface->logger->log_level,
                    "udp",
                    networkInterface->forwarder_network_address);
  }
#endif
  return 0;
}

void GatewayLinuxUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  if (udpNetwork->my_socket != 0) {
    close(udpNetwork->my_socket);
    udpNetwork->my_socket = 0;
#ifdef WITH_LOGGING
    if (n->logger) {
      log_close_socket(n->logger,
                       n->logger->log_level,
                       "udp",
                       n->forwarder_network_address);
    }
#endif
  }
}

int GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                           int timeout_ms,
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

int GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, int timeout_ms,
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
