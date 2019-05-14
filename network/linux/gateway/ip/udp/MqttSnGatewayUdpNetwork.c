//
// Created by bele on 26.01.19.
//

#include "MqttSnGatewayUdpNetwork.h"
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h>
#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.h>
#include <network/shared/ip/IpHelperLogging.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h>

int32_t GatewayLinuxUdpInitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  memset(udpNetwork, 0, sizeof(MqttSnGatewayUdpNetwork));
  udpNetwork->received_messages = 0;
  udpNetwork->unicast_socket = -1;
#ifdef WITH_UDP_BROADCAST
  udpNetwork->multicast_socket = -1;
#endif
  strcpy(udpNetwork->protocol, CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXUDPNETWORKPROTOCOL);
  n->initialize = GatewayLinuxUdpInitialize;
  n->deinitialize = GatewayLinuxUdpDeinitialize;
  n->receive = GatewayLinuxUdpReceive;
  n->send = GatewayLinuxUdpSend;
  n->connect = GatewayLinuxUdpConnect;
  n->disconnect = GatewayLinuxUdpDisconnect;
  return 0;
}

int32_t GatewayLinuxUdpDeinitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  return 0;
}

int32_t GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

  if (n->mqtt_sn_gateway_address == NULL) {
    // FEATURE implement searching for gateway
    return -1;
  }
  uint16_t unicast_port = get_port_from_device_address(n->gateway_network_address);
  udpNetwork->unicast_socket = initialize_udp_socket(unicast_port);
  if (udpNetwork->unicast_socket < 0) {
#ifdef WITH_LOGGING
    log_failed_opening_unicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_address);
#endif

#ifdef WITH_UDP_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (udpNetwork->multicast_socket < 0) {
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
  }
#endif

  return 0;
}

int32_t GatewayLinuxUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;
  if (udpNetwork->unicast_socket > -1) {
    close(udpNetwork->unicast_socket);
    udpNetwork->unicast_socket = -1;
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, udpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif
  }
#ifdef WITH_UDP_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (udpNetwork->multicast_socket > -1) {
      close(udpNetwork->multicast_socket);
      udpNetwork->multicast_socket = -1;
#ifdef WITH_LOGGING
      log_close_multicast_socket(n->logger, udpNetwork->protocol, n->gateway_network_broadcast_address);
#endif
    }
  }
#endif

  return 0;
}

int32_t GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n,
                            const device_address *from,
                            const device_address *to,
                            const uint8_t *data,
                            uint16_t data_length,
                            uint16_t *send_data_length,
                            uint8_t signal_strength,
                            int32_t timeout_ms,
                            void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

#ifdef WITH_DEBUG_LOGGING
  log_db_send_gateway_message(n->logger, from, to, data, data_length);
#endif

  ssize_t send_bytes = send_udp_message(udpNetwork->unicast_socket, to, data, data_length);

  if (send_bytes == -1) {
    return -1;
  }

  *send_data_length = send_bytes;
#ifdef WITH_DEBUG_LOGGING
  if (send_bytes != data_length) {
    log_incomplete_gateway_message(n->logger, from, data, data_length);
  }
#endif

  return 0;
}

int32_t GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n,
                               device_address *from,
                               device_address *to,
                               uint8_t *data,
                               uint16_t *data_length,
                               uint16_t max_data_length,
                               uint8_t *signal_strength,
                               int32_t timeout_ms,
                               void *context) {
  MqttSnGatewayUdpNetwork *udpNetwork = (MqttSnGatewayUdpNetwork *) context;

#ifdef WITH_UDP_BROADCAST
  int message_received = is_multicast_or_unicast_message_receive(udpNetwork->unicast_socket,
                                                                 udpNetwork->multicast_socket,
                                                                 timeout_ms);
#else
  int message_received = is_udp_message_received(udpNetwork->unicast_socket, timeout_ms);
#endif

  if (message_received < 0) {
#ifdef WITH_LOGGING
    log_select_error(n->logger);
#endif
    return -1;
  }

#ifdef WITH_UDP_BROADCAST
  if (message_received == 3) {
    if (udpNetwork->received_messages++ % 2) {
      return GatewayLinuxUdpReceiveUnicast(n, from, to, data, data_length, max_data_length, udpNetwork);
    } else if (n->gateway_network_broadcast_address) {
      return GatewayLinuxUdpReceiveMulticast(n,
                                             from,
                                             to,
                                             data,
                                             data_length,
                                             max_data_length,
                                             signal_strength,
                                             udpNetwork);
    } else {
      return GatewayLinuxUdpReceiveUnicast(n, from, to, data, data_length, max_data_length, udpNetwork);
    }
  }
#endif

  if (message_received == 1) {
    udpNetwork->received_messages++;
    return GatewayLinuxUdpReceiveUnicast(n, from, to, data, data_length, max_data_length, udpNetwork);
  }

#ifdef WITH_UDP_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (message_received == 2) {
      udpNetwork->received_messages++;
      return GatewayLinuxUdpReceiveMulticast(n,
                                             from,
                                             to,
                                             data,
                                             data_length,
                                             max_data_length,
                                             signal_strength,
                                             udpNetwork);
    }
  }
#endif

  return 0;
}

int32_t GatewayLinuxUdpReceiveUnicast(MqttSnGatewayNetworkInterface *n,
                                      device_address *from,
                                      device_address *to,
                                      uint8_t *data,
                                      uint16_t *data_length,
                                      uint16_t max_data_length,
                                      MqttSnGatewayUdpNetwork *udpNetwork) {
  ssize_t rec_data_length = 0;
  int unicast_rc = receive_udp_message(udpNetwork->unicast_socket, data, &rec_data_length, max_data_length, from);
  if (unicast_rc < 0) {
#ifdef WITH_LOGGING
    log_unicast_socket_failed(n->logger, udpNetwork->protocol, n->gateway_network_address);
#endif
    return -1;
  }
  *data_length = rec_data_length;
  *to = *n->gateway_network_address;
#ifdef WITH_DEBUG_LOGGING
  if (unicast_rc > 0) {
    log_db_rec_gateway_message(n->logger, from, to, data, *data_length);
  }
#endif
  return 0;
}

#ifdef WITH_UDP_BROADCAST
int32_t GatewayLinuxUdpReceiveMulticast(MqttSnGatewayNetworkInterface *n,
                                        device_address *from,
                                        device_address *to,
                                        uint8_t *data,
                                        uint16_t *data_length,
                                        uint16_t max_data_length,
                                        uint8_t *signal_strength,
                                        MqttSnGatewayUdpNetwork *udpNetwork) {
  if (n->gateway_network_broadcast_address) {
    ssize_t rec_data_length = 0;
    int multicast_rc = receive_udp_message(udpNetwork->multicast_socket, data, &rec_data_length, max_data_length, from);
    if (multicast_rc < 0) {
#ifdef WITH_LOGGING
      log_multicast_socket_failed(n->logger, udpNetwork->protocol, n->gateway_network_broadcast_address);
#endif
      return -1;
    }
    *data_length = rec_data_length;
    *signal_strength = 0x01;
    *to = *n->gateway_network_broadcast_address;
#ifdef WITH_DEBUG_LOGGING
    if (multicast_rc > 0) {
      log_db_rec_gateway_message(n->logger, from, to, data, *data_length);
    }
#endif
  }
  return 0;
}
#endif
