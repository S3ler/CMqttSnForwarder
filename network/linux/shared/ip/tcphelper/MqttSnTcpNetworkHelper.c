//
// Created by SomeDude on 07.05.2019.
//

#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "MqttSnTcpNetworkHelper.h"

int connect_to_tcp_host(uint32_t ip, uint16_t port) {
  int socket_fd;
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(ip);
  address.sin_port = htons(port);
  socklen_t address_len = sizeof(address);
  // create new tcp socket
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    close(socket_fd);
    return -1;
  }

  if (connect(socket_fd, (struct sockaddr *) &address, address_len) < 0) {
    close(socket_fd);
    return -1;
  }
  return socket_fd;
}

int is_tcp_message_received(int socket_fd, int32_t timeout_ms) {
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  fd_set readfds;
  FD_ZERO(&readfds);

  FD_SET(socket_fd, &readfds);
  int max_sd = socket_fd;
  int activity;
  if (timeout_ms == -1) {
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
  } else {
    activity = select(max_sd + 1, &readfds, NULL, NULL, &interval);
  }

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

int listen_on_tcp_socket(uint32_t ip, uint16_t port, uint32_t max_pending_connections) {

  int socket_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(struct sockaddr_in);

  // create master socket
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    close(socket_fd);
    return -1;
  }

  int option = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &option, sizeof(option)) < 0) {
    close(socket_fd);
    return -1;
  }

  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(ip);
  address.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr *) &address, addrlen) < 0) {
    close(socket_fd);
    return -1;
  }

  if (listen(socket_fd, max_pending_connections) < 0) {
    close(socket_fd);
    return -1;
  }

  return socket_fd;
}

int new_connection_or_is_message_received(int listen_socket_fd,
                                          const int client_socket_fds[],
                                          int client_socket_fds_len,
                                          int32_t timeout_ms,
                                          fd_set *read_fds,
                                          int multicast_socket_fd) {

  FD_ZERO(read_fds);


  // add listen socket fd to set
  FD_SET(listen_socket_fd, read_fds);
  int max_fd = listen_socket_fd;

  if (multicast_socket_fd > -1) {
    FD_SET(multicast_socket_fd, read_fds);
    if (multicast_socket_fd > max_fd) {
      max_fd = multicast_socket_fd;
    }
  }
  // add client socket fd to set
  for (int i = 0; i < client_socket_fds_len; i++) {
    // socket fd
    int socket_fd = client_socket_fds[i];

    // if valid socket fd then add to read list
    if (socket_fd > -1) {
      FD_SET(socket_fd, read_fds);
    }

    // determine highest fd
    if (socket_fd > max_fd) {
      max_fd = socket_fd;
    }
  }

  int activity = 0;
  if (timeout_ms < 0) {
    activity = select(max_fd + 1, read_fds, NULL, NULL, NULL);
  } else {
    struct timeval interval = {.tv_sec = timeout_ms / 1000, .tv_usec = (timeout_ms % 1000) * 1000};
    activity = select(max_fd + 1, read_fds, NULL, NULL, &interval);
  }

  if ((activity < 0) && (errno != EINTR)) {
    return -1;
  }
  if (activity == 0) {
    return 0;
  }

  int rv = 0;
  if (FD_ISSET(listen_socket_fd, read_fds)) {
    // listen only
    rv = 1;
  }
  if (multicast_socket_fd > -1) {
    if (FD_ISSET(multicast_socket_fd, read_fds)) {
      if (rv == 1) {
        // listen and multicast
        rv = 5;
      } else {
        // multicast only
        rv = 3;
      }
    }
  }
  // add client socket fd to set
  for (int i = 0; i < client_socket_fds_len; i++) {
    // socket descriptor
    int socket_descriptor = client_socket_fds[i];
    if (socket_descriptor < 0) {
      continue;
    }
    if (FD_ISSET(socket_descriptor, read_fds)) {
      if (rv == 1) {
        // listen and client only
        return 4;
      } else if (rv == 3) {
        // multicast and client only
        return 6;
      } else if (rv == 5) {
        // all
        return 7;
      } else {
        // client only
        return 2;
      }
    }
  }
  // returns listen only or listen and client only
  return rv;
}

