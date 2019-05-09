//
// Created by bele on 26.01.19.
//

#include "MqttSnGatewayUdpNetwork.h"
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <forwarder/network/shared/ip/IpHelper.h>
#include <forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h>

int GatewayLinuxUdpInit(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  memset(udpNetwork, 0, sizeof(MqttSnGatewayUdpNetwork));
  udpNetwork->unicast_socket = -1;
  udpNetwork->multicast_socket = -1;
  strcpy(udpNetwork->protocol, CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXUDPNETWORKPROTOCOL);
  n->gateway_network_init = GatewayLinuxUdpInit;
  n->gateway_network_receive = GatewayLinuxUdpReceive;
  n->gateway_network_send = GatewayLinuxUdpSend;
  n->gateway_network_connect = GatewayLinuxUdpConnect;
  n->gateway_network_disconnect = GatewayLinuxUdpDisconnect;
  return 0;
}

int GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

  if (n->mqtt_sn_gateway_address == NULL) {
    // FEATURE implement searching for gateway
    return -1;
  }
  uint16_t unicast_port = get_port_from_device_address(n->gateway_network_address);
  udpNetwork->unicast_socket = initialize_udp_socket(unicast_port);
  if (udpNetwork->unicast_socket < -1) {
#ifdef WITH_LOGGING
    log_failed_opening_unicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_address);
#endif

  if (n->gateway_network_broadcast_address) {
    uint32_t multicast_ip = 0;
    uint16_t multicast_port = 0;
    if (get_ipv4_and_port_from_device_address(&multicast_ip, &multicast_port, n->gateway_network_broadcast_address)) {
#ifdef WITH_LOGGING
      log_failed_convert_device_address_to_ipv4_and_port(n->logger, n->gateway_network_broadcast_address);
#endif
      return -1;
    }
    udpNetwork->multicast_socket = initialize_udp_multicast_socket(udpNetwork->unicast_socket,
                                                                   multicast_ip,
                                                                   multicast_port);
    if (udpNetwork->multicast_socket < 0) {
#ifdef WITH_LOGGING
      log_failed_opening_multicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_broadcast_address);
#endif
      return -1;
    }
#ifdef WITH_LOGGING
    log_opening_multicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_broadcast_address);
#endif
  }

  return 0;
}

void GatewayLinuxUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  if (udpNetwork->unicast_socket > -1) {
    close(udpNetwork->unicast_socket);
    udpNetwork->unicast_socket = -1;
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, udpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif
  }
  if (udpNetwork->multicast_socket > -1) {
    close(udpNetwork->multicast_socket);
    udpNetwork->multicast_socket = -1;
#ifdef WITH_LOGGING
    log_close_multicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_broadcast_address);
#endif
  }
}

int GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                           int32_t timeout_ms,
                           void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

  if (n->gateway_network_broadcast_address) {
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
                                                                        CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH);
      if (unicast_rc < 0) {
#ifdef WITH_LOGGING
        log_unicast_socket_failed(n->logger, udpNetwork->protocol, n->gateway_network_address);
#endif
        return -1;
      }
#ifdef WITH_DEBUG_LOGGING
      if (unicast_rc > 0) {
        const MqttSnMessageData *msg = back(receiveBuffer);
        log_db_rec_gateway_message(n->logger, n->gateway_network_address, &msg->address, msg->data, msg->data_length);
      }
#endif
    }
    if (message_received == 2 || message_received == 3) {
      int multicast_rc = receive_and_save_udp_multicast_message_into_receive_buffer(udpNetwork->multicast_socket,
                                                                                    receiveBuffer,
                                                                                    CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH);
      if (multicast_rc < 0) {
#ifdef WITH_LOGGING
        log_multicast_socket_failed(n->logger, udpNetwork->protocol, n->gateway_network_broadcast_address);
#endif
        return -1;
      }
#ifdef WITH_DEBUG_LOGGING
      if (multicast_rc > 0) {
        const MqttSnMessageData *msg = back(receiveBuffer);
        log_db_rec_gateway_message(n->logger,
                                   n->gateway_network_broadcast_address,
                                   &msg->address,
                                   msg->data,
                                   msg->data_length);
        //TODO check output here and everywhere alse in linux
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
                                                                    CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_MAX_DATA_LENGTH);
  if (unicast_rc < 0) {
#ifdef WITH_LOGGING
    log_unicast_socket_failed(n->logger, udpNetwork->protocol, n->gateway_network_address);
#endif
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  if (unicast_rc > 0) {
    const MqttSnMessageData *msg = back(receiveBuffer);
    log_db_rec_gateway_message(n->logger, n->gateway_network_address, &msg->address, msg->data, msg->data_length);
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
  ssize_t send_bytes = send_udp_message(udpNetwork->unicast_socket,
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
