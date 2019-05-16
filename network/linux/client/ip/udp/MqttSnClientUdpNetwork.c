

#include "MqttSnClientUdpNetwork.h"
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <string.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h>
#include <network/shared/client/logging/MqttSnDebugMessageLogging.h>
#include <network/shared/ip/IpHelperLogging.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h>

int32_t ClientLinuxUdpInitialize(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpNetwork = (MqttSnClientUdpNetwork *) context;
  memset(udpNetwork, 0, sizeof(MqttSnClientUdpNetwork));
  udpNetwork->received_messages = 0;
  udpNetwork->unicast_socket = -1;
#ifdef WITH_UDP_BROADCAST
  udpNetwork->multicast_socket = -1;
#endif
  strcpy(udpNetwork->protocol, CMQTTSNFORWARDER_MQTTSNCLIENTLINUXUDPNETWORKPROTOCOL);
  n->initialize = ClientLinuxUdpInitialize;
  n->deinitialize = ClientLinuxUdpDeinitialize;
  n->connect = ClientLinuxUdpConnect;
  n->disconnect = ClientLinuxUdpDisconnect;
  n->send = ClientLinuxUdpSend;
  n->receive = ClientLinuxUdpReceive;
  return 0;
}

int32_t ClientLinuxUdpDeinitialize(MqttSnClientNetworkInterface *n, void *context) {
  return 0;
}

int32_t ClientLinuxUdpConnect(MqttSnClientNetworkInterface *n, void *context) {
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

#ifdef WITH_UDP_BROADCAST
  if (n->client_network_broadcast_address) {
    if (udpNetwork->multicast_socket < 0) {
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
  }
#endif

  return 0;
}

int32_t ClientLinuxUdpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpNetwork = (MqttSnClientUdpNetwork *) context;
  if (udpNetwork->unicast_socket > -1) {
    close(udpNetwork->unicast_socket);
    udpNetwork->unicast_socket = -1;
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, udpNetwork->protocol, n->client_network_address);
#endif
  }
#ifdef WITH_UDP_BROADCAST
  if (n->client_network_broadcast_address) {
    if (udpNetwork->multicast_socket > -1) {
      close(udpNetwork->multicast_socket);
      udpNetwork->multicast_socket = -1;
#ifdef WITH_LOGGING
      log_close_multicast_socket(n->logger, udpNetwork->protocol, n->client_network_broadcast_address);
#endif
    }
  }
#endif

  return 0;
}

int32_t ClientLinuxUdpSend(MqttSnClientNetworkInterface *n,
                           const device_address *from,
                           const device_address *to,
                           const uint8_t *data,
                           uint16_t data_length,
                           uint8_t signal_strength,
                           int32_t timeout_ms,
                           void *context) {
  MqttSnClientUdpNetwork *clientUdpNetwork = (MqttSnClientUdpNetwork *) context;

#ifdef WITH_DEBUG_LOGGING
  log_db_send_client_message(n->logger, from, to, data, data_length);
#endif

  ssize_t send_rc = send_udp_message(clientUdpNetwork->unicast_socket, to, data, data_length);

#ifdef WITH_DEBUG_LOGGING
  if (send_rc > 0 && send_rc != data_length) {
    log_incomplete_client_message(n->logger, from, data, data_length);
  }

#endif

  return send_rc;
}

int32_t ClientLinuxUdpReceive(MqttSnClientNetworkInterface *n,
                              device_address *from,
                              device_address *to,
                              uint8_t *data,
                              uint16_t max_data_length,
                              uint8_t *signal_strength,
                              int32_t timeout_ms,
                              void *context) {
  MqttSnClientUdpNetwork *udpNetwork = (MqttSnClientUdpNetwork *) context;

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
    if (udpNetwork->received_messages % 2) {
      return ClientLinuxUdpReceiveUnicast(n, from, to, data, max_data_length, udpNetwork);
    } else if (n->client_network_broadcast_address) {
      return ClientLinuxUdpReceiveMulticast(n,
                                            from,
                                            to,
                                            data,
                                            max_data_length,
                                            signal_strength,
                                            udpNetwork);
    } else {
      return ClientLinuxUdpReceiveUnicast(n, from, to, data, max_data_length, udpNetwork);
    }
  }
#endif

  if (message_received == 1) {
    return ClientLinuxUdpReceiveUnicast(n, from, to, data, max_data_length, udpNetwork);
  }

#ifdef WITH_UDP_BROADCAST
  if (n->client_network_broadcast_address) {
    if (message_received == 2) {
      return ClientLinuxUdpReceiveMulticast(n,
                                            from,
                                            to,
                                            data,
                                            max_data_length,
                                            signal_strength,
                                            udpNetwork);
    }
  }
#endif

  return 0;
}

int32_t ClientLinuxUdpReceiveUnicast(MqttSnClientNetworkInterface *n,
                                     device_address *from,
                                     device_address *to,
                                     uint8_t *data,
                                     uint16_t max_data_length,
                                     MqttSnClientUdpNetwork *udpNetwork) {
  int unicast_rc = receive_udp_message(udpNetwork->unicast_socket, data, max_data_length, from);
  if (unicast_rc < 0) {
#ifdef WITH_LOGGING
    log_unicast_socket_failed(n->logger, udpNetwork->protocol, n->client_network_address);
#endif
    return -1;
  }
  *to = *n->client_network_address;
  udpNetwork->received_messages += 1;
#ifdef WITH_DEBUG_LOGGING
  if (unicast_rc > 0) {
    log_db_rec_client_message(n->logger, from, to, data, unicast_rc);
  }
#endif
  return unicast_rc;
}
#ifdef WITH_UDP_BROADCAST

int32_t ClientLinuxUdpReceiveMulticast(MqttSnClientNetworkInterface *n,
                                       device_address *from,
                                       device_address *to,
                                       uint8_t *data,
                                       uint16_t max_data_length,
                                       uint8_t *signal_strength,
                                       MqttSnClientUdpNetwork *udpNetwork) {
  if (n->client_network_broadcast_address) {
    int multicast_rc = receive_udp_message(udpNetwork->multicast_socket, data, max_data_length, from);
    if (multicast_rc < 0) {
#ifdef WITH_LOGGING
      log_multicast_socket_failed(n->logger, udpNetwork->protocol, n->client_network_broadcast_address);
#endif
      return -1;
    }
    *signal_strength = 0x01;
    *to = *n->client_network_broadcast_address;
    udpNetwork->received_messages += 1;
#ifdef WITH_DEBUG_LOGGING
    if (multicast_rc > 0) {
      log_db_rec_client_message(n->logger, from, to, data, multicast_rc);
    }
#endif
    return multicast_rc;
  }
  return 0;
}
#endif
