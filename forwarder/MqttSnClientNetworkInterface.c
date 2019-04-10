//
// Created by bele on 28.01.19.
//

#include <assert.h>
#include "MqttSnClientNetworkInterface.h"

int ClientNetworkInit(MqttSnClientNetworkInterface *n,
                      device_address *mqtt_sn_gateway_device_address,
                      device_address *client_network_address,
                      void *context,
                      int (*client_network_init)(MqttSnClientNetworkInterface *, void *)) {
  assert(client_network_init != NULL);

  n->client_network_address = client_network_address;
  n->mqtt_sn_gateway_address = mqtt_sn_gateway_device_address;
  n->client_network_init = client_network_init;
  if (n->client_network_init(n, context) == 0) {
    assert(n->client_network_init != NULL);
    assert(n->client_network_connect != NULL);
    assert(n->client_network_disconnect != NULL);
    assert(n->client_network_send != NULL);
    assert(n->client_network_receive != NULL);
    n->status = 0;
  } else {
    n->status = -1;
  }

  return n->status;
}

int ClientNetworkConnect(MqttSnClientNetworkInterface *n, void *context) {
  if (n->status < 0) {
    return -1;
  }
  assert(n->client_network_connect != NULL);
  if (n->status == 0 &&
      n->client_network_connect(n, context) == 0) {
    n->status = 1;
    return 0;
  } else {
    n->status = -1;
  }
  return n->status;
}

void ClientNetworkDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  assert(n->client_network_disconnect != NULL);
  n->client_network_disconnect(n, context);
  n->status = -1;
}

int ClientNetworkSend(MqttSnClientNetworkInterface *n,
                      MqttSnFixedSizeRingBuffer *sendBuffer,
                      int timeout_ms,
                      void *context) {
  if (n->status <= 0) {
    return -1;
  }
  assert(n->client_network_send != NULL);
  if (n->client_network_send(n, sendBuffer, timeout_ms, context)) {
    n->status = -1;
  }
  return n->status != 1;
}
int ClientNetworkReceive(MqttSnClientNetworkInterface *n,
                         MqttSnFixedSizeRingBuffer *receiveBuffer,
                         int timeout_ms,
                         void *context) {
  if (n->status <= 0) {
    return -1;
  }
  assert(n->client_network_receive != NULL);
  if (n->client_network_receive(n, receiveBuffer, timeout_ms, context)) {
    n->status = -1;
  }
  return n->status != 1;
}
