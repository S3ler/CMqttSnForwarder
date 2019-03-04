//
// Created by bele on 28.01.19.
//

#include "MqttSnClientNetworkInterface.h"

int ClientNetworkInit(MqttSnClientNetworkInterface *n,
                      device_address *client_network_address,
                      void *context,
                      int (*client_network_init)(MqttSnClientNetworkInterface *, void *)) {
  n->client_network_address = client_network_address;
  n->client_network_init = client_network_init;
  if (n->client_network_init(n, context) == 0) {
    n->status = 0;
  } else {
    n->status = -1;
  }
  return n->status;
}

int ClientNetworkConnect(MqttSnClientNetworkInterface *n, void *context) {
  if (n->status == 0 &&
      n->client_network_connect != 0 &&
      n->client_network_connect(n, context) == 0) {
    n->status = 1;
    return 0;
  } else {
    n->status = -1;
  }
  return n->status;
}

void ClientNetworkDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  if (n->client_network_disconnect != 0) {
    n->client_network_disconnect(n, context);
  }
  n->status = -1;
}
