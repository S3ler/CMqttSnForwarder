//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "MqttSnFixedSizeRingBuffer.h"
#include "MqttSnForwarderLogging.h"

typedef struct MqttSnGatewayNetworkInterface {

  int status;

  int (*gateway_network_receive)(struct MqttSnGatewayNetworkInterface *, MqttSnFixedSizeRingBuffer *, int, void *context);

  int (*gateway_network_send)(struct MqttSnGatewayNetworkInterface *, MqttSnFixedSizeRingBuffer *, int, void *context);

  int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *context);

  int (*gateway_network_connect)(struct MqttSnGatewayNetworkInterface *, void *context);

  void (*gateway_network_disconnect)(struct MqttSnGatewayNetworkInterface *, void *context);

  device_address *gateway_network_address;

  device_address *forwarder_network_address;

#ifdef WITH_LOGGING
  MqttSnLogger *logger;
#endif

} MqttSnGatewayNetworkInterface;

int GatewayNetworkInit(MqttSnGatewayNetworkInterface *n,
                       device_address *gateway_network_address,
                       device_address *forwarder_network_address,
                       void *context,
                       int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *context));

int GatewayNetworkConnect(MqttSnGatewayNetworkInterface *, void *context);

void GatewayNetworkDisconnect(MqttSnGatewayNetworkInterface *, void *context);

int GatewayNetworkSend(struct MqttSnGatewayNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int timeout_ms,
                       void *context);

int GatewayNetworkReceive(struct MqttSnGatewayNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int timeout_ms,
                          void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
