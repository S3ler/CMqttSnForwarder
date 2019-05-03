//
// Created by bele on 26.01.19.
//

#include "MqttSnGatewayTcpNetwork.h"
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <forwarder/network/shared/ip/MqttSnIpNetworkHelper.h>
#include <forwarder/network/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h>
#include <forwarder/network/shared/shared/IpHelper.h>

int GatewayLinuxTcpInit(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
  tcpNetwork->mqtt_sg_gateway_fd = -1;
  strcpy(tcpNetwork->protocol, "tcp");
  n->gateway_network_receive = GatewayLinuxTcpReceive;
  n->gateway_network_send = GatewayLinuxTcpSend;
  n->gateway_network_connect = GatewayLinuxTcpConnect;
  n->gateway_network_disconnect = GatewayLinuxTcpDisconnect;
  return 0;
}

int GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  if (networkInterface->mqtt_sn_gateway_address == NULL) {
    // FEATURE implement searching for gateway
    return -1;
  }
  uint32_t gateway_ip = 0;
  uint16_t gateway_port = 0;
  if (get_ipv4_and_port_from_device_address(&gateway_ip, &gateway_port, networkInterface->mqtt_sn_gateway_address)) {
    return -1;
  }

  struct sockaddr_in address;
  address.sin_port = htons(gateway_port);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(gateway_ip);

  int connect_result = 0;
  tcpNetwork->mqtt_sg_gateway_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (tcpNetwork->mqtt_sg_gateway_fd < 0) {
    return -1;
  }
  connect_result = connect(tcpNetwork->mqtt_sg_gateway_fd, (struct sockaddr *) &address, sizeof(address));

  if (connect_result < 0) {
    return -1;
  }
#ifdef WITH_LOGGING
  if (networkInterface->logger) {
    log_open_socket(networkInterface->logger,
                    tcpNetwork->protocol,
                    networkInterface->mqtt_sn_gateway_address);
  }
#endif
  return 0;
}

void GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
  if (tcpNetwork->mqtt_sg_gateway_fd >= 0) {
    close(tcpNetwork->mqtt_sg_gateway_fd);
    tcpNetwork->mqtt_sg_gateway_fd = -1;
    memset(tcpNetwork->gateway_buffer, 0, CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH);
    tcpNetwork->gateway_buffer_bytes = 0;
  }
}

int GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                           int32_t timeout_ms,
                           void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  fd_set read_file_descriptor;
  FD_ZERO(&read_file_descriptor);
  // add master socket to set
  FD_SET(tcpNetwork->mqtt_sg_gateway_fd, &read_file_descriptor);
  int maximum_socket_descriptor = tcpNetwork->mqtt_sg_gateway_fd;

  int activity = 0;
  if (timeout_ms < 0) {
    activity = select(maximum_socket_descriptor + 1, &read_file_descriptor, NULL, NULL, NULL);
  } else {
    struct timeval interval = {.tv_sec = timeout_ms / 1000, .tv_usec = (timeout_ms % 1000) * 1000};
    activity = select(maximum_socket_descriptor + 1, &read_file_descriptor, NULL, NULL, &interval);
  }

  if ((activity < 0) && (errno != EINTR)) {
    log_select_error(n->logger);
    return -1;
  }
  if (activity == 0) {
    return 0;
  }

  if (FD_ISSET(tcpNetwork->mqtt_sg_gateway_fd, &read_file_descriptor) == 0) {
    return 0;
  }

  if (save_receive_gateway_message_from_tcp_socket_into_receive_buffer(tcpNetwork, receiveBuffer) != 0) {
#ifdef WITH_LOGGING
    if (n->logger) {
      log_gateway_lost_connection(n->logger,
                                  tcpNetwork->protocol,
                                  n->mqtt_sn_gateway_address);
    }
#endif
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  if (n->logger) {
    const MqttSnMessageData *msg = back(receiveBuffer);
    log_db_rec_gateway_message(n->logger,
                               n->mqtt_sn_gateway_address,
                               NULL,
                               msg->data,
                               msg->data_length);
  }
#endif

  return 0;
}

int GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n,
                        MqttSnFixedSizeRingBuffer *sendBuffer,
                        int32_t timeout_ms,
                        void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  MqttSnMessageData sendMessageData = {0};
  if (pop(sendBuffer, &sendMessageData) != 0) {
    return 0;
  }

  struct timeval interval;
  if (timeout_ms < 0) {
    interval.tv_sec = 0;
    interval.tv_usec = 0;
  } else if (timeout_ms == 0) {
    interval.tv_sec = 0;
    interval.tv_usec = 1;
  } else {
    interval.tv_sec = timeout_ms / 1000;
    interval.tv_usec = (timeout_ms % 1000) * 1000;
  }

  if (setsockopt(tcpNetwork->mqtt_sg_gateway_fd,
                 SOL_SOCKET,
                 SO_SNDTIMEO,
                 (char *) &interval,
                 sizeof(struct timeval))) {
    return -1;
  }

  ssize_t rc = send(tcpNetwork->mqtt_sg_gateway_fd, sendMessageData.data, sendMessageData.data_length, 0);
  if (rc < 0) {
#ifdef WITH_LOGGING
    if (n->logger) {
      log_gateway_lost_connection(n->logger,
                                  tcpNetwork->protocol,
                                  n->mqtt_sn_gateway_address);
    }
#endif
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  log_db_send_gateway_message(n->logger,
                              &sendMessageData.address,
                              n->mqtt_sn_gateway_address,
                              sendMessageData.data,
                              sendMessageData.data_length);
#endif
  if (rc != sendMessageData.data_length) {
    put(sendBuffer, &sendMessageData);
#ifdef WITH_DEBUG_LOGGING
    log_incomplete_message(n->logger,
                           &sendMessageData.address,
                           sendMessageData.data,
                           sendMessageData.data_length);
#endif
    return -1;
  }

  return 0;
}

int save_receive_gateway_message_from_tcp_socket_into_receive_buffer(
    MqttSnGatewayTcpNetwork *gatewayTcpNetwork,
    MqttSnFixedSizeRingBuffer *receiveBuffer) {
  int gateway_fd = gatewayTcpNetwork->mqtt_sg_gateway_fd;
  uint8_t buffer[CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH];
  ssize_t read_bytes = 0;
  if ((read_bytes = read(gateway_fd,
                         buffer,
                         CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH)) < 0) {
    return -1;
  }

  if (read_bytes == 0) {
    return -1;
  }
  device_address gateway_address = get_device_address_from_file_descriptor(gateway_fd);
  return save_tcp_messages_into_receive_buffer(buffer,
                                               read_bytes,
                                               gateway_address,
                                               gatewayTcpNetwork->gateway_buffer,
                                               &gatewayTcpNetwork->gateway_buffer_bytes,
                                               receiveBuffer);
}
