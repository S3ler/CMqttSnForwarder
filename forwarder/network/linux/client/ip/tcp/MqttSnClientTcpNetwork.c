//
// Created by bele on 27.01.19.
//

#include "MqttSnClientTcpNetwork.h"
#include <forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <forwarder/network/linux/shared/shared/IpHelper.h>

int ClientLinuxTcpInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
  clientTcpNetwork->master_socket = -1;
  strcpy(clientTcpNetwork->protocol, "tcp");
  clientTcpNetwork->max_clients = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS;
  for (int i = 0; i < clientTcpNetwork->max_clients; ++i) {
    clientTcpNetwork->client_socket[i] = -1;
    memset(clientTcpNetwork->client_buffer[i], 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
    clientTcpNetwork->client_buffer_bytes[i] = 0;
  }

  n->client_network_init = ClientLinuxTcpInit;
  n->client_network_connect = ClientLinuxTcpConnect;
  n->client_network_disconnect = ClientLinuxTcpDisconnect;
  n->client_network_receive = ClientLinuxTcpReceive;
  n->client_network_send = ClientLinuxTcpSend;
  return 0;
}

int ClientLinuxTcpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
  uint16_t port = get_port_from_device_address(n->client_network_address);

  int master_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(struct sockaddr_in);

  // create master socket
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  int option = 1;
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    return -1;
  }

  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(master_socket, (struct sockaddr *) &address, addrlen) < 0) {
    return -1;
  }

  if (listen(master_socket, CMQTTSNFORWARDER_MAXIMUM_PENDING_CONNECTIONS) < 0) {
    return -1;
  }
  clientTcpNetwork->master_socket = master_socket;
#ifdef WITH_LOGGING
  if (n->logger) {
    log_open_socket(n->logger,
                    clientTcpNetwork->protocol,
                    n->client_network_address);
  }
#endif
  return 0;
}

void ClientLinuxTcpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
  if (clientTcpNetwork->master_socket >= 0) {
    close(clientTcpNetwork->master_socket);
    clientTcpNetwork->master_socket = -1;
  }
  for (int i = 0; i < clientTcpNetwork->max_clients; ++i) {
#ifdef WITH_DEBUG_LOGGING
    if (n->logger && clientTcpNetwork->client_socket[i] >= 0) {
      device_address address = get_device_address_from_file_descriptor(clientTcpNetwork->client_socket[i]);
      log_close_connection(n->logger, clientTcpNetwork->protocol, &address);
    }
#endif
    close_client_connection(n, clientTcpNetwork, i);
  }
  n->status = -1;
}

int ClientLinuxTcpReceive(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int32_t timeout_ms,
                          void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;

  int maximum_socket_descriptor;
  fd_set read_file_descriptor_set;

  MqttSnClientNetworkInitReadFdSet(clientTcpNetwork, &maximum_socket_descriptor, &read_file_descriptor_set);

  int activity = 0;
  if (timeout_ms < 0) {
    activity = select(maximum_socket_descriptor + 1, &read_file_descriptor_set, NULL, NULL, NULL);
  } else {
    struct timeval interval = {.tv_sec = timeout_ms / 1000, .tv_usec = (timeout_ms % 1000) * 1000};
    activity = select(maximum_socket_descriptor + 1, &read_file_descriptor_set, NULL, NULL, &interval);
  }

  if ((activity < 0) && (errno != EINTR)) {
    log_select_error(n->logger);
    return -1;
  }
  if (activity == 0) {
    return 0;
  }

  // if something happened on the master socket, then it is an incoming connection
  if (MqttSnClientHandleMasterSocket(n, clientTcpNetwork, &read_file_descriptor_set) != 0) {
    return -1;
  }

  // else its some IO operation on some other socket
  MqttSnClientHandleClientSockets(n, clientTcpNetwork, receiveBuffer, &read_file_descriptor_set);

  return 0;
}

void MqttSnClientNetworkInitReadFdSet(const MqttSnClientTcpNetwork *clientTcpNetwork, int *maximum_socket_descriptor,
                                      fd_set *read_file_descriptor_set) {
  FD_ZERO(read_file_descriptor_set);

  // add master socket to set
  FD_SET(clientTcpNetwork->master_socket, read_file_descriptor_set);
  (*maximum_socket_descriptor) = clientTcpNetwork->master_socket;

  // add child sockets to set
  for (int i = 0; i < clientTcpNetwork->max_clients; i++) {
    // socket descriptor
    int socket_descriptor = clientTcpNetwork->client_socket[i];

    // if valid socket descriptor then add to read list
    if (socket_descriptor > 0) {
      FD_SET(socket_descriptor, read_file_descriptor_set);
    }

    // highest file descriptor number, need it for the select function
    if (socket_descriptor > (*maximum_socket_descriptor)) {
      (*maximum_socket_descriptor) = socket_descriptor;
    }
  }
}

int ClientLinuxTcpSend(MqttSnClientNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int32_t timeout_ms,
                       void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;

  MqttSnMessageData clientSendMessage = {0};
  if (pop(sendBuffer, &clientSendMessage) != 0) {
    return 0;
  }

  // convert device_address of clientSendMessage to IP and port
  for (int i = 0; i < clientTcpNetwork->max_clients; ++i) {
    if (clientTcpNetwork->client_socket[i] <= 0) {
      continue;
    }

    device_address peer_address = get_device_address_from_file_descriptor(clientTcpNetwork->client_socket[i]);
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

    if (setsockopt(clientTcpNetwork->client_socket[i],
                   SOL_SOCKET,
                   SO_SNDTIMEO,
                   (char *) &interval,
                   sizeof(struct timeval))) {
      return -1;
    }

    ssize_t rc = send(clientTcpNetwork->client_socket[i], clientSendMessage.data, clientSendMessage.data_length, 0);
    if (rc < 0) {
#ifdef WITH_DEBUG_LOGGING
      if (n->logger) {
        device_address address = get_device_address_from_file_descriptor(clientTcpNetwork->client_socket[i]);
        log_lost_connection(n->logger, clientTcpNetwork->protocol, &address);
      }
#endif
      close_client_connection(n, clientTcpNetwork, i);
      continue;
    }
#ifdef WITH_DEBUG_LOGGING
    if (n->logger) {
      if (log_db_send_client_message(n->logger,
                                     n->mqtt_sn_gateway_address,
                                     &clientSendMessage.address,
                                     clientSendMessage.data,
                                     clientSendMessage.data_length)) {
        return -1;
      }
    }
#endif
  }

  return 0;
}

void close_client_connection(MqttSnClientNetworkInterface *n, MqttSnClientTcpNetwork *clientTcpNetwork, int i) {
  if (clientTcpNetwork->client_socket[i] >= 0) {
    close(clientTcpNetwork->client_socket[i]);
    clientTcpNetwork->client_socket[i] = -1;
    memset(clientTcpNetwork->client_buffer[i], 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
    clientTcpNetwork->client_buffer_bytes[i] = 0;
  }
}

int save_received_messages_from_tcp_socket_into_receive_buffer(MqttSnClientTcpNetwork *clientTcpNetwork,
                                                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                               int client_socket_position) {
  int client_fd = clientTcpNetwork->client_socket[client_socket_position];
  uint8_t buffer[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH];
  ssize_t read_bytes = 0;
  if ((read_bytes = read(client_fd, buffer, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH)) <= 0) {
    return -1;
  }

  device_address client_address = get_device_address_from_file_descriptor(client_fd);
  return save_tcp_messages_into_receive_buffer(buffer,
                                               read_bytes,
                                               client_address,
                                               clientTcpNetwork->client_buffer[client_socket_position],
                                               &clientTcpNetwork->client_buffer_bytes[client_socket_position],
                                               receiveBuffer);
}

void MqttSnClientHandleClientSockets(MqttSnClientNetworkInterface *n,
                                     MqttSnClientTcpNetwork *clientTcpNetwork,
                                     MqttSnFixedSizeRingBuffer *receiveBuffer,
                                     fd_set *readfds) {
  for (int client_socket_position = 0;
       client_socket_position < clientTcpNetwork->max_clients;
       client_socket_position++) {
    int client_fd = clientTcpNetwork->client_socket[client_socket_position];

    if (FD_ISSET(client_fd, readfds) == 0) {
      continue;
    }
    if (save_received_messages_from_tcp_socket_into_receive_buffer(clientTcpNetwork,
                                                                   receiveBuffer,
                                                                   client_socket_position) != 0) {
#ifdef WITH_DEBUG_LOGGING
      if (n->logger) {
        device_address
            address = get_device_address_from_file_descriptor(clientTcpNetwork->client_socket[client_socket_position]);
        log_lost_connection(n->logger, clientTcpNetwork->protocol, &address);
      }
#endif
      close_client_connection(n, clientTcpNetwork, client_socket_position);
      continue;
    }
#ifdef WITH_DEBUG_LOGGING
    if (n->logger) {
      const MqttSnMessageData *msg = back(receiveBuffer);
      log_db_rec_client_message(n->logger,
                                &msg->address,
                                n->mqtt_sn_gateway_address,
                                msg->data,
                                msg->data_length);
    }
#endif
  }
}

int MqttSnClientHandleMasterSocket(MqttSnClientNetworkInterface *n,
                                   MqttSnClientTcpNetwork *clientTcpNetwork,
                                   fd_set *readfds) {
  if (!FD_ISSET(clientTcpNetwork->master_socket, readfds)) {
    return 0;
  }
  int empty_socket_position = -1;


  //find empty socket position
  for (int i = 0; i < clientTcpNetwork->max_clients; ++i) {
    if (clientTcpNetwork->client_socket[i] == -1) {
      empty_socket_position = i;
      break;
    }
  }

  if (empty_socket_position < 0) {
    return 0;
  }

  int new_socket;
  int addrlen;
  struct sockaddr_in address;
  addrlen = sizeof(address);
  if ((new_socket = accept(clientTcpNetwork->master_socket, (struct sockaddr *) &address,
                           (socklen_t *) &addrlen)) < 0) {
    return -1;
  }
  clientTcpNetwork->client_socket[empty_socket_position] = new_socket;
#ifdef WITH_DEBUG_LOGGING
  if (n->logger) {
    device_address new_client_address = get_device_address_from_sockaddr_in(&address);
    log_new_connection(n->logger, clientTcpNetwork->protocol, &new_client_address);
  }
#endif
  return 0;
}
