//
// Created by bele on 28.01.19.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <netdb.h>
#include <errno.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "forwarder/global_defines.h"

#define FORWARDER_IP "127.0.0.1"
#define FORWARDER_CLIENT_NETWORK_PORT 7777
#define GATEWAY_PORT 8888
#define CLIENT_PORT 11111
#define MAX_MSG_LEN 255
//#define FORWARDER_HEADER_LEN 3
#define PUBLISH_HEADER_LEN 7

#pragma pack(push, 1)
typedef struct MQTT_SN_PUBLISH {
  uint8_t length;
  uint8_t msg_type;
  uint8_t flags;
  uint16_t topic_id;
  uint16_t msg_id;
  uint8_t data[MAX_MSG_LEN - PUBLISH_HEADER_LEN];
} MQTT_SN_PUBLISH;
#pragma pack(pop)
//TODO send publish messages

int MqttSnPublishInit(MQTT_SN_PUBLISH *publish, uint8_t *data, uint16_t length);

int printMqttSnPublish(char *from, MQTT_SN_PUBLISH *publish);

int openGatewaySocket(int port);

int connectForwarder(char *ip, int port);

int send_fd(int fd, uint8_t *buf, int buf_len);

int recv_fd(int fd, uint8_t *buf, int buf_len);

int awaitForwarderConnection(int fd);

int main() {

  int gateway_fd;
  int client_fd;

  gateway_fd = openGatewaySocket(GATEWAY_PORT);
  if (gateway_fd < 0) {
    perror("gateway socket connect");
    exit(EXIT_FAILURE);
  }
  client_fd = openGatewaySocket(CLIENT_PORT);
  if (gateway_fd < 0) {
    perror("client socket connect");
    exit(EXIT_FAILURE);
  }
  sleep(1);
  printf("Ready");
  fflush(stdout);

  uint8_t buf[MAX_MSG_LEN];
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
  while (1) {
    memset(buf, 0, sizeof(buf));
    int numRead = read(0, buf, MAX_MSG_LEN);
    if (numRead > 0) {
      printf("You said: %s", buf);

      if (strcmp((char*)buf, "exit\n") == 0) {
        break;
      }
      MQTT_SN_PUBLISH publish = {0};
      MqttSnPublishInit(&publish, buf, strlen((char*)buf));

      struct sockaddr_in to_address;
      to_address.sin_family = AF_INET;
      to_address.sin_port = htons(FORWARDER_CLIENT_NETWORK_PORT);
      inet_pton(AF_INET, FORWARDER_IP, &(to_address.sin_addr));
      socklen_t to_sockaddr_socklen = sizeof(to_address);
      if (sendto(client_fd, (uint8_t *) &publish, publish.length, 0,
                 (const struct sockaddr *) &to_address, to_sockaddr_socklen) != publish.length) {
        perror("MqttSnForwarder socket send");
        exit(EXIT_FAILURE);
      }
      fflush(stdout);
    }

    { // FAKE MQTT-SN Gateway ping-pong


      int timeout_ms = 100;
      struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
      if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
        interval.tv_sec = 0;
        interval.tv_usec = 100;
      }
      setsockopt(gateway_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &interval, sizeof(struct timeval));

      char gateway_recv_buf[MAX_MSG_LEN + FORWARDER_HEADER_LEN + sizeof(device_address)] = {0};
      struct sockaddr_in recv_sockaddr;
      memset(&recv_sockaddr, 0, sizeof(recv_sockaddr));
      socklen_t recv_sockaddr_socklen = sizeof(recv_sockaddr);

      int gateway_rec_buf_len = recvfrom(gateway_fd,
                                         gateway_recv_buf,
                                         MAX_MSG_LEN + FORWARDER_HEADER_LEN + sizeof(device_address),
                                         MSG_WAITALL,
                                         (struct sockaddr *) &recv_sockaddr,
                                         &recv_sockaddr_socklen);
      if (gateway_rec_buf_len < 0) {
        // TODO implement check for EAGAIN/EWOULDBLOCK then uncommend
        // perror("gateway socket recv");
        // exit(EXIT_FAILURE);
      }
      if (gateway_rec_buf_len > 0) {
        char recv_ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &(recv_sockaddr.sin_addr), recv_ip, INET_ADDRSTRLEN);

        //MQTT_SN_FORWARD_ENCAPSULATION *encapsulation = (MQTT_SN_FORWARD_ENCAPSULATION *) gateway_recv_buf;
        printf("Gateway - %s:%d - Length: %d, MsgType(0xFE): %02x, Ctrl: %d, MsgH: %s, Msg: %s \n",
               recv_ip, ntohs(recv_sockaddr.sin_port),
               gateway_recv_buf[0], gateway_recv_buf[1], gateway_recv_buf[3],
               &gateway_recv_buf[3 + sizeof(device_address)],
               &gateway_recv_buf[3 + sizeof(device_address) + PUBLISH_HEADER_LEN]);
        fflush(stdout);

        struct sockaddr_in to_address = recv_sockaddr;
        socklen_t to_address_socklen = sizeof(to_address);
        if (sendto(gateway_fd, (uint8_t *) &gateway_recv_buf, gateway_rec_buf_len, 0,
                   (const struct sockaddr *) &to_address, to_address_socklen) != gateway_rec_buf_len) {
          perror("Gateway socket send");
          exit(EXIT_FAILURE);
        }
      }
    } // FAKE MQTT-SN Gateway ping-pong


    { // FAKE MQTT-SN Client receive

      int timeout_ms = 100;
      struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
      if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
        interval.tv_sec = 0;
        interval.tv_usec = 100;
      }
      setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &interval, sizeof(struct timeval));

      char client_recv_buf[MAX_MSG_LEN] = {0};
      struct sockaddr_in recv_sockaddr;
      memset(&recv_sockaddr, 0, sizeof(recv_sockaddr));
      socklen_t recv_sockaddr_socklen = sizeof(recv_sockaddr);

      int client_rec_buf_len = recvfrom(client_fd,
                                        client_recv_buf,
                                        MAX_MSG_LEN,
                                        MSG_WAITALL,
                                        (struct sockaddr *) &recv_sockaddr,
                                        &recv_sockaddr_socklen);

      if (client_rec_buf_len < 0) {
        // TODO implement check for EAGAIN/EWOULDBLOCK then uncommend
        // perror("MqttSnForwarder socket recv");
        // exit(EXIT_FAILURE);
      }
      if (client_rec_buf_len > 0) {
        if (client_rec_buf_len > PUBLISH_HEADER_LEN) {
          printMqttSnPublish("Forwarder", (MQTT_SN_PUBLISH *) client_recv_buf);
        } else {
          printf("Forwarder - Unknown - Msg: %s ", client_recv_buf);
          fflush(stdout);
        }
      }
    } // FAKE MQTT-SN Client receive

  }
  return 0;
}

int awaitForwarderConnection(int fd) {
  printf("no connection to establish - return immediately");
  return -1;
}

int openGatewaySocket(int port) {
  int master_socket;
  int opt = true;
  struct sockaddr_in address;

  // create master socket
  if ((master_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // bind the socket to the localhost clientTcpNetwork->port
  if (bind(master_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  printf("Listener on port %d \n", port);

  return master_socket;
}

int MqttSnPublishInit(MQTT_SN_PUBLISH *publish, uint8_t *data, uint16_t length) {
  publish->length = (uint8_t) (length + PUBLISH_HEADER_LEN);
  publish->msg_type = 0x0C;
  publish->flags = 0;
  publish->flags = (uint8_t) (publish->flags | 0x62); // 0b01100010
  publish->topic_id = 1;
  publish->msg_id = 0;
  memcpy(publish->data, data, length);
  return 0;
}

int printMqttSnPublish(char *from, MQTT_SN_PUBLISH *publish) {
  printf("%s - Publish - Length: %d, TopicId, %d, MsgId: %d, Data: %s", from, publish->length, publish->topic_id,
         publish->msg_id, publish->data);
  fflush(stdout);
  return 0;
}
