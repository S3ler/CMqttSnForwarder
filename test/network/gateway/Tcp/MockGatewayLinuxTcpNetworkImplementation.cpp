//
// Created by bele on 03.02.19.
//

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "MockGatewayLinuxTcpNetworkImplementation.h"

void MockGatewayLinuxTcpNetworkImplementation::setMockGateway(MockGateway *mockGateway) {
  this->mockGateway = mockGateway;
}
bool MockGatewayLinuxTcpNetworkImplementation::isNetworkConnected() {
  return !isNetworkDisconnected();
}
bool MockGatewayLinuxTcpNetworkImplementation::isNetworkDisconnected() {
  // check if someone is connected
  return master_socket_fd == -1;
}
ssize_t MockGatewayLinuxTcpNetworkImplementation::sendNetwork(const uint8_t *buf, uint8_t len) {
  if (client_socket_fd == -1) {
    return 0;
  }
  if (send(client_socket_fd, buf, len, 0) != len) {
    close(client_socket_fd);
    client_socket_fd = -1;
  }
  return len;
}
bool MockGatewayLinuxTcpNetworkImplementation::connectNetwork(device_address *address) {
  uint16_t port = address->bytes[4] << 8;
  port += address->bytes[5] << 0;

  // create master socket
  if ((master_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    return false;
    exit(EXIT_FAILURE);
  }

  int opt = true;
  if (setsockopt(master_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    return false;
    exit(EXIT_FAILURE);
  }

  // type of socket created
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(port);

  // bind the socket to localhost
  if (bind(master_socket_fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    perror("bind failed");
    return false;
    exit(EXIT_FAILURE);
  }

  if (listen(master_socket_fd, 3) < 0) {
    perror("listen");
    return false;
    exit(EXIT_FAILURE);
  }
  return true;
}

void MockGatewayLinuxTcpNetworkImplementation::disconnectNetwork() {
  close(master_socket_fd);
  master_socket_fd = -1;
  close(client_socket_fd);
  client_socket_fd = -1;
}

void MockGatewayLinuxTcpNetworkImplementation::loopNetwork() {
  if (client_socket_fd == -1) {
    // client not connected yet
    // await client connection
    // prepare timeout interval
    int timeout_ms = 100;
    struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
    if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
      interval.tv_sec = 0;
      interval.tv_usec = 100;
    }


    // init master_socket readset
    fd_set readfds;
    FD_ZERO(&readfds);

    // add master socket to set
    FD_SET(master_socket_fd, &readfds);
    int max_sd = master_socket_fd;

    int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
    // int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR)) {
      printf("select error");
      exit(EXIT_FAILURE);
    }
    if (activity == 0) {
      // no connection
      return;
    }

    if (FD_ISSET(master_socket_fd, &readfds)) {
      struct sockaddr_in sockaddr;
      int addrlen = sizeof(sockaddr);
      if ((client_socket_fd = accept(master_socket_fd, (struct sockaddr *) &sockaddr,
                                     (socklen_t *) &addrlen)) < 0) {
        perror("new_socket");
        exit(EXIT_FAILURE);
      }
    }
    ///
  } else {
    ///
    int timeout_ms = 100;
    struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
    if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
      interval.tv_sec = 0;
      interval.tv_usec = 100;
    }

    // init client_socket to readset
    fd_set readfds;
    FD_ZERO(&readfds);

    // add master socket to set
    FD_SET(client_socket_fd, &readfds);
    int max_sd = client_socket_fd;

    int activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
    // int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR)) {
      printf("select error");
      exit(EXIT_FAILURE);
    }
    if (activity == 0) {
      // no connection
      return;
    }

    int sd = client_socket_fd;
    if (FD_ISSET(sd, &readfds)) {
      // Check if it was for closing, and also read the incoming message
      char buffer[MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE];
      int buffer_length = MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE;
      int valread;
      if ((valread = read(sd, buffer, buffer_length)) == 0) {
        // client_socket broken
        close(sd);
        client_socket_fd = -1;
      } else {
        mockGateway->receive(reinterpret_cast<uint8_t *>(buffer), valread);
      }
    }

  }
}
