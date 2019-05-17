//
// Created by bele on 26.01.19.
//

#include "MqttSnGatewayTcpNetwork.h"
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.h>
#include <network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h>
#include <network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#include <parser/MqttSnMessageParser.h>
#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.h>
#include <network/shared/ip/IpHelperLogging.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h>
#include <stropts.h>
#include <asm/ioctls.h>

int32_t GatewayLinuxTcpInitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (GatewayLinuxUdpInitialize(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  tcpNetwork->received_messages = 0;
  tcpNetwork->mqtt_sg_gateway_fd = -1;
  strcpy(tcpNetwork->protocol, CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXTCPNETWORKPROTOCOL);
  n->initialize = GatewayLinuxTcpInitialize;
  n->deinitialize = GatewayLinuxTcpDeinitialize;
  n->receive = GatewayLinuxTcpReceive;
  n->send = GatewayLinuxTcpSend;
  n->connect = GatewayLinuxTcpConnect;
  n->disconnect = GatewayLinuxTcpDisconnect;
  return 0;
}

int32_t GatewayLinuxTcpDeinitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (GatewayLinuxUdpDeinitialize(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  return 0;
}

int32_t GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  if (n->mqtt_sn_gateway_address == NULL) {
    // FEATURE implement searching for gateway
    return -1;
  }

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (GatewayLinuxUdpConnect(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif

  if (tcpNetwork->mqtt_sg_gateway_fd > 0) {
    return 0;
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

  return 0;
}

int32_t GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
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

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (GatewayLinuxUdpDisconnect(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  return 0;
}

int32_t GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n,
                            const device_address *from,
                            const device_address *to,
                            const uint8_t *data,
                            uint16_t data_length,
                            uint8_t signal_strength,
                            int32_t timeout_ms,
                            void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (memcmp(to, n->gateway_network_broadcast_address, sizeof(device_address)) == 0) {
      return GatewayLinuxUdpSend(n,
                                 from,
                                 to,
                                 data,
                                 data_length,
                                 signal_strength,
                                 timeout_ms,
                                 &tcpNetwork->udp_multicast_network);
    }
  }
#endif

  if (memcmp(to, n->mqtt_sn_gateway_address, sizeof(device_address)) != 0) {
#ifdef WITH_LOGGING
    log_gateway_unknown_destination(n->logger, from, to, data, data_length);
#endif
    return data_length;
  }

#ifdef WITH_DEBUG_LOGGING
  log_db_send_gateway_message(n->logger, from, to, data, data_length);
#endif

  ssize_t send_rc = send(tcpNetwork->mqtt_sg_gateway_fd, data, data_length, 0);

  if (send_rc == 0) {
#ifdef WITH_LOGGING
    log_gateway_close_connection(n->logger, tcpNetwork->protocol, n->gateway_network_address);
#endif
    return -1;
  }

  if (send_rc < 0) {
#ifdef WITH_LOGGING
    log_gateway_lost_connection(n->logger, tcpNetwork->protocol, n->mqtt_sn_gateway_address);
#endif
    return -1;
  }

#ifdef WITH_DEBUG_LOGGING
  if (send_rc > 0 && send_rc != data_length) {
    log_incomplete_gateway_message(n->logger, from, data, data_length);
    return -1;
  }
#endif

  return send_rc;
}

int32_t GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n,
                               device_address *from,
                               device_address *to,
                               uint8_t *data,
                               uint16_t max_data_length,
                               uint8_t *signal_strength,
                               int32_t timeout_ms,
                               void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
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

#ifdef WITH_UDP_BROADCAST
  if (message_received == 3) {
    if (tcpNetwork->received_messages++ % 2) {
      return GatewayLinuxTcpReceiveUnicast(n, from, to, data, max_data_length, tcpNetwork);
    } else if (n->gateway_network_broadcast_address) {
      return GatewayLinuxUdpReceive(n,
                                    from,
                                    to,
                                    data,
                                    max_data_length,
                                    signal_strength,
                                    timeout_ms,
                                    &tcpNetwork->udp_multicast_network);
    } else {
      return GatewayLinuxTcpReceiveUnicast(n, from, to, data, max_data_length, tcpNetwork);
    }
  }
#endif

  if (message_received == 1) {
    tcpNetwork->received_messages++;
    return GatewayLinuxTcpReceiveUnicast(n, from, to, data, max_data_length, tcpNetwork);
  }

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
  if (n->gateway_network_broadcast_address) {
    if (message_received == 2) {
      tcpNetwork->received_messages++;
      return GatewayLinuxUdpReceive(n,
                                    from,
                                    to,
                                    data,
                                    max_data_length,
                                    signal_strength,
                                    timeout_ms,
                                    &tcpNetwork->udp_multicast_network);
    }
  }
#endif

  return 0;
}
int32_t GatewayLinuxTcpReceiveUnicast(MqttSnGatewayNetworkInterface *n,
                                      device_address *from,
                                      device_address *to,
                                      uint8_t *data,
                                      uint16_t max_data_length,
                                      MqttSnGatewayTcpNetwork *tcp_network) {
  int unicast_rc = save_received_tcp_packet_into_receive_buffer(tcp_network->mqtt_sg_gateway_fd,
                                                                from,
                                                                tcp_network->gateway_buffer,
                                                                &tcp_network->gateway_buffer_bytes,
                                                                sizeof(tcp_network->gateway_buffer),
                                                                data,
                                                                max_data_length,
                                                                &tcp_network->to_drop_bytes);
  if (unicast_rc == 0) {
    log_gateway_close_connection(n->logger, tcp_network->protocol, n->mqtt_sn_gateway_address);
  }
  if (unicast_rc < 0) {
#ifdef WITH_LOGGING
    log_gateway_lost_connection(n->logger, tcp_network->protocol, n->mqtt_sn_gateway_address);
#endif
    return -1;
  }
  *to = *n->gateway_network_address;
#ifdef WITH_DEBUG_LOGGING
  log_db_rec_gateway_message(n->logger, from, to, data, unicast_rc);
#endif
  return unicast_rc;
}
