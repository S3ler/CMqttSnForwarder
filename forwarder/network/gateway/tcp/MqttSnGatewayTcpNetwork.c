//
// Created by bele on 26.01.19.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <network/tcphelper/MqttSnTcpNetworkMessageParser.h>
#include "MqttSnGatewayTcpNetwork.h"
#include "../../../MqttSnFixedSizeRingBuffer.h"

int GatewayLinuxTcpInit(struct MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
  tcpNetwork->my_socket = 0;
  n->gateway_receive = GatewayLinuxTcpReceive;
  n->gateway_send = GatewayLinuxTcpSend;
  n->gateway_network_connect = GatewayLinuxTcpConnect;
  n->gateway_network_disconnect = GatewayLinuxTcpDisconnect;
  return 0;
}

int GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  if (networkInterface->mqtt_sn_gateway_network_address == NULL) {
    // TODO implement searching for gateway
  }

  char ipAsString[255] = {0};
  sprintf(ipAsString,
          "%d.%d.%d.%d",
          networkInterface->mqtt_sn_gateway_network_address->bytes[0],
          networkInterface->mqtt_sn_gateway_network_address->bytes[1],
          networkInterface->mqtt_sn_gateway_network_address->bytes[2],
          networkInterface->mqtt_sn_gateway_network_address->bytes[3]);
  uint16_t port = (((uint16_t) networkInterface->mqtt_sn_gateway_network_address->bytes[4]) << 8)
      + ((uint16_t) networkInterface->mqtt_sn_gateway_network_address->bytes[5]);
  /*
  char *addr = tcpNetwork->ip;
  int port = tcpNetwork->port;
  */
  char *addr = ipAsString;
  if (addr == NULL || port == 0) {
    return -1;
  }

  int type = SOCK_STREAM;
  struct sockaddr_in address;
  int rc = -1;
  sa_family_t family = AF_INET;
  struct addrinfo *result = NULL;
  struct addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};

  if ((rc = getaddrinfo(addr, NULL, &hints, &result)) == 0) {
    struct addrinfo *res = result;

    /* prefer ip4 addresses */
    while (res) {
      if (res->ai_family == AF_INET) {
        result = res;
        break;
      }
      res = res->ai_next;
    }

    if (result->ai_family == AF_INET) {
      address.sin_port = htons(port);
      address.sin_family = family = AF_INET;
      address.sin_addr = ((struct sockaddr_in *) (result->ai_addr))->sin_addr;
    } else
      rc = -1;

    freeaddrinfo(result);
  }

  if (rc == 0) {
    tcpNetwork->my_socket = socket(family, type, 0);
    if (tcpNetwork->my_socket != -1)
      rc = connect(tcpNetwork->my_socket, (struct sockaddr *) &address, sizeof(address));
    else
      rc = -1;
  }

  return rc;
}

void GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
  if (tcpNetwork->my_socket != 0) {
    close(tcpNetwork->my_socket);
    tcpNetwork->my_socket = 0;
  }
}

int GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *receiveBuffer,
                           uint32_t timeout_ms,
                           void *context) {
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;

  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  // new
  int max_sd;
  fd_set readfds;
  MqttSnGatewaytNetworkInitReadFdSet(tcpNetwork, &max_sd, &readfds);


  // wait for an activity on one of the sockets, timout is interval, so wait until interval
  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
  // int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

  if ((activity < 0) && (errno != EINTR)) {
    return -1;
  }
  if (activity == 0) {
    // printf("gateway select timeout\n");
    return 0;
  }

  if (FD_ISSET(tcpNetwork->my_socket, &readfds) == 0) {
    return 0;
  }

  if (save_receive_gateway_message_from_tcp_socket_into_receive_buffer(tcpNetwork, receiveBuffer) != 0) {
    GatewayLinuxTcpDisconnect(n, context);
    return -1;
  }
  /*
  if (MqttSnGatewayHandleMasterSocket(tcpNetwork, receiveBuffer, &readfds) != 0) {
    return -1;
  }
  */
  /*
  setsockopt(tcpNetwork->my_socket, SOL_SOCKET, SO_RCVTIMEO, (char *) &interval, sizeof(struct timeval));

  // TODO exchange len and buffer
  int len = 255;
  uint8_t buffer[255] = {0};
  int bytes = 0;
  while (bytes < len) {
      int rc = recv(tcpNetwork->my_socket, &buffer[bytes], (size_t) (len - bytes), 0);
      if (rc == -1) {
          if (errno != EAGAIN && errno != EWOULDBLOCK)
              bytes = -1;
          break;
      } else if (rc == 0) {
          bytes = 0;
          break;
      } else
          bytes += rc;
  }

  if (bytes < 0) {
      return -1;
  }
  if (bytes == 0) {
      return 0;
  }
  MqttSnMessageData receivedMessageData = {0};
  // receivedMessageData.data = buffer;
  if (bytes >= MAX_MESSAGE_LENGTH) {
      // packet is ignored because it is too long
      return 0;
  }
  memcpy(receivedMessageData.data, buffer, bytes);
  receivedMessageData.data_length = bytes;

  // convert IP-Address to device_address
  struct sockaddr_in gatewayAddress;
  int addrlen = sizeof(struct sockaddr_in);
  getpeername(tcpNetwork->my_socket, (struct sockaddr *) &gatewayAddress, (socklen_t *) &addrlen);
  unsigned char *ip = (unsigned char *) &gatewayAddress.sin_addr.s_addr;
  receivedMessageData.gatewayAddress.bytes[0] = ip[0];
  receivedMessageData.gatewayAddress.bytes[1] = ip[1];
  receivedMessageData.gatewayAddress.bytes[2] = ip[2];
  receivedMessageData.gatewayAddress.bytes[3] = ip[3];

  uint16_t port_as_number = (uint16_t) htons(gatewayAddress.sin_port);
  receivedMessageData.gatewayAddress.bytes[4] = (uint8_t) (port_as_number >> 8);
  receivedMessageData.gatewayAddress.bytes[5] = (uint8_t) (port_as_number >> 0);

  put(receiveBuffer, &receivedMessageData);
  */

  return 0;
}

int GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n, MqttSnFixedSizeRingBuffer *sendBuffer, uint32_t timeout_ms,
                        void *context) {
  // TODO: implement later: sendNetwork more messages until timeout runs out
  MqttSnGatewayTcpNetwork *tcpNetwork = (MqttSnGatewayTcpNetwork *) context;
  MqttSnMessageData gatewaySendMessageData = {0};

  if (pop(sendBuffer, &gatewaySendMessageData) == 0) {
    struct timeval tv;
    tv.tv_sec = 0;  /* 30 Secs Timeout */
    tv.tv_usec = timeout_ms * 1000;  // Not Init'ing this can cause strange errors

    uint8_t *buffer = gatewaySendMessageData.data;
    uint16_t len = gatewaySendMessageData.data_length;

    setsockopt(tcpNetwork->my_socket, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv, sizeof(struct timeval));
    ssize_t rc = write(tcpNetwork->my_socket, gatewaySendMessageData.data, gatewaySendMessageData.data_length);

    if (rc != len) {
      put(sendBuffer, &gatewaySendMessageData);
      return -1;
    }
  }

  return 0;
}

void MqttSnGatewaytNetworkInitReadFdSet(MqttSnGatewayTcpNetwork *clientTcpNetwork, int *max_sd,
                                        fd_set *readfds) {// clear the socket set
  FD_ZERO(readfds);

  // add master socket to set
  FD_SET(clientTcpNetwork->my_socket, readfds);
  (*max_sd) = clientTcpNetwork->my_socket;
}

int MqttSnGatewayHandleMasterSocket(MqttSnGatewayTcpNetwork *clientTcpNetwork,
                                    MqttSnFixedSizeRingBuffer *receiveBuffer,
                                    fd_set *readfds) {
  if (FD_ISSET(clientTcpNetwork->my_socket, readfds)) {
    // TODO read
    int sd = clientTcpNetwork->my_socket;
    // Check if it was for closing, and also read the incoming message
    char buffer[1024];
    int buffer_length = 1024;
    int valread;
    if ((valread = read(sd, buffer, buffer_length)) == 0) {
      // Somebody disconnected, get his details and print
      int addrlen;
      struct sockaddr_in address;
      getpeername(sd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
      printf("Gateway disconnected, ip %s, port %d \n",
             inet_ntoa(address.sin_addr), ntohs(address.sin_port));

      // Close the socket and mark as 0 in the list for reuse
      close(sd);
      clientTcpNetwork->my_socket = 0;
      return -1;
    }
      // Echo back the message that came in
    else {
      // set the string terminating NULL byte on the end of the data read
      if (valread > MAX_MESSAGE_LENGTH) {
        return 0;
      }
      MqttSnMessageData receiveMessageData = {0};
      receiveMessageData.data_length = valread;
      memcpy(receiveMessageData.data, buffer, receiveMessageData.data_length);

      // convert IP-Address to device_address
      struct sockaddr_in address;
      int addrlen = sizeof(struct sockaddr_in);
      getpeername(clientTcpNetwork->my_socket, (struct sockaddr *) &address, (socklen_t *) &addrlen);
      unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;
      receiveMessageData.address.bytes[0] = ip[0];
      receiveMessageData.address.bytes[1] = ip[1];
      receiveMessageData.address.bytes[2] = ip[2];
      receiveMessageData.address.bytes[3] = ip[3];

      uint16_t port_as_number = (uint16_t) htons(address.sin_port);
      receiveMessageData.address.bytes[4] = (uint8_t) (port_as_number >> 8);
      receiveMessageData.address.bytes[5] = (uint8_t) (port_as_number >> 0);

      put(receiveBuffer, &receiveMessageData);
      // buffer[valread] = '\0';
      // sendNetwork(sd, buffer, strlen(buffer), 0);
    }

  }
  return 0;
}

int save_receive_gateway_message_from_tcp_socket_into_receive_buffer(
    MqttSnGatewayTcpNetwork *gatewayTcpNetwork,
    MqttSnFixedSizeRingBuffer *receiveBuffer) {
  int gateway_fd = gatewayTcpNetwork->my_socket;
  uint16_t buffer_length = CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH;
  uint8_t buffer[CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH];
  ssize_t read_bytes = 0;
  if ((read_bytes = read(gateway_fd, buffer, buffer_length)) <= 0) {
    return -1;
  }

  // convert IP-Address to device_address
  device_address gateway_address = {0};
  struct sockaddr_in address;
  int addrlen = sizeof(struct sockaddr_in);
  getpeername(gateway_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
  unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;
  gateway_address.bytes[0] = ip[0];
  gateway_address.bytes[1] = ip[1];
  gateway_address.bytes[2] = ip[2];
  gateway_address.bytes[3] = ip[3];

  uint16_t port_as_number = (uint16_t) htons(address.sin_port);
  gateway_address.bytes[4] = (uint8_t) (port_as_number >> 8);
  gateway_address.bytes[5] = (uint8_t) (port_as_number >> 0);

  return save_messages_into_receive_buffer(buffer,
                                           read_bytes,
                                           gateway_address,
                                           gatewayTcpNetwork->gateway_buffer,
                                           &gatewayTcpNetwork->gateway_buffer_bytes,
                                           receiveBuffer);
  /*
  if (gatewayTcpNetwork->gateway_buffer[0] == 0) {
    // no message in buffer, new message, read message bytes
    if (save_incomplete_new_message(buffer,
                                    read_bytes,
                                    gateway_address,
                                    gatewayTcpNetwork->gateway_buffer,
                                    &gatewayTcpNetwork->gateway_buffer_bytes,
                                    receiveBuffer) == 0) {
      return 0;
    }
    if (save_multiple_complete_new_messages(buffer,
                                            read_bytes,
                                            gateway_address,
                                            gatewayTcpNetwork->gateway_buffer,
                                            &gatewayTcpNetwork->gateway_buffer_bytes,
                                            receiveBuffer) == 0) {
      return 0;
    }
    return -1;
  }
  if (isThreeBytesHeader(gatewayTcpNetwork->gateway_buffer, gatewayTcpNetwork->gateway_buffer_bytes) &&
      !isCompleteThreeBytesHeader(gatewayTcpNetwork->gateway_buffer, gatewayTcpNetwork->gateway_buffer_bytes)) {
    if (gatewayTcpNetwork->gateway_buffer_bytes == 1) {
      if (read_bytes == 1) {
        // copy and increase - then we are done
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes] = buffer[0];
        gatewayTcpNetwork->gateway_buffer_bytes += 1;
        return 0;
      } else if (read_bytes == 2) {
        // copy and increase - then we are done
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes] = buffer[0];
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes + 1] = buffer[1];
        gatewayTcpNetwork->gateway_buffer_bytes += 2;
        return 0;
      } else {
        // copy into buffer but do not increase - rest ist handled in the following methods
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes] = buffer[0];
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes + 1] = buffer[1];
      }
    } else if (gatewayTcpNetwork->gateway_buffer_bytes == 2) {
      if (read_bytes == 1) {
        // copy and increase - then we are done
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes] = buffer[0];
        gatewayTcpNetwork->gateway_buffer_bytes += 1;
        return 0;
      } else {
        // copy into buffer but do not increase - rest ist handled in the following methods
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes] = buffer[0];
        gatewayTcpNetwork->gateway_buffer[gatewayTcpNetwork->gateway_buffer_bytes + 1] = buffer[1];
      }
    }

  }
  // messages already in buffer, new message, read message bytes
  if (save_incomplete_message(buffer,
                              read_bytes,
                              gateway_address,
                              gatewayTcpNetwork->gateway_buffer,
                              &gatewayTcpNetwork->gateway_buffer_bytes,
                              receiveBuffer) == 0) {
    return 0;
  }
  if (save_multiple_messages(buffer,
                             read_bytes,
                             gateway_address,
                             gatewayTcpNetwork->gateway_buffer,
                             &gatewayTcpNetwork->gateway_buffer_bytes,
                             receiveBuffer) == 0) {
    return 0;
  }
  return -1;
  */
  /*
// TODO read
  int sd = gatewayTcpNetwork->my_socket;
  // Check if it was for closing, and also read the incoming message
  char buffer[1024];
  int buffer_length = 1024;
  int valread;
  if ((valread = read(sd, buffer, buffer_length)) == 0) {
    // Somebody disconnected, get his details and print
    int addrlen;
    struct sockaddr_in address;
    getpeername(sd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
    printf("Gateway disconnected, ip %s, port %d \n",
           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    // Close the socket and mark as 0 in the list for reuse
    close(sd);
    clientTcpNetwork->my_socket = 0;
    return -1;
  }
    // Echo back the message that came in
  else {
    // set the string terminating NULL byte on the end of the data read
    if (valread > MAX_MESSAGE_LENGTH) {
      return 0;
    }
    MqttSnMessageData receiveMessageData = {0};
    receiveMessageData.data_length = valread;
    memcpy(receiveMessageData.data, buffer, receiveMessageData.data_length);

    // convert IP-Address to device_address
    struct sockaddr_in address;
    int addrlen = sizeof(struct sockaddr_in);
    getpeername(clientTcpNetwork->my_socket, (struct sockaddr *) &address, (socklen_t *) &addrlen);
    unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;
    receiveMessageData.address.bytes[0] = ip[0];
    receiveMessageData.address.bytes[1] = ip[1];
    receiveMessageData.address.bytes[2] = ip[2];
    receiveMessageData.address.bytes[3] = ip[3];

    uint16_t port_as_number = (uint16_t) htons(address.sin_port);
    receiveMessageData.address.bytes[4] = (uint8_t) (port_as_number >> 8);
    receiveMessageData.address.bytes[5] = (uint8_t) (port_as_number >> 0);

    put(receiveBuffer, &receiveMessageData);
    // buffer[valread] = '\0';
    // sendNetwork(sd, buffer, strlen(buffer), 0);
  }
  return 0;
   */
}