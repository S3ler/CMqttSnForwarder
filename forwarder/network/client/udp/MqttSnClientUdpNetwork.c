

#include "MqttSnClientUdpNetwork.h"
#include <network/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <network/iphelper/MqttSnIpNetworkHelper.h>

int ClientLinuxUdpInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *clientUdpNetwork = (MqttSnClientUdpNetwork *) context;
  clientUdpNetwork->master_socket = -1;

  n->client_network_init = ClientLinuxUdpInit;
  n->client_network_connect = ClientLinuxUdpConnect;
  n->client_network_disconnect = ClientLinuxUdpDisconnect;
  n->client_network_receive = ClientLinuxUdpReceive;
  n->client_network_send = ClientLinuxUdpSend;
  return 0;
}

int ClientLinuxUdpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *clientUdpNetwork = (MqttSnClientUdpNetwork *) context;

  uint16_t port = get_port_from_device_address(n->client_network_address);

  clientUdpNetwork->master_socket = initialize_udp_socket(port);

  if (clientUdpNetwork->master_socket == -1) {
    return -1;
  }
#ifdef WITH_LOGGING
  if (n->logger) {
    log_open_socket(n->logger,
                    n->logger->log_level,
                    "udp",
                    n->client_network_address);
  }
#endif
  return 0;
}

void ClientLinuxUdpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *clientUdpNetwork = (MqttSnClientUdpNetwork *) context;
  if (clientUdpNetwork->master_socket != -1) {
    close(clientUdpNetwork->master_socket);
    clientUdpNetwork->master_socket = -1;
#ifdef WITH_LOGGING
    if (n->logger) {
      log_close_socket(n->logger,
                       n->logger->log_level,
                       "udp",
                       n->client_network_address);
    }
#endif
  }
}

int ClientLinuxUdpReceive(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          uint32_t timeout_ms,
                          void *context) {
  MqttSnClientUdpNetwork *clientUdpNetwork = (MqttSnClientUdpNetwork *) context;

  int message_received = is_udp_message_received(clientUdpNetwork->master_socket, timeout_ms);
  if (message_received == -1) {
    return -1;
  }

  if (message_received == 0) {
    return 0;
  }

  int rc = receive_udp_message(clientUdpNetwork->master_socket,
                               receiveBuffer,
                               CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_MAX_DATA_LENGTH);
  if (rc < 0) {
    return -1;
  }
#ifdef WITH_LOGGING
  if (rc > 0) {
    if (n->logger) {
      const MqttSnMessageData *msg = back(receiveBuffer);
      log_client_message(n->logger,
                          n->logger->log_level,
                          &msg->address,
                          msg->data,
                          msg->data_length);
    }
  }
#endif
  return 0;
}

int ClientLinuxUdpSend(MqttSnClientNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       uint32_t timeout_ms,
                       void *context) {
  MqttSnClientUdpNetwork *clientUdpNetwork = (MqttSnClientUdpNetwork *) context;
  MqttSnMessageData gatewaySendMessageData = {0};

  if (pop(sendBuffer, &gatewaySendMessageData) != 0) {
    return 0;
  }

  ssize_t send_bytes = send_udp_message(clientUdpNetwork->master_socket,
                                        &gatewaySendMessageData.address,
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