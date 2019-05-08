//
// Created by SomeDude on 05.05.2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h>
#include <forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h>
#include <string.h>
#include <forwarder/network/linux/shared/shared/IpHelper.h>
#include <forwarder/logging/linux/stdout/StdoutLogging.h>
#include <arpa/inet.h>
#include <forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h>

#ifndef MQTT_SN_MULTICAST_IP
#define MQTT_SN_MULTICAST_IP "224.1.1.103"
#endif
//#define MQTT_SN_MULTICAST_IP     "224.1.1.100" // gateway network
//#define MQTT_SN_MULTICAST_IP     "224.1.1.101" // client network
#ifndef MQTT_SN_MULTICAST_PORT
#define MQTT_SN_MULTICAST_PORT   5353
#endif
//#define MQTT_SN_FORWARDER_CLIENT_NETWORK_PORT   7777
#ifndef CLIENT_PORT
//#define CLIENT_PORT 11100
#endif
#define MAX_MSG_LEN 512

int is_read_available(int read_fd) {
  fd_set set;
  struct timeval timeout;
  int rv;
  FD_ZERO(&set); /* clear the set */
  FD_SET(read_fd, &set); /* add our file descriptor to the set */
  timeout.tv_sec = 0;
  timeout.tv_usec = 10000;

  rv = select(read_fd + 1, &set, NULL, NULL, &timeout);
  if (rv == -1) {
    perror("select"); /* an error accured */
    exit(EXIT_FAILURE);
  } else if (rv == 0) {
    // timeout
    return 0;
  }
  return 1;
}

int read_and_send(int unicast_socket) {
  if (!is_read_available(0)) {
    return 0;
  }
  uint8_t buf[MAX_MSG_LEN] = {0};
  int numRead = read(0, buf, MAX_MSG_LEN);
  if (numRead > 0) {
    printf("You said: %s", buf);
    fflush(stdout);

    if (strcmp((char *) buf, "exit\n") == 0) {
      return -1;
    }

    device_address to = {0};

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(MQTT_SN_MULTICAST_IP);
    sockaddr.sin_port = htons(MQTT_SN_MULTICAST_PORT);
    to = get_device_address_from_sockaddr_in(&sockaddr);
    if (send_udp_message(unicast_socket, &to, buf, numRead) != numRead) {
      perror("error send_udp_mutlicast_message");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}

int main() {
  MqttSnLogger logger = {0};
  MqttSnLoggerInit(&logger, LOG_LEVEL_DEBUG, stdout_log_init);

  int unicast_socket = initialize_udp_socket(CLIENT_PORT);
  if (unicast_socket < 0) {
    perror("error unicast_socket");
    exit(EXIT_FAILURE);
  }
  device_address unicast_device_address = {0};
  add_port_to_device_address(CLIENT_PORT, &unicast_device_address);
  log_opening_unicast_socket(&logger, "unicast udp", &unicast_device_address);

  device_address multicast_device_address = {0};
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(MQTT_SN_MULTICAST_IP);
  sockaddr.sin_port = htons(MQTT_SN_MULTICAST_PORT);
  multicast_device_address = get_device_address_from_sockaddr_in(&sockaddr);

  uint32_t ip;
  uint16_t port;
  get_ipv4_and_port_from_device_address(&ip, &port, &multicast_device_address);

  int multicast_socket = initialize_udp_multicast_socket(unicast_socket, ip, port);
  if (multicast_socket < 0) {
    perror("error multicast_socket");
    exit(EXIT_FAILURE);
  }
  printf("Ready: %d", CLIENT_PORT);
  fflush(stdout);

  while (1) {
    int read_rc = read_and_send(unicast_socket);
    if (read_rc < 0) {
      return EXIT_FAILURE;
    } else if (read_rc > 0) {
      return EXIT_SUCCESS;
    }

    if (is_multicast_message_receive(multicast_socket, 10)) {
      MqttSnMessageData msg = {0};
      device_address empty = {0};
      ssize_t read_bytes = -1;
      if (receive_udp_mutlicast_message(multicast_socket, msg.data, &read_bytes, sizeof(msg.data), &msg.address)
          == 0) {
        msg.data_length = read_bytes;
        log_db_rec_client_message(&logger,
                                  &msg.address,
                                  &empty,
                                  msg.data,
                                  msg.data_length);
      }
    }
  }
  return EXIT_SUCCESS;
}
