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

#define MQTT_SN_MDNS_IP     "224.0.0.251"
#define MQTT_SN_MDNS_PORT   5353
#define CLIENT_PORT 11111
#define MAX_MSG_LEN 512
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
  log_open_socket(&logger, "unicast udp", &unicast_device_address);

  int multicast_socket = initialize_udp_multicast_socket(unicast_socket);
  if (multicast_socket < 0) {
    perror("error mullticast_socket");
    exit(EXIT_FAILURE);
  }
  printf("Ready: %d", CLIENT_PORT);
  fflush(stdout);

  uint8_t buf[MAX_MSG_LEN];
  while (1) {
    memset(buf, 0, sizeof(buf));
    int numRead = read(0, buf, MAX_MSG_LEN);
    if (numRead > 0) {
      printf("You said: %s", buf);
      fflush(stdout);

      if (strcmp((char *) buf, "exit\n") == 0) {
        break;
      }

      device_address to = {0};

      struct sockaddr_in sockaddr;
      sockaddr.sin_family = AF_INET;
      sockaddr.sin_addr.s_addr = inet_addr(MQTT_SN_MDNS_IP);
      sockaddr.sin_port = htons(MQTT_SN_MDNS_PORT);
      to = get_device_address_from_sockaddr_in(&sockaddr);
      if (send_udp_message(unicast_socket, &to, buf, numRead) != numRead) {
        perror("error send_udp_mutlicast_message");
        exit(EXIT_FAILURE);
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
  }
  return EXIT_SUCCESS;
}
