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
#include <network/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include "MqttSnFixedSizeRingBuffer.h"

int GatewayLinuxUdpInit(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  memset(udpNetwork, 0, sizeof(MqttSnGatewayUdpNetwork));
  udpNetwork->my_socket = -1;
  strcpy(udpNetwork->protocol, "udp");
  n->gateway_network_receive = GatewayLinuxUdpReceive;
  n->gateway_network_send = GatewayLinuxUdpSend;
  n->gateway_network_connect = GatewayLinuxUdpConnect;
  n->gateway_network_disconnect = GatewayLinuxUdpDisconnect;
  return 0;
}

int GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

  if (networkInterface->mqtt_sn_gateway_address == NULL) {
    // FEATURE implement searching for gateway
    return -1;
  }
  uint16_t port = get_port_from_device_address(networkInterface->gateway_network_address);

  udpNetwork->my_socket = initialize_udp_socket(port);

  if (udpNetwork->my_socket == -1) {
    return -1;
  }
#ifdef WITH_LOGGING
  if (networkInterface->logger) {
    log_open_socket(networkInterface->logger,
                    udpNetwork->protocol,
                    networkInterface->gateway_network_address);
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
                       udpNetwork->protocol,
                       n->mqtt_sn_gateway_address);
    }
#endif
  }
}

int GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                           int32_t timeout_ms,
                           void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

  int message_received = is_udp_message_received(udpNetwork->my_socket, timeout_ms);
  if (message_received == -1) {
    return -1;
  }

  if (message_received == 0) {
    return 0;
  }

  int rc = receive_udp_message(udpNetwork->my_socket,
                               receiveBuffer,
                               CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH);
  if (rc < 0) {
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  if (rc > 0) {
    if (n->logger) {
      const MqttSnMessageData *msg = back(receiveBuffer);
      log_db_rec_gateway_message(n->logger,
                                 n->gateway_network_address,
                                 &msg->address,
                                 msg->data,
                                 msg->data_length);
    }
  }
#endif
  return 0;
}

int GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n,
                        MqttSnFixedSizeRingBuffer *sendBuffer,
                        int32_t timeout_ms,
                        void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  MqttSnMessageData gatewaySendMessageData = {0};

  if (pop(sendBuffer, &gatewaySendMessageData) != 0) {
    return 0;
  }
#ifdef WITH_DEBUG_LOGGING
  log_db_send_gateway_message(n->logger,
                              &gatewaySendMessageData.address,
                              n->mqtt_sn_gateway_address,
                              gatewaySendMessageData.data,
                              gatewaySendMessageData.data_length);
#endif
  ssize_t send_bytes = send_udp_message(udpNetwork->my_socket,
                                        n->mqtt_sn_gateway_address,
                                        gatewaySendMessageData.data,
                                        gatewaySendMessageData.data_length);

  if (send_bytes == -1) {
    return -1;
  }
  if (send_bytes != gatewaySendMessageData.data_length) {
    put(sendBuffer, &gatewaySendMessageData);
#ifdef WITH_DEBUG_LOGGING
    log_incomplete_message(n->logger,
                           &gatewaySendMessageData.address,
                           gatewaySendMessageData.data,
                           gatewaySendMessageData.data_length);
#endif
    return -1;
  }

  return 0;
}
