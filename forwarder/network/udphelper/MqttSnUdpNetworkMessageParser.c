//
// Created by SomeDude on 16.03.2019.
//

#include "MqttSnUdpNetworkMessageParser.h"
#include <string.h>
#include <network/gateway/udp/MqttSnGatewayUdpNetwork.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <network/iphelper/MqttSnIpNetworkHelper.h>

int save_udp_messages_into_receive_buffer(uint8_t *buffer,
                                          ssize_t read_bytes,
                                          device_address gateway_address,
                                          MqttSnFixedSizeRingBuffer *receiveBuffer) {

  if (isThreeBytesUdpHeader(buffer, read_bytes) &&
      !isCompleteThreeBytesUdpHeader(buffer, read_bytes)) {
    return 0;
  }

  if (get_udp_message_length(buffer) < 3) {
    return 0;
  }

  if (save_complete_new_udp_message(buffer,
                                    read_bytes,
                                    gateway_address,
                                    receiveBuffer) == 0) {
    return 0;
  }
  return 0;
}

uint16_t get_udp_message_length(uint8_t *data) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->three_octets_length_field_indicator == 0x01) {
    uint16_t length = 0;
    length += (((uint16_t) threeOctetsLengthField->msb_length) << 8);
    length += (((uint16_t) threeOctetsLengthField->lsb_length) << 0);
    return length;
  }
  MqttSnMessageHeaderOneOctetLengthField
      *oneOctetLengthField = (MqttSnMessageHeaderOneOctetLengthField *) data;
  return oneOctetLengthField->length;
}

int isCompleteThreeBytesUdpHeader(uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->three_octets_length_field_indicator == 0x01 && data_length > 2) {
    return 1;
  }
  return 0;
}

int isThreeBytesUdpHeader(uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->three_octets_length_field_indicator == 0x01) {
    return 1;
  }
  return 0;
}

int save_complete_new_udp_message(uint8_t *data,
                                  ssize_t data_length,
                                  device_address address,
                                  MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (data_length != get_udp_message_length(data)) {
    return -1;
  }
  save_udp_message_into_receive_buffer(data, (uint16_t) data_length, address, receiveBuffer);
  return 0;
}

int save_udp_message_into_receive_buffer(uint8_t *data,
                                         uint16_t data_length,
                                         device_address address,
                                         MqttSnFixedSizeRingBuffer *receiveBuffer) {
  MqttSnMessageData receiveMessageData = {0};
  receiveMessageData.data_length = data_length;
  memcpy(receiveMessageData.data, data, receiveMessageData.data_length);
  receiveMessageData.address = address;
  put(receiveBuffer, &receiveMessageData);
  return 0;
}

int initialize_udp_socket(uint16_t port) {
  int socket_fd;

  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    close(socket_fd);
    return -1;
  }

  int option = true;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    close(socket_fd);
    return -1;
  }

  // type of socket created
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    close(socket_fd);
    return -1;
  }

  return socket_fd;
}

ssize_t send_udp_message(int socket_fd, const device_address *to, const uint8_t *data, uint16_t data_length) {
  struct sockaddr_in to_sockaddr = get_sockaddr_in_from_device_address(to);
  socklen_t to_sockaddr_socklen = sizeof(to_sockaddr);

  int flags = 0;
  ssize_t send_bytes =
      sendto(socket_fd, data, data_length, flags, (const struct sockaddr *) &to_sockaddr, to_sockaddr_socklen);
  if (send_bytes == -1) {
    return -1;
  }
  return send_bytes;
}

int is_udp_message_received(int socket_fd, uint32_t timeout_ms) {
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  fd_set readfds;
  FD_ZERO(&readfds);

  FD_SET(socket_fd, &readfds);
  int max_sd = socket_fd;

  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);

  if ((activity < 0) && (errno != EINTR)) {
    return -1;
  }
  if (activity == 0) {
    return 0;
  }
  if (FD_ISSET(socket_fd, &readfds) == 0) {
    return 0;
  }
  return 1;
}

int receive_udp_message(int socket_fd, MqttSnFixedSizeRingBuffer *receiveBuffer, uint16_t max_data_length) {
  uint8_t buffer[max_data_length];
  memset(buffer, 0, max_data_length);
  int buffer_length = max_data_length;
  ssize_t read_bytes;

  struct sockaddr_in recv_sockaddr;
  memset(&recv_sockaddr, 0, sizeof(recv_sockaddr));
  socklen_t recv_sockaddr_socklen = sizeof(recv_sockaddr);

  // TODO what happens if too much data are send aka packetsize > MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE
  // TODO read out first 3 bytes, peek datasize, if too long remove next datasize bytes out of buffer
  // TODO write a testcase for this behaviour

  if ((read_bytes = recvfrom(socket_fd, buffer, buffer_length, MSG_WAITALL,
                             (struct sockaddr *) &recv_sockaddr, &recv_sockaddr_socklen)) <= 0) {
    return -1;
  }

  device_address received_address = get_device_address_from_sockaddr_in(&recv_sockaddr);

  return save_udp_messages_into_receive_buffer(buffer,
                                               read_bytes,
                                               received_address,
                                               receiveBuffer);
}
