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

int ClientLinuxTcpInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;
#ifdef WITH_TCP_BROADCAST
  if (n->client_network_broadcast_address) {
    if (ClientLinuxUdpInit(n, &tcpNetwork->udp_multicast_network) < 0) {
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
  }

  n->client_network_init = ClientLinuxTcpInit;
  n->client_network_connect = ClientLinuxTcpConnect;
  n->client_network_disconnect = ClientLinuxTcpDisconnect;
  n->client_network_receive = ClientLinuxTcpReceive;
  n->client_network_send = ClientLinuxTcpSend;
  return 0;
}

int ClientLinuxTcpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;

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
#ifdef WITH_TCP_BROADCAST
  if (n->client_network_broadcast_address) {
    if (ClientLinuxUdpConnect(n, &tcpNetwork->udp_multicast_network) < 0) {
      return -1;
    }
  }
#endif
  return 0;
}

void ClientLinuxTcpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;
  if (tcpNetwork->listen_socket_fd > -1) {
    close(tcpNetwork->listen_socket_fd);
    tcpNetwork->listen_socket_fd = -1;
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, tcpNetwork->protocol, n->client_network_address);
#endif
  }
  for (int i = 0; i < tcpNetwork->max_clients; ++i) {
#ifdef WITH_DEBUG_LOGGING
    if (tcpNetwork->client_socket_fds[i] >= 0) {
      device_address address = get_device_address_from_tcp_file_descriptor(tcpNetwork->client_socket_fds[i]);
      log_closed_connection(n->logger, tcpNetwork->protocol, &address);
    }
#endif
    close_client_connection(n, tcpNetwork, i);
  }
#ifdef WITH_TCP_BROADCAST
  if (n->client_network_broadcast_address) {
    ClientLinuxUdpDisconnect(n, &tcpNetwork->udp_multicast_network);
#ifdef WITH_LOGGING
    log_close_multicast_socket(n->logger, tcpNetwork->protocol, n->client_network_broadcast_address);
#endif
  }
#endif
}

int ClientLinuxTcpReceive(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int32_t timeout_ms,
                          void *context) {
  MqttSnClientTcpNetwork *tcpNetwork = (MqttSnClientTcpNetwork *) context;

  fd_set read_fds;
#ifdef WITH_TCP_BROADCAST
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

  if (select_rc == 1 || select_rc == 4 || select_rc == 5 || select_rc == 7) {
    // if something happened on the master socket, then it is an incoming connection
    if (MqttSnClientHandleMasterSocket(n, tcpNetwork) != 0) {
      return -1;
    }
  }

  if (select_rc == 2 || select_rc == 4 || select_rc == 6 || select_rc == 7) {
    // some IO operation on some other socket
    MqttSnClientHandleClientSockets(n, tcpNetwork, receiveBuffer, &read_fds);
  }
#ifdef WITH_TCP_BROADCAST
  // TODO we also receive message on the multicast port and forward them
  if (n->client_network_broadcast_address) {
    if (select_rc == 3 || select_rc == 5 || select_rc == 6 || select_rc == 7) {
      if (ClientLinuxUdpReceive(n, receiveBuffer, 0, &tcpNetwork->udp_multicast_network) < 0) {
        return -1;
      }
    }
  }
#endif

  return 0;
}

int ClientLinuxTcpSend(MqttSnClientNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int32_t timeout_ms,
                       void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;

  if (isEmpty(sendBuffer)) {
    return 0;
  }

#ifdef WITH_TCP_BROADCAST
  if (n->client_network_broadcast_address) {
    const MqttSnMessageData *front_msg = front(sendBuffer);
    if (!front_msg) {
      return 0;
    }
    if (front_msg->broadcast_radius &&
        (memcmp(&front_msg->address, n->client_network_broadcast_address, sizeof(device_address)) == 0)) {
      MqttSnMessageData clientSendMessage = {0};
      if (pop(sendBuffer, &clientSendMessage) != 0) {
        return 0;
      }
      // is broadcast message => send via udp
      MqttSnFixedSizeRingBuffer tmp_sendQueue = {0};
      MqttSnFixedSizeRingBufferInit(&tmp_sendQueue);
      if (put(&tmp_sendQueue, &clientSendMessage) < 0) {
        // error which should never happen
        return -1;
      }
      if (ClientLinuxUdpSend(n, &tmp_sendQueue, 0, &clientTcpNetwork->udp_multicast_network) < 0) {
        return -1;
      }
      return 0;
    }
  }
#endif

  MqttSnMessageData clientSendMessage = {0};
  if (pop(sendBuffer, &clientSendMessage) != 0) {
    return 0;
  }

  // convert device_address of clientSendMessage to IP and port
  for (int client_socket_fd_position = 0;
       client_socket_fd_position < clientTcpNetwork->max_clients;
       ++client_socket_fd_position) {
    int client_socket_fd = clientTcpNetwork->client_socket_fds[client_socket_fd_position];
    if (clientTcpNetwork->client_socket_fds[client_socket_fd_position] < 0) {
      continue;
    }

    device_address peer_address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
    if (memcmp(peer_address.bytes, clientSendMessage.address.bytes, sizeof(device_address)) != 0) {
      continue;
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

    if (setsockopt(clientTcpNetwork->client_socket_fds[client_socket_fd_position],
                   SOL_SOCKET,
                   SO_SNDTIMEO,
                   (char *) &interval,
                   sizeof(struct timeval))) {
      return -1;
    }

    ssize_t client_send_rc = send(clientTcpNetwork->client_socket_fds[client_socket_fd_position],
                                  clientSendMessage.data,
                                  clientSendMessage.data_length,
                                  0);
    if (client_send_rc < 0) {
#ifdef WITH_DEBUG_LOGGING
      device_address address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
      log_lost_connection(n->logger, clientTcpNetwork->protocol, &address);
#endif
      close_client_connection(n, clientTcpNetwork, client_socket_fd_position);
      continue;
    }
#ifdef WITH_DEBUG_LOGGING
    log_db_send_client_message(n->logger,
                               n->mqtt_sn_gateway_address,
                               &clientSendMessage.address,
                               clientSendMessage.data,
                               clientSendMessage.data_length);
#endif
  }

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

void MqttSnClientHandleClientSockets(MqttSnClientNetworkInterface *n,
                                     MqttSnClientTcpNetwork *clientTcpNetwork,
                                     MqttSnFixedSizeRingBuffer *receiveBuffer,
                                     fd_set *read_fds) {
  for (int client_socket_fd_position = 0;
       client_socket_fd_position < clientTcpNetwork->max_clients;
       client_socket_fd_position++) {
    int client_socket_fd = clientTcpNetwork->client_socket_fds[client_socket_fd_position];
    if (client_socket_fd < 0) {
      continue;
    }
    if (FD_ISSET(client_socket_fd, read_fds) == 0) {
      continue;
    }
    // TODO receive_rc values are not all implemented
    int receive_rc = save_received_messages_from_tcp_socket_into_receive_buffer(clientTcpNetwork,
                                                                                receiveBuffer,
                                                                                client_socket_fd_position);
    if (receive_rc == -2) {
      // connection was close by client
#ifdef WITH_DEBUG_LOGGING
      device_address address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
      log_client_disconnected(n->logger, clientTcpNetwork->protocol, &address);
#endif
    }
    if (receive_rc == -1) {
      // connection to client lost
#ifdef WITH_DEBUG_LOGGING
      device_address address = get_device_address_from_tcp_file_descriptor(client_socket_fd);
      log_lost_connection(n->logger, clientTcpNetwork->protocol, &address);
#endif
      close_client_connection(n, clientTcpNetwork, client_socket_fd_position);
      continue;
    }
#ifdef WITH_DEBUG_LOGGING
    if (receive_rc > 0) {
      for (int32_t msg_counter = 0; msg_counter < receive_rc; ++msg_counter) {
        const MqttSnMessageData *msg = getBack(receiveBuffer, ((receive_rc - 1) - msg_counter));
        log_db_rec_client_message(n->logger, &msg->address, n->mqtt_sn_gateway_address, msg->data, msg->data_length);
      }
    }
#endif
  }
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
    return 0;
  }

  int new_socket;
  int addrlen;
  struct sockaddr_in address;
  addrlen = sizeof(address);
  if ((new_socket = accept(tcpNetwork->listen_socket_fd,
                           (struct sockaddr *) &address,
                           (socklen_t *) &addrlen)) < 0) {
    return -1;
  }
  tcpNetwork->client_socket_fds[empty_socket_fd_position] = new_socket;
#ifdef WITH_DEBUG_LOGGING
  device_address new_client_address = get_device_address_from_sockaddr_in(&address);
  log_new_connection(n->logger, tcpNetwork->protocol, &new_client_address);
#endif
  return 0;
}
