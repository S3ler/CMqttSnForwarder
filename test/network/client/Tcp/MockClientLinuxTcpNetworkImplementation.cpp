//
// Created by bele on 06.02.19.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "MockClientLinuxTcpNetworkImplementation.h"

void MockClientLinuxTcpNetworkImplementation::setMockClient(MockClient *client) {
  this->mockClient = client;
}

bool MockClientLinuxTcpNetworkImplementation::isNetworkConnected() {
  return !isNetworkDisconnected();
}

bool MockClientLinuxTcpNetworkImplementation::isNetworkDisconnected() {
  return forwarder_socket_fd == -1;
}

int MockClientLinuxTcpNetworkImplementation::sendToNetwork(device_address *to, const uint8_t *buf, uint8_t len) {
  if (forwarder_socket_fd == -1) {
    return 0;
  }
  // check if Tcp socket is connected, if not then do it!
  if (send(forwarder_socket_fd, buf, len, 0) != len) {
    close(forwarder_socket_fd);
    forwarder_socket_fd = -1;
  }
  return len;
}

bool MockClientLinuxTcpNetworkImplementation::connectNetwork(device_address *address) {

  uint16_t port = address->bytes[4] << 8;
  port += address->bytes[5] << 0;

  // create forwarder_socket
  if ((forwarder_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    return false;
    exit(EXIT_FAILURE);
  }

  int opt = true;
  if (setsockopt(forwarder_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    return false;
    exit(EXIT_FAILURE);
  }

  // type of socket created
  // sockaddrIn.sin_addr =
  char ipAsString[255] = {0};
  sprintf(ipAsString,
          "%d.%d.%d.%d",
          address->bytes[0], address->bytes[1], address->bytes[2], address->bytes[3]);

  addrinfo *result = NULL;
  addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};
  if (getaddrinfo(ipAsString, NULL, &hints, &result)) {
    perror("getaddrinfo");
    return false;
  }

  struct addrinfo *res = result;

  /* prefer ip4 addresses */
  while (res) {
    if (res->ai_family == AF_INET) {
      result = res;
      break;
    }
    res = res->ai_next;
  }

  if (result->ai_family != AF_INET) {
    perror("Not AF_INET");
    return false;
  }

  sockaddr_in sockaddrIn;
  sockaddrIn.sin_addr = ((struct sockaddr_in *) (result->ai_addr))->sin_addr;
  sockaddrIn.sin_family = AF_INET;
  sockaddrIn.sin_addr.s_addr = INADDR_ANY;
  sockaddrIn.sin_port = htons(port);

  freeaddrinfo(result);

  if (connect(forwarder_socket_fd, (struct sockaddr *) &sockaddrIn, sizeof(sockaddr_in)) < 0) {
    perror("connect");
    return false;
  }
  return true;
}

void MockClientLinuxTcpNetworkImplementation::disconnectNetwork() {
  close(forwarder_socket_fd);
  forwarder_socket_fd = -1;
}

int MockClientLinuxTcpNetworkImplementation::loopNetwork(MockClientNetworkReceiver *pReceiver) {
  uint32_t timeout_ms = 100;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }
  // init read fd set
  // new
  int max_sd;
  fd_set readfds;
  FD_ZERO(&readfds);

  FD_SET(forwarder_socket_fd, &readfds);
  max_sd = forwarder_socket_fd;

  int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);

  if ((activity < 0) && (errno != EINTR)) {
    //printf("loopNetwork select error");
    return 0;
  }
  if (activity == 0) {
    return 0;
  }
  if (FD_ISSET(forwarder_socket_fd, &readfds)) {
    int sd = forwarder_socket_fd;
    uint8_t buffer[MAX_MESSAGE_LENGTH + 10];
    int buffer_length = MAX_MESSAGE_LENGTH + 10;
    int valread;
    if ((valread = read(sd, buffer, buffer_length)) <= 0) {
      return -1;
    } else {
      if (valread > MAX_MESSAGE_LENGTH) {
        perror("MAX_MESSAGE_LENGTH");
        return 0;
      }
      // convert IP-Address to device_address
      device_address sender_address;
      struct sockaddr_in address;
      socklen_t addrlen = sizeof(struct sockaddr_in);
      getpeername(forwarder_socket_fd, (struct sockaddr *) &address, &addrlen);
      unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;
      sender_address.bytes[0] = ip[0];
      sender_address.bytes[1] = ip[1];
      sender_address.bytes[2] = ip[2];
      sender_address.bytes[3] = ip[3];
      uint16_t port_as_number = (uint16_t) htons(address.sin_port);
      sender_address.bytes[4] = (uint8_t) (port_as_number >> 8);
      sender_address.bytes[5] = (uint8_t) (port_as_number >> 0);
      pReceiver->receive_any_message(&sender_address, buffer, valread);
      return 0;
    }
  }

  return 0;
}
void MockClientLinuxTcpNetworkImplementation::setNetworkAddress(device_address *address) {
  this->address = address;
}
