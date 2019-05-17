//
// Created by bele on 27.01.19.
//

#include "MqttSnClientTcpNetwork.h"
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <network/shared/client/logging/MqttSnDebugMessageLogging.h>
#include <network/shared/ip/IpHelperLogging.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h>

int ClientLinuxTcpInitialize(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  if (n->client_network_broadcast_address) {
    if (ClientLinuxUdpInitialize(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  tcpNetwork->listen_socket_fd = -1;
  strcpy(tcpNetwork->protocol, CMQTTSNFORWARDER_MQTTSNCLIENTLINUXTCPNETWORKPROTOCOL);
  tcpNetwork->max_clients = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS;
  for (int i = 0; i < tcpNetwork->max_clients; ++i) {
    tcpNetwork->client_socket_fds[i] = -1;
    memset(tcpNetwork->client_buffer[i], 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
    tcpNetwork->client_buffer_bytes[i] = 0;
    tcpNetwork->client_to_drop_bytes[i] = 0;
  }

  n->initialize = ClientLinuxTcpInitialize;
  n->deinitialize = ClientLinuxTcpDeinitialize;
  n->connect = ClientLinuxTcpConnect;
  n->disconnect = ClientLinuxTcpDisconnect;
  n->receive = ClientLinuxTcpReceive;
  n->send = ClientLinuxTcpSend;
  return 0;
}

int32_t ClientLinuxTcpDeinitialize(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  if (n->client_network_broadcast_address) {
    if (ClientLinuxUdpDeinitialize(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  return 0;
}

int32_t ClientLinuxTcpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  if (n->client_network_broadcast_address) {
    if (ClientLinuxUdpConnect(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif

  uint32_t client_network_ip = 0;
  uint16_t client_network_port = 0;
  if (get_ipv4_and_port_from_device_address(&client_network_ip, &client_network_port, n->client_network_address)) {
#ifdef WITH_LOGGING
    log_failed_convert_device_address_to_ipv4_and_port(n->logger, n->client_network_address);
#endif
    return -1;
  }
  tcpNetwork->listen_socket_fd = listen_on_tcp_socket(client_network_ip, client_network_port,
                                                      CMQTTSNFORWARDER_MAXIMUM_PENDING_CONNECTIONS);
  if (tcpNetwork->listen_socket_fd < 0) {
#ifdef WITH_LOGGING
    log_failed_opening_unicast_socket(n->logger, tcpNetwork->protocol, n->client_network_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, tcpNetwork->protocol, n->client_network_address);
#endif

  return 0;
}

int32_t ClientLinuxTcpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;
  if (tcpNetwork->listen_socket_fd > -1) {
    close(tcpNetwork->listen_socket_fd);
    tcpNetwork->listen_socket_fd = -1;
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, tcpNetwork->protocol, n->client_network_address);
#endif
  }
  for (int i = 0; i < tcpNetwork->max_clients; ++i) {
    close_client_connection(n, tcpNetwork, i);
#ifdef WITH_DEBUG_LOGGING
    if (tcpNetwork->client_socket_fds[i] >= 0) {
      device_address address = get_device_address_from_tcp_file_descriptor(tcpNetwork->client_socket_fds[i]);
      log_closed_connection(n->logger, tcpNetwork->protocol, &address);
    }
#endif
  }

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  if (n->client_network_broadcast_address) {
    if (ClientLinuxUdpDisconnect(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  return 0;
}

int32_t ClientLinuxTcpSend(MqttSnClientNetworkInterface *n,
                           const device_address *from,
                           const device_address *to,
                           const uint8_t *data,
                           uint16_t data_length,
                           uint8_t signal_strength,
                           int32_t timeout_ms,
                           void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  if (n->client_network_broadcast_address) {
    if (memcmp(to, n->client_network_broadcast_address, sizeof(device_address)) == 0) {
      return ClientLinuxUdpSend(n,
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

  for (int client_socket_fd_position = 0;
       client_socket_fd_position < tcpNetwork->max_clients;
       ++client_socket_fd_position) {
    int client_socket_fd = tcpNetwork->client_socket_fds[client_socket_fd_position];
    if (client_socket_fd < 0) {
      continue;
    }

    device_address peer_address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
    if (memcmp(&peer_address, to, sizeof(device_address)) != 0) {
      continue;
    }

    ssize_t client_send_rc = send(client_socket_fd, data, data_length, 0);
    if (client_send_rc < 0) {
      close_client_connection(n, tcpNetwork, client_socket_fd_position);
#ifdef WITH_DEBUG_LOGGING
      device_address address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
      log_lost_connection(n->logger, tcpNetwork->protocol, &address);
#endif
      return 0;
    }
#ifdef WITH_DEBUG_LOGGING
    if (client_send_rc > 0 && client_send_rc == data_length) {
      log_db_send_client_message(n->logger, from, to, data, data_length);
    }
#endif
    return client_send_rc;
  }

  // no broadcast message and not client message => ignore message
#ifdef WITH_DEBUG_LOGGING
  log_client_unknown_destination(n->logger, from, to, data, data_length);
#endif
  return data_length;
}

int32_t ClientLinuxTcpReceive(MqttSnClientNetworkInterface *n,
                              device_address *from,
                              device_address *to,
                              uint8_t *data,
                              uint16_t max_data_length,
                              uint8_t *signal_strength,
                              int32_t timeout_ms,
                              void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;

  fd_set read_fds;
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  int select_rc = new_connection_or_is_message_received(tcpNetwork->listen_socket_fd,
                                                        tcpNetwork->client_socket_fds,
                                                        tcpNetwork->max_clients,
                                                        timeout_ms,
                                                        &read_fds,
                                                        tcpNetwork->udp_multicast_network.multicast_socket);
#else
  int select_rc = new_connection_or_is_message_received(tcpNetwork->listen_socket_fd,
                                                                                 tcpNetwork->client_socket_fds,
                                                                                 tcpNetwork->max_clients,
                                                                                 timeout_ms,
                                                                                 &read_fds,
                                                                                 -1);
#endif
  if (select_rc < 0) {
#ifdef WITH_LOGGING
    log_select_error(n->logger);
#endif
    return -1;
  }

  if (select_rc == 0) {
    return 0;
  }

  if (select_rc == 1 || select_rc == 4 || select_rc == 5 || select_rc == 7) {
    // if something happened on the master socket, then it is an incoming connection
    if (MqttSnClientHandleMasterSocket(n, tcpNetwork) != 0) {
      return -1;
    }
  }
#ifdef WITH_UDP_BROADCAST
  if (select_rc == 6 || select_rc == 7) {
    if (n->client_network_broadcast_address) {
      if (((tcpNetwork->received_messages + tcpNetwork->udp_multicast_network.received_messages)
          % (tcpNetwork->max_clients + 1)) == 0) {
        return ClientLinuxUdpReceive(n,
                                     from,
                                     to,
                                     data,
                                     max_data_length,
                                     signal_strength,
                                     timeout_ms,
                                     &tcpNetwork->udp_multicast_network);
      }
      return MqttSnClientHandleClientSockets(n, tcpNetwork, &read_fds, from, to, data, max_data_length);
    }
  }
#endif

  if (select_rc == 2 || select_rc == 4) {
    return MqttSnClientHandleClientSockets(n, tcpNetwork, &read_fds, from, to, data, max_data_length);
  }
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST
  if (n->client_network_broadcast_address) {
    if (select_rc == 3 || select_rc == 5) {
      // TODO filter out any NON broadcast message / also in gateway
      return ClientLinuxUdpReceive(n,
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

void close_client_connection(MqttSnClientNetworkInterface *n, MqttSnClientTcpNetwork *tcpNetwork, int i) {
  if (tcpNetwork->client_socket_fds[i] > -1) {
    close(tcpNetwork->client_socket_fds[i]);
    tcpNetwork->client_socket_fds[i] = -1;
    memset(tcpNetwork->client_buffer[i], 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
    tcpNetwork->client_buffer_bytes[i] = 0;
  }
}

/*
int save_received_messages_from_tcp_socket_into_receive_buffer(MqttSnClientTcpNetwork *tcpNetwork,
                                                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                               int client_socket_position) {
  int client_fd = tcpNetwork->client_socket_fds[client_socket_position];
  uint8_t buffer[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH];
  ssize_t read_bytes = 0;
  if ((read_bytes = read(client_fd, buffer, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH)) <= 0) {
    return -2;
  }

  device_address client_address = get_device_address_from_tcp_file_descriptor(client_fd);
  return save_tcp_messages_into_receive_buffer(buffer,
                                               read_bytes,
                                               client_address,
                                               tcpNetwork->client_buffer[client_socket_position],
                                               &tcpNetwork->client_buffer_bytes[client_socket_position],
                                               receiveBuffer);
}
*/
int32_t MqttSnClientHandleClientSockets(MqttSnClientNetworkInterface *n,
                                        MqttSnClientTcpNetwork *tcpNetwork,
                                        fd_set *read_fds,
                                        device_address *from,
                                        device_address *to,
                                        uint8_t *data,
                                        uint16_t max_data_length) {

  for (int i = 0; i < tcpNetwork->max_clients; i++) {
    // under the assumption that there is uniform distribution for all client received message we implement some form
    // of round robin depend on the received message.
    int pos = (i + tcpNetwork->received_messages) % tcpNetwork->max_clients;

    int client_socket_fd = tcpNetwork->client_socket_fds[pos];
    if (client_socket_fd < 0) {
      continue;
    }
    if (FD_ISSET(client_socket_fd, read_fds) == 0) {
      continue;
    }

    int unicast_rc = save_received_tcp_packet_into_receive_buffer(tcpNetwork->client_socket_fds[pos],
                                                                  from,
                                                                  tcpNetwork->client_buffer[pos],
                                                                  &tcpNetwork->client_buffer_bytes[pos],
                                                                  sizeof(tcpNetwork->client_buffer[pos]),
                                                                  data,
                                                                  max_data_length,
                                                                  &tcpNetwork->client_to_drop_bytes[pos]);

    if (unicast_rc == 0) {
#ifdef WITH_LOGGING
      device_address peer_address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
      log_client_disconnected(n->logger, tcpNetwork->protocol, &peer_address);
#endif
      close_client_connection(n, tcpNetwork, pos);
      continue;
    }
    if (unicast_rc < 0) {
#ifdef WITH_LOGGING
      device_address peer_address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
      log_lost_connection(n->logger, tcpNetwork->protocol, &peer_address);
#endif
      close_client_connection(n, tcpNetwork, pos);
      continue;
    }
    tcpNetwork->received_messages += 1;
    *to = *n->client_network_address;
#ifdef WITH_DEBUG_LOGGING
    if (unicast_rc > 0) {
      log_db_rec_client_message(n->logger, from, to, data, unicast_rc);
    }
#endif
    return unicast_rc;
  }
  return 0;
}

int MqttSnClientHandleMasterSocket(MqttSnClientNetworkInterface *n, MqttSnClientTcpNetwork *tcpNetwork) {
  int empty_socket_fd_position = -1;

  // find empty socket position
  for (int i = 0; i < tcpNetwork->max_clients; ++i) {
    if (tcpNetwork->client_socket_fds[i] == -1) {
      empty_socket_fd_position = i;
      break;
    }
  }

  if (empty_socket_fd_position < 0) {
    // no empty socket position
    return 0;
  }

  int new_socket;
  struct sockaddr_in address;
  socklen_t addr_len = sizeof(address);
  if ((new_socket = accept(tcpNetwork->listen_socket_fd, (struct sockaddr *) &address, &addr_len)) < 0) {
    return -1;
  }
  tcpNetwork->client_socket_fds[empty_socket_fd_position] = new_socket;
#ifdef WITH_LOGGING
  device_address new_client_address = get_device_address_from_sockaddr_in(&address);
  log_new_connection(n->logger, tcpNetwork->protocol, &new_client_address);
#endif
  return 0;
}
