//
// Created by bele on 27.01.19.
//

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <network/tcphelper/MqttSnTcpNetworkMessageParser.h>

#include "MqttSnClientTcpNetwork.h"
#include "../../../MqttSnClientNetworkInterface.h"
#include "../../../MqttSnFixedSizeRingBuffer.h"

int ClientLinuxTcpInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
  clientTcpNetwork->master_socket = -1;
  clientTcpNetwork->max_clients = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS;
  for (int i = 0; i < clientTcpNetwork->max_clients; ++i) {
    clientTcpNetwork->client_socket[i] = -1;
    memset(clientTcpNetwork->client_buffer[i], 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
    clientTcpNetwork->client_buffer_bytes[i] = 0;
  }
  uint16_t port = n->client_network_address->bytes[5];
  port += (uint16_t) n->client_network_address->bytes[4] << 8;

  clientTcpNetwork->port = port;
  n->client_network_init = ClientLinuxTcpInit;
  n->client_network_connect = ClientLinuxTcpConnect;
  n->client_network_disconnect = ClientLinuxTcpDisconnect;
  n->client_network_receive = ClientLinuxTcpReceive;
  n->client_network_send = ClientLinuxTcpSend;
  return 0;
}

int ClientLinuxTcpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;

  int master_socket;
  struct sockaddr_in address;

  int addrlen = sizeof(struct sockaddr_in);

  // create master socket
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    return -1;
    exit(EXIT_FAILURE);
  }

  // set master socket to allow multiple connections,
  // this is just a good habit, it will work without this
  int option = 1;
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    perror("setsockopt");
    return -1;
    exit(EXIT_FAILURE);
  }

  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(clientTcpNetwork->port);

  // bind the socket to the localhost clientTcpNetwork->port
  if (bind(master_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
    perror("bind failed");
    return -1;
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", clientTcpNetwork->port);

  // try to specify maximum of 3 pending connections for the master socket
  if (listen(master_socket, 3) < 0) {
    // TODO find out more about the functionality here
    perror("listen");
    return -1;
    exit(EXIT_FAILURE);
  }
  clientTcpNetwork->master_socket = master_socket;

  // accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");
  return 0;
}

void ClientLinuxTcpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  n->status = -1; // TODO check if -1 setting here is correct
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
  if (clientTcpNetwork->master_socket != -1) {
    close(clientTcpNetwork->master_socket);
    clientTcpNetwork->master_socket = -1;
  }
  for (int i = 0; i < clientTcpNetwork->max_clients; ++i) {
    if (clientTcpNetwork->client_socket[i] != -1) {
      close(clientTcpNetwork->client_socket[i]);
      clientTcpNetwork->client_socket[i] = -1;
    }
  }
}

int ClientLinuxTcpReceive(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          uint32_t timeout_ms,
                          void *context) {

  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
  int max_sd;
  fd_set readfds;
  // prepare timeout interval
  // TODO; timeout_ms = 100
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  MqttSnClientNetworkInitReadFdSet(clientTcpNetwork, &max_sd, &readfds);

  // wait for an activity on one of the sockets, timout is NULL, so wait indefinitely
  // int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
  // TODO exchange with timeout!

  // wait for an activity on one of the sockets, timout is interval, so wait until interval
  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
  // int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

  if ((activity < 0) && (errno != EINTR)) {
    printf("select error");
    printf("%s\n", strerror(errno));
    return -1;
  }
  if (activity == 0) {
    // printf("select timeout\n");
    fflush(stdout);
    return 0;
  }

  // MqttSnClientNetworkInitReadFdSet(clientTcpNetwork, &max_sd, &readfds);

  // if something happened on the master socket, then it is an incoming connection
  if (MqttSnClientHandleMasterSocket(clientTcpNetwork, &readfds) != 0) {
    return -1;
  }

  // else its some IO operation on some other socket
  MqttSnClientHandleClientSockets(clientTcpNetwork, receiveBuffer, &readfds);

  return 0;
}

void MqttSnClientNetworkInitReadFdSet(const MqttSnClientTcpNetwork *clientTcpNetwork, int *max_sd,
                                      fd_set *readfds) {// clear the socket set
  FD_ZERO(readfds);

  // add master socket to set
  FD_SET(clientTcpNetwork->master_socket, readfds);
  (*max_sd) = clientTcpNetwork->master_socket;

  // add child sockets to set
  for (int i = 0; i < clientTcpNetwork->max_clients; i++) {
    // socket descriptor
    int sd = clientTcpNetwork->client_socket[i];

    // if valid socket descriptor then add to read list
    if (sd > 0) {
      FD_SET(sd, readfds);
    }

    // highest file descriptor number, need it for the select function
    if (sd > (*max_sd)) {
      (*max_sd) = sd;
    }
  }
}

int ClientLinuxTcpSend(MqttSnClientNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                       void *context) {
  // TODO: at the moment only 1 packet is sendNetwork per call => can lead to client starvation
  // minimum 1 packet
  // with timeout
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;

  MqttSnMessageData sendMessage = {0};

  if (pop(sendBuffer, &sendMessage) != 0) {
    return 0;
  }

  // convert device_address of sendMessage to IP and port
  for (int i = 0; i < clientTcpNetwork->max_clients; ++i) {
    device_address peer_address = {0};

    if (clientTcpNetwork->client_socket[i] <= 0) {
      continue;
    }
    if (getDeviceAddressFromFileDescriptor(clientTcpNetwork->client_socket[i], &peer_address) != 0) {
      continue;
    }
    if (memcmp(peer_address.bytes, sendMessage.address.bytes, sizeof(device_address)) != 0) {
      continue;
    }

    if (send(clientTcpNetwork->client_socket[i], sendMessage.data, sendMessage.data_length, 0) !=
        sendMessage.data_length) {
      close_client_connection(clientTcpNetwork, i);
    }

  }

  return 0;
}

void close_client_connection(MqttSnClientTcpNetwork *clientTcpNetwork, int i) {
  close(clientTcpNetwork->client_socket[i]);
  clientTcpNetwork->client_socket[i] = -1;
}

int getDeviceAddressFromFileDescriptor(int peer_fd, device_address *peer_address) {
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  getpeername(peer_fd, (struct sockaddr *) &address, &addrlen);

  unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;
  (*peer_address).bytes[0] = ip[0];
  (*peer_address).bytes[1] = ip[1];
  (*peer_address).bytes[2] = ip[2];
  (*peer_address).bytes[3] = ip[3];

  uint16_t port_as_number = (uint16_t) htons(address.sin_port);
  (*peer_address).bytes[4] = (uint8_t) (port_as_number >> 8);
  (*peer_address).bytes[5] = (uint8_t) (port_as_number >> 0);
  return 0;
}

int save_received_messages_from_tcp_socket_into_receive_buffer(MqttSnClientTcpNetwork *clientTcpNetwork,
                                                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                               int client_socket_position) {
  int client_fd = clientTcpNetwork->client_socket[client_socket_position];
  uint16_t buffer_length = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH;
  uint8_t buffer[CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH];
  ssize_t read_bytes = 0;
  if ((read_bytes = read(client_fd, buffer, buffer_length)) <= 0) {
    return -1;
  }

  device_address client_address = get_client_device_address(client_fd);

  return save_messages_into_receive_buffer(buffer,
                                           read_bytes,
                                           client_address,
                                           clientTcpNetwork->client_buffer[client_socket_position],
                                           &clientTcpNetwork->client_buffer_bytes[client_socket_position],
                                           receiveBuffer);
/*
  if (clientTcpNetwork->client_buffer[client_socket_position][0] == 0) {
    // no message in buffer, new message, read message bytes
    if (save_incomplete_new_message(buffer,
                                    read_bytes,
                                    get_client_device_address(client_fd),
                                    clientTcpNetwork->client_buffer[client_socket_position],
                                    &clientTcpNetwork->client_buffer_bytes[client_socket_position],
                                    receiveBuffer) == 0) {
      return 0;
    }
    if (save_multiple_complete_new_messages(buffer,
                                            read_bytes,
                                            get_client_device_address(client_fd),
                                            clientTcpNetwork->client_buffer[client_socket_position],
                                            &clientTcpNetwork->client_buffer_bytes[client_socket_position],
                                            receiveBuffer) == 0) {
      return 0;
    }
    return -1;
  }
  // messages already in buffer, new message, read message bytes
  if (save_incomplete_message(buffer,
                              read_bytes,
                              get_client_device_address(client_fd),
                              clientTcpNetwork->client_buffer[client_socket_position],
                              &clientTcpNetwork->client_buffer_bytes[client_socket_position],
                              receiveBuffer) == 0) {
    return 0;
  }

  if (save_multiple_messages(buffer,
                             read_bytes,
                             get_client_device_address(client_fd),
                             clientTcpNetwork->client_buffer[client_socket_position],
                             &clientTcpNetwork->client_buffer_bytes[client_socket_position],
                             receiveBuffer) == 0) {
    return 0;
  }
  return -1;
  */
}

void MqttSnClientHandleClientSockets(MqttSnClientTcpNetwork *clientTcpNetwork, MqttSnFixedSizeRingBuffer *receiveBuffer,
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
      close(client_fd);
      clientTcpNetwork->client_socket[client_socket_position] = 0;
      memset(clientTcpNetwork->client_buffer[client_socket_position],
             0,
             CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
      clientTcpNetwork->client_buffer_bytes[client_socket_position] = 0;
    }
  }
}
device_address get_client_device_address(int client_file_descriptor) {

  struct sockaddr_in address;
  int addrlen = sizeof(struct sockaddr_in);
  device_address clientAddress = {0};

  getpeername(client_file_descriptor, (struct sockaddr *) &address, (socklen_t *) &addrlen);
  unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;

  clientAddress.bytes[0] = ip[0];
  clientAddress.bytes[1] = ip[1];
  clientAddress.bytes[2] = ip[2];
  clientAddress.bytes[3] = ip[3];

  uint16_t port_as_number = (uint16_t) htons(address.sin_port);
  clientAddress.bytes[4] = (uint8_t) (port_as_number >> 8);
  clientAddress.bytes[5] = (uint8_t) (port_as_number >> 0);

  return clientAddress;
}

int MqttSnClientHandleMasterSocket(MqttSnClientTcpNetwork *clientTcpNetwork, fd_set *readfds) {
  if (FD_ISSET(clientTcpNetwork->master_socket, readfds)) {
    int new_socket;
    int addrlen;
    struct sockaddr_in address;
    addrlen = sizeof(address);
    if ((new_socket = accept(clientTcpNetwork->master_socket, (struct sockaddr *) &address,
                             (socklen_t *) &addrlen)) < 0) {
      return -1;
    }

    // inform user of the socket number - used in gateway_send and client_network_receive commands
    printf("New connection, socket fd is %d, ip is: %s, port: %d\n",
           new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    // add new socket to array of sockets
    for (int client_socket_position = 0;
         client_socket_position < clientTcpNetwork->max_clients;
         ++client_socket_position) {
      // if position is empty
      if (clientTcpNetwork->client_socket[client_socket_position] == -1) {
        clientTcpNetwork->client_socket[client_socket_position] = new_socket;
        printf("Adding to list of sockets as %d\n", client_socket_position);

        break;
      }
    }
  }
  return 0;
}
