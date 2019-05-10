//
// Created by bele on 26.01.19.
//

#include "MqttSnGatewayTcpNetwork.h"
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h>
#include <forwarder/network/shared/ip/IpHelper.h>
#include <forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.h>
#include <forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h>
#include <forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <forwarder/network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#include <forwarder/MqttSnMessageParser.h>

int GatewayLinuxTcpInit(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
#ifdef WITH_TCP_BROADCAST
  if (GatewayLinuxUdpInit(n, &tcpNetwork->udp_multicast_network) < 0) {
    return -1;
  }
#endif
  tcpNetwork->mqtt_sg_gateway_fd = -1;
  strcpy(tcpNetwork->protocol, CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXTCPNETWORKPROTOCOL);
  n->gateway_network_receive = GatewayLinuxTcpReceive;
  n->gateway_network_send = GatewayLinuxTcpSend;
  n->gateway_network_connect = GatewayLinuxTcpConnect;
  n->gateway_network_disconnect = GatewayLinuxTcpDisconnect;

  return 0;
}

int GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  if (n->mqtt_sn_gateway_address == NULL) {
    // FEATURE implement searching for gateway
    return -1;
  }
  uint32_t gateway_ip = 0;
  uint16_t gateway_port = 0;
  if (get_ipv4_and_port_from_device_address(&gateway_ip, &gateway_port, n->mqtt_sn_gateway_address)) {
#ifdef WITH_LOGGING
    log_failed_convert_device_address_to_ipv4_and_port(n->logger, n->mqtt_sn_gateway_address);
#endif
    return -1;
  }

  tcpNetwork->mqtt_sg_gateway_fd = connect_to_tcp_host(gateway_ip, gateway_port);
  if (tcpNetwork->mqtt_sg_gateway_fd < 0) {
#ifdef WITH_LOGGING
    log_failed_opening_unicast_socket(n->logger, tcpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, tcpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif

#ifdef WITH_TCP_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (GatewayLinuxUdpConnect(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  return 0;
}

void GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
  if (tcpNetwork->mqtt_sg_gateway_fd > -1) {
    close(tcpNetwork->mqtt_sg_gateway_fd);
    tcpNetwork->mqtt_sg_gateway_fd = -1;
    memset(tcpNetwork->gateway_buffer, 0, CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH);
    tcpNetwork->gateway_buffer_bytes = 0;
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, tcpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif
  }

#ifdef WITH_TCP_BROADCAST
  if (n->gateway_network_broadcast_address) {
    GatewayLinuxUdpDisconnect(n, &tcpNetwork->udp_multicast_network);
  }
#endif
}

int GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *receiveBuffer,
                           int32_t timeout_ms,
                           void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  // TODO is message received with gateway_fd AND multicast
#ifdef WITH_TCP_BROADCAST
  int message_received = is_multicast_or_unicast_message_receive(tcpNetwork->mqtt_sg_gateway_fd,
                                                                 tcpNetwork->udp_multicast_network.multicast_socket,
                                                                 timeout_ms);
#else
  int message_received = is_tcp_message_received(tcpNetwork->mqtt_sg_gateway_fd, timeout_ms);
#endif
  if (message_received < 0) {
#ifdef WITH_LOGGING
    log_select_error(n->logger);
#endif
    return -1;
  }

  if (message_received == 1 || message_received == 3) {
    int unicast_rc = save_received_tcp_packet_into_receive_buffer(tcpNetwork, receiveBuffer);
    if (unicast_rc < 0) {
#ifdef WITH_LOGGING
      log_gateway_lost_connection(n->logger, tcpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif
      return -1;
    }
#ifdef WITH_DEBUG_LOGGING
    if (unicast_rc > 0) {
      for (int32_t msg_counter = 0; msg_counter < unicast_rc; ++msg_counter) {
        const MqttSnMessageData *msg = getBack(receiveBuffer, ((unicast_rc - 1) - msg_counter));
        log_db_rec_gateway_message(n->logger, n->mqtt_sn_gateway_address, NULL, msg->data, msg->data_length);
      }
    }
#endif
  }

#ifdef WITH_TCP_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (GatewayLinuxUdpReceive(n, receiveBuffer, 0, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif

  return 0;
}

int GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n,
                        MqttSnFixedSizeRingBuffer *sendBuffer,
                        int32_t timeout_ms,
                        void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  if (isEmpty(sendBuffer)) {
    return 0;
  }

#ifdef WITH_TCP_BROADCAST
  // send message via udp if it is no encapsulation message
  if (n->gateway_network_broadcast_address) {
    const MqttSnMessageData *front_msg = front(sendBuffer);
    if (!front_msg) {
      return 0;
    }
    if (front_msg->broadcast_radius &&
        (memcmp(&front_msg->address, n->gateway_network_broadcast_address, sizeof(device_address)) == 0)) {
      MqttSnMessageData sendMessageData = {0};
      if (pop(sendBuffer, &sendMessageData) != 0) {
        return 0;
      }
      // is broadcast message => send via udp
      MqttSnFixedSizeRingBuffer tmp_sendQueue = {0};
      MqttSnFixedSizeRingBufferInit(&tmp_sendQueue);
      if (put(&tmp_sendQueue, &sendMessageData) < 0) {
        // error which should never happen
        return -1;
      }
      if (GatewayLinuxUdpSend(n, &tmp_sendQueue, 0, &tcpNetwork->udp_multicast_network) < 0) {
        return -1;
      }
      return 0;
    }
  }
#endif

  MqttSnMessageData gatewayMessage = {0};
  if (pop(sendBuffer, &gatewayMessage) != 0) {
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

  ssize_t rc = send(tcpNetwork->mqtt_sg_gateway_fd, gatewayMessage.data, gatewayMessage.data_length, 0);
  if (rc < 0) {
#ifdef WITH_LOGGING
    log_gateway_lost_connection(n->logger, tcpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  log_db_send_gateway_message(n->logger,
                              &gatewayMessage.address,
                              n->mqtt_sn_gateway_address,
                              gatewayMessage.data,
                              gatewayMessage.data_length);
#endif
  if (rc != gatewayMessage.data_length) {
    put(sendBuffer, &gatewayMessage);
#ifdef WITH_DEBUG_LOGGING
    log_incomplete_message(n->logger,
                           &gatewayMessage.address,
                           gatewayMessage.data,
                           gatewayMessage.data_length);
#endif
    return -1;
  }

  return 0;
}

int save_received_tcp_packet_into_receive_buffer(
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
  device_address gateway_address = get_device_address_from_tcp_file_descriptor(gateway_fd);
  return save_tcp_messages_into_receive_buffer(buffer,
                                               read_bytes,
                                               gateway_address,
                                               gatewayTcpNetwork->gateway_buffer,
                                               &gatewayTcpNetwork->gateway_buffer_bytes,
                                               receiveBuffer);
}
