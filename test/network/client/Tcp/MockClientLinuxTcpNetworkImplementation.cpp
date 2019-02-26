//
// Created by bele on 06.02.19.
//

#include <zconf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
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
  if(getaddrinfo(ipAsString, NULL, &hints, &result)){
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
  // TODO find out own network ip and other port, then set it in the MockClient
  return true;
}

void MockClientLinuxTcpNetworkImplementation::disconnectNetwork() {
  close(forwarder_socket_fd);
  forwarder_socket_fd = -1;
}

void MockClientLinuxTcpNetworkImplementation::loopNetwork() {

}
void MockClientLinuxTcpNetworkImplementation::setNetworkAddress(device_address *address) {
  this->address = address;
}
