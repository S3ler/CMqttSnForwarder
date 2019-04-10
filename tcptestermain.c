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
#include <MqttSnMessageParser.h>
#include "forwarder/global_defines.h"

#define FORWARDER_CLIENT_NETWORK_PORT 9999
#define GATEWAY_PORT 22222
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

int is_fd_set(int fd);

int main() {
  // TODO set sockets reusable
  // TODO implement accepting connection
  int gateway_fd, forwarder_fd, connected_forwarder_fd;
  gateway_fd = openGatewaySocket(GATEWAY_PORT);
  puts("GATEWAY_PORT");
  fflush(stdout);

  if (gateway_fd < 0) {
    perror("gateway socket connect");
    exit(EXIT_FAILURE);
  }
  puts("Waiting for Forwarder to connect");
  connected_forwarder_fd = awaitForwarderConnection(gateway_fd);
  puts("Forwarder Connected");
  fflush(stdout);

  sleep(1);
  forwarder_fd = connectForwarder("127.0.0.1", FORWARDER_CLIENT_NETWORK_PORT);
  if (forwarder_fd < 0) {
    perror("MqttSnForwarder socket connect");
    exit(EXIT_FAILURE);
  }

  printf("Ready");
  fflush(stdout);
  uint8_t buf[MAX_MSG_LEN];
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
  while (1) {
    memset(buf, 0, sizeof(buf));
    int numRead = read(0, buf, MAX_MSG_LEN);
    if (numRead > 0) {
      printf("You said: %s", buf);

      if (strcmp((char *) buf, "exit\n") == 0) {
        break;
      }
      MQTT_SN_PUBLISH publish = {0};
      MqttSnPublishInit(&publish, buf, strlen((char *) buf));
      uint8_t b[255];
      memcpy(b, &publish, publish.length);
      if (send_fd(forwarder_fd, (uint8_t *) &publish, publish.length) != publish.length) {
        perror("MqttSnForwarder socket send");
        exit(EXIT_FAILURE);
      }
      fflush(stdout);
    }

    { // FAKE MQTT-SN Gateway ping-pong
      if (is_fd_set(connected_forwarder_fd)) {
        uint8_t  gateway_rec_buf[MAX_MSG_LEN + FORWARDER_HEADER_LEN + sizeof(device_address)] = {0};
        int gateway_rec_buf_len = recv_fd(connected_forwarder_fd, gateway_rec_buf,
                                          MAX_MSG_LEN + FORWARDER_HEADER_LEN + sizeof(device_address));
        if (gateway_rec_buf_len < 0) {
          perror("gateway socket recv");
          exit(EXIT_FAILURE);
        }
        if (gateway_rec_buf_len > 0) {
          // MQTT_SN_FORWARD_ENCAPSULATION *encapsulation = (MQTT_SN_FORWARD_ENCAPSULATION *) gateway_rec_buf;
          printf("Gateway - Length: %d, MsgType(0xFE): %02x, Ctrl: %d, MsgH: %s, Msg: %s \n",
                 gateway_rec_buf[0], gateway_rec_buf[1], gateway_rec_buf[3],
                 &gateway_rec_buf[3 + sizeof(device_address)],
                 &gateway_rec_buf[3 + sizeof(device_address) + PUBLISH_HEADER_LEN]);
          fflush(stdout);
        }
        if (send_fd(connected_forwarder_fd, gateway_rec_buf, gateway_rec_buf_len) != gateway_rec_buf_len) {
          perror("gateway socket send");
          exit(EXIT_FAILURE);
        }
        fflush(stdout);
      }
    } // FAKE MQTT-SN Gateway ping-pong

    { // FAKE MQTT-SN Client receive
      if (is_fd_set(forwarder_fd)) {
        uint8_t forwarder_rec_buf[MAX_MSG_LEN] = {0};
        int forwarder_rec_buf_len = recv_fd(forwarder_fd, forwarder_rec_buf, MAX_MSG_LEN);
        if (forwarder_rec_buf_len < 0) {
          perror("MqttSnForwarder socket recv");
          exit(EXIT_FAILURE);
        }
        if (forwarder_rec_buf_len > 0) {
          if (forwarder_rec_buf_len > PUBLISH_HEADER_LEN) {
            printMqttSnPublish("Forwarder", (MQTT_SN_PUBLISH *) forwarder_rec_buf);
          } else {
            printf("Forwarder - Unknown - Msg: %s ", forwarder_rec_buf);
          }
        }
        fflush(stdout);
      }
    } // FAKE MQTT-SN Client receive
  }
  return 0;
}

int is_fd_set(int fd) {
  int timeout_ms = 1000;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  fd_set readfds;
  FD_ZERO(&readfds);

  FD_SET(fd, &readfds);
  int max_sd = fd;

  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);

  if ((activity < 0) && (errno != EINTR)) {
    return -1;
  }
  if (activity == 0) {
    return 0;
  }
  if (FD_ISSET(fd, &readfds) == 0) {
    return 0;
  }
  return 1;
}

int awaitForwarderConnection(int fd) {
  // TODO implement accepting connection
  int new_socket;
  int addrlen;
  struct sockaddr_in address;
  addrlen = sizeof(address);
  if ((new_socket = accept(fd, (struct sockaddr *) &address,
                           (socklen_t *) &addrlen)) < 0) {
    perror("accept");
    fprintf(stderr, "socket() failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // inform user of the socket number - used in gateway_send and client_receive commands
  printf("Forwarder conmected, socket fd is %d, ip is: %s, port: %d\n",
         new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

  return new_socket;
}

int send_fd(int fd, uint8_t *buf, int buf_len) {
  int timeout_ms = 100;
  struct timeval tv;
  tv.tv_sec = 0;  /* 30 Secs Timeout */
  tv.tv_usec = timeout_ms * 1000;  // Not Init'ing this can cause strange errors

  uint8_t *buffer = buf;
  uint16_t len = buf_len;

  setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv, sizeof(struct timeval));

  int rc = write(fd, buffer, len);

  if (rc != len) {
    return -1;
  }
  return rc;
}

int recv_fd(int fd, uint8_t *buf, int buf_len) {
  int timeout_ms = 100;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &interval, sizeof(struct timeval));

  uint8_t buffer[MAX_MSG_LEN + FORWARDER_HEADER_LEN + sizeof(device_address)] = {0};

  int bytes = 0;
  while (bytes < buf_len) {
    int rc = recv(fd, &buffer[bytes], (size_t) (buf_len - bytes), 0);
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

  memcpy(buf, &buffer, bytes);
  return bytes;
}

int connectForwarder(char *ip, int port) {
  int fd;
  char *addr = ip;

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
    fd = socket(family, type, 0);
    if (fd != -1) {
      rc = connect(fd, (struct sockaddr *) &address, sizeof(address));
      if (rc == 0) {
        return fd;
      }
      rc = -1;
    } else {
      rc = -1;
      return -1;
    }
  }

  return rc;

}

int openGatewaySocket(int port) {
  int master_socket;
  int opt = true;
  struct sockaddr_in address;

  // create master socket
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
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

  // try to specify maximum of 5 pending connections for the master socket
  if (listen(master_socket, 5) < 0) {
    // TODO find out more about the functionality here
    perror("listen");
    exit(EXIT_FAILURE);
  }

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
