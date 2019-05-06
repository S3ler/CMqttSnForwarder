//
// Created by bele on 28.01.19.
//

#include "MqttSnGatewayNetworkInterface.h"
#include <assert.h>
#include <stdio.h>

int GatewayNetworkInit(MqttSnGatewayNetworkInterface *n,
                       device_address *mqtt_sn_gateway_address,
                       device_address *gateway_network_address,
                       device_address *gateway_network_broadcast_address,
                       void *context,
                       int (*gateway_network_init)(MqttSnGatewayNetworkInterface *, void *)) {
  assert(gateway_network_init != NULL);

  n->status = 0;
  n->gateway_network_address = gateway_network_address;
  n->mqtt_sn_gateway_address = mqtt_sn_gateway_address;
  n->gateway_network_broadcast_address = gateway_network_broadcast_address,
  n->gateway_network_init = gateway_network_init;
  if (n->gateway_network_init(n, context) == 0) {
    assert(n->gateway_network_init != NULL);
    assert(n->gateway_network_connect != NULL);
    assert(n->gateway_network_disconnect != NULL);
    assert(n->gateway_network_send != NULL);
    assert(n->gateway_network_receive != NULL);
    n->status = 1;
  } else {
    n->status = -1;
  }

  if (n->status != 1) {
    return -1;
  }
  return 0;
}

int GatewayNetworkConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  if (n->status != 1) {
    return -1;
  }
  assert(n->gateway_network_connect != NULL);
  if (n->status == 1 &&
      n->gateway_network_connect != NULL &&
      n->gateway_network_connect(n, context) == 0) {
    n->status = 2;
  } else {
    n->status = -1;
  }

  if (n->status != 2) {
    return -1;
  }
  return 0;
}

void GatewayNetworkDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  if (n->gateway_network_disconnect != NULL) {
    n->gateway_network_disconnect(n, context);
  }
  n->status = -1;
}

int GatewayNetworkSend(MqttSnGatewayNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int timeout_ms,
                       void *context) {
  if (n->status != 2) {
    return -1;
  }
  assert(n->gateway_network_send != NULL);
  if (n->gateway_network_send(n, sendBuffer, timeout_ms, context)) {
    n->status = -1;
  }

  if (n->status != 2) {
    return -1;
  }
  return 0;
}

int GatewayNetworkReceive(MqttSnGatewayNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int timeout_ms,
                          void *context) {
  if (n->status != 2) {
    return -1;
  }
  assert(n->gateway_network_receive != NULL);
  if (n->gateway_network_receive(n, receiveBuffer, timeout_ms, context)) {
    n->status = -1;
  }

  if (n->status != 2) {
    return -1;
  }
  return 0;
}

