//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MqttSnForwarderLogging.h"
#include "MqttSnFixedSizeRingBuffer.h"
#include <stdint.h>

typedef struct MqttSnGatewayNetworkInterface_ {

  int status;

  int (*gateway_network_receive)(struct MqttSnGatewayNetworkInterface_ *,
                                 MqttSnFixedSizeRingBuffer *,
                                 int32_t,
                                 void *context);

  int (*gateway_network_send)(struct MqttSnGatewayNetworkInterface_ *,
                              MqttSnFixedSizeRingBuffer *,
                              int32_t,
                              void *context);

  int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface_ *, void *context);

  int (*gateway_network_connect)(struct MqttSnGatewayNetworkInterface_ *, void *context);

  void (*gateway_network_disconnect)(struct MqttSnGatewayNetworkInterface_ *, void *context);

  device_address *gateway_network_address;

  device_address *mqtt_sn_gateway_address;

  device_address *gateway_network_broadcast_address;

#ifdef WITH_LOGGING
  MqttSnLogger *logger;
#endif

} MqttSnGatewayNetworkInterface;

int GatewayNetworkInit(MqttSnGatewayNetworkInterface *n,
                       device_address *mqtt_sn_gateway_address,
                       device_address *gateway_network_address,
                       device_address *gateway_network_broadcast_address,
                       void *context,
                       int (*gateway_network_init)(MqttSnGatewayNetworkInterface *, void *));

int GatewayNetworkConnect(MqttSnGatewayNetworkInterface *, void *context);

void GatewayNetworkDisconnect(MqttSnGatewayNetworkInterface *, void *context);

int GatewayNetworkSend(MqttSnGatewayNetworkInterface *n,
                       MqttSnFixedSizeRingBuffer *sendBuffer,
                       int timeout_ms,
                       void *context);

int GatewayNetworkReceive(MqttSnGatewayNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          int timeout_ms,
                          void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
