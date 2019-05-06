

#include "MqttSnClientUdpNetwork.h"
#include <forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <string.h>
#include <forwarder/network/linux/shared/shared/IpHelper.h>
#include <forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h>

int ClientLinuxUdpInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpNetwork = (MqttSnClientUdpNetwork *) context;
  memset(udpNetwork, 0, sizeof(MqttSnClientUdpNetwork));
  udpNetwork->unicast_socket = -1;
  udpNetwork->multicast_socket = -1;
  strcpy(udpNetwork->protocol, CMQTTSNFORWARDER_MQTTSNCLIENTLINUXUDPNETWORKPROTOCOL);
  n->client_network_init = ClientLinuxUdpInit;
  n->client_network_connect = ClientLinuxUdpConnect;
  n->client_network_disconnect = ClientLinuxUdpDisconnect;
  n->client_network_receive = ClientLinuxUdpReceive;
  n->client_network_send = ClientLinuxUdpSend;
  return 0;
}

int ClientLinuxUdpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpNetwork = (MqttSnClientUdpNetwork *) context;

  uint16_t unicast_port = get_port_from_device_address(n->client_network_address);
  udpNetwork->unicast_socket = initialize_udp_socket(unicast_port);
  if (udpNetwork->unicast_socket < 0) {
#ifdef WITH_LOGGING
    log_failed_opening_unicast_socket(n->logger, udpNetwork->protocol, n->client_network_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, udpNetwork->protocol, n->client_network_address);
#endif

  if (n->client_network_broadcast_address != NULL) {
    uint32_t multicast_ip = 0;
    uint16_t multicast_port = 0;
    if (get_ipv4_and_port_from_device_address(&multicast_ip, &multicast_port, n->client_network_broadcast_address)) {
#ifdef WITH_LOGGING
      log_failed_convert_device_address_to_ipv4_and_port(n->logger, n->client_network_broadcast_address);
#endif
      return -1;
    }
    udpNetwork->multicast_socket = initialize_udp_multicast_socket(udpNetwork->unicast_socket,
                                                                   multicast_ip,
                                                                   multicast_port);
    if (udpNetwork->multicast_socket < 0) {
#ifdef WITH_LOGGING
      log_failed_opening_multicast_socket(n->logger, udpNetwork->protocol, n->client_network_broadcast_address);
#endif
      return -1;
    }
#ifdef WITH_LOGGING
    log_opening_multicast_socket(n->logger, udpNetwork->protocol, n->client_network_broadcast_address);
#endif
  }

  return 0;
}

void ClientLinuxUdpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpNetwork = (MqttSnClientUdpNetwork *) context;
  if (udpNetwork->unicast_socket > -1) {
    close(udpNetwork->unicast_socket);
    udpNetwork->unicast_socket = -1;
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, udpNetwork->protocol, n->client_network_address);
#endif
  }
  if (udpNetwork->multicast_socket > -1) {
    close(udpNetwork->multicast_socket);
    udpNetwork->multicast_socket = -1;
#ifdef WITH_LOGGING
    log_close_multicast_socket(n->logger, udpNetwork->protocol, n->client_network_broadcast_address);
#endif
  }
}

int ClientLinuxUdpReceive(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int32_t timeout_ms,
                          void *context) {
  MqttSnClientUdpNetwork *udpNetwork = (MqttSnClientUdpNetwork *) context;

  if (n->client_network_broadcast_address) {
    int message_received = is_multicast_or_unicast_message_receive(udpNetwork->unicast_socket,
                                                                   udpNetwork->multicast_socket, timeout_ms);
    if (message_received < 0) {
#ifdef WITH_LOGGING
      log_select_error(n->logger);
#endif
      return -1;
    }
    if (message_received == 0) {
      return 0;
    }
    if (message_received == 1 || message_received == 3) {
      int unicast_rc = receive_and_save_udp_message_into_receive_buffer(udpNetwork->unicast_socket,
                                                                        receiveBuffer,
                                                                        CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_MAX_DATA_LENGTH);
      if (unicast_rc < 0) {
#ifdef WITH_LOGGING
        log_unicast_socket_failed(n->logger, udpNetwork->protocol, n->client_network_address);
#endif
        return -1;
      }
#ifdef WITH_DEBUG_LOGGING
      if (unicast_rc > 0) {
        const MqttSnMessageData *msg = back(receiveBuffer);
        log_db_rec_client_message(n->logger, &msg->address, n->mqtt_sn_gateway_address, msg->data, msg->data_length);
      }
#endif
    }
    if (message_received == 2 || message_received == 3) {
      int multicast_rc = receive_and_save_udp_multicast_message_into_receive_buffer(udpNetwork->multicast_socket,
                                                                                    receiveBuffer,
                                                                                    CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_MAX_DATA_LENGTH);
      if (multicast_rc < 0) {
#ifdef WITH_LOGGING
        log_multicast_socket_failed(n->logger, udpNetwork->protocol, n->client_network_broadcast_address);
#endif
        return -1;
      }
#ifdef WITH_DEBUG_LOGGING
      if (multicast_rc > 0) {
        const MqttSnMessageData *msg = back(receiveBuffer);
        log_db_rec_client_message(n->logger,
                                  &msg->address,
                                  n->client_network_broadcast_address,
                                  msg->data,
                                  msg->data_length);
      }
#endif
    }
    return 0;
  }

  // only unicast
  int message_received = is_udp_message_received(udpNetwork->unicast_socket, timeout_ms);
  if (message_received < 0) {
#ifdef WITH_LOGGING
    log_select_error(n->logger);
#endif
    return -1;
  }
  if (message_received == 0) {
    return 0;
  }

  int unicast_rc = receive_and_save_udp_message_into_receive_buffer(udpNetwork->unicast_socket,
                                                                    receiveBuffer,
                                                                    CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_MAX_DATA_LENGTH);
  if (unicast_rc < 0) {
#ifdef WITH_LOGGING
    log_unicast_socket_failed(n->logger, udpNetwork->protocol, n->client_network_address);
#endif
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  if (unicast_rc > 0) {
    if (n->logger) {
      const MqttSnMessageData *msg = back(receiveBuffer);
      log_db_rec_client_message(n->logger, &msg->address, n->mqtt_sn_gateway_address, msg->data, msg->data_length);
    }
  }
#endif
  return 0;
}

int ClientLinuxUdpSend(MqttSnClientNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int32_t timeout_ms,
                       void *context) {
  MqttSnClientUdpNetwork *clientUdpNetwork = (MqttSnClientUdpNetwork *) context;
  MqttSnMessageData clientSendMessageData = {0};

  if (pop(sendBuffer, &clientSendMessageData) != 0) {
    return 0;
  }

#ifdef WITH_DEBUG_LOGGING
  if (n->logger) {
    if (log_db_send_client_message(n->logger,
                                   n->mqtt_sn_gateway_address,
                                   &clientSendMessageData.address,
                                   clientSendMessageData.data,
                                   clientSendMessageData.data_length)) {
      return -1;
    }
  }
#endif

  ssize_t send_bytes = send_udp_message(clientUdpNetwork->unicast_socket,
                                        &clientSendMessageData.address,
                                        clientSendMessageData.data,
                                        clientSendMessageData.data_length);

  if (send_bytes == -1) {
    return -1;
  }
  if (send_bytes != clientSendMessageData.data_length) {
    put(sendBuffer, &clientSendMessageData);
#ifdef WITH_DEBUG_LOGGING
    log_incomplete_message(n->logger,
                           &clientSendMessageData.address,
                           clientSendMessageData.data,
                           clientSendMessageData.data_length);
#endif
    return -1;
  }

  return 0;
}
