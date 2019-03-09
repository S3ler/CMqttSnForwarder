//
// Created by bele on 28.01.19.
//

#include "MqttSnGatewayNetworkInterface.h"

int GatewayNetworkInit(MqttSnGatewayNetworkInterface *n,
                       device_address *gateway_network_address,
                       device_address *mqtt_sn_gateway_network_address,
                       void *context,
                       int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *context)) {
  n->status = 0;
  n->gateway_network_address = gateway_network_address;
  n->mqtt_sn_gateway_network_address = mqtt_sn_gateway_network_address;
  n->gateway_network_init = gateway_network_init;
  if (n->gateway_network_init(n, context) == 0) {
    n->status = 0;
  } else {
    n->status = -1;
  }
  return n->status;
}

int GatewayNetworkConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  if (n->status == 0 &&
      n->gateway_network_connect != 0 &&
      n->gateway_network_connect(n, context) == 0) {
    n->status = 1;
    return 0;
  } else {
    n->status = -1;
  }
  return n->status;
}

void GatewayNetworkDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  if (n->gateway_network_disconnect != 0) {
    n->gateway_network_disconnect(n, context);
  }
  n->status = -1;
}