//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H

#include <stdint.h>
#include "MqttSnFixedSizeRingBuffer.h"

typedef struct MqttSnGatewayNetworkInterface {

    int status;

    int (*gateway_receive)(struct MqttSnGatewayNetworkInterface *, MqttSnFixedSizeRingBuffer *, uint32_t, void* context);

    int (*gateway_send)(struct MqttSnGatewayNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context);

    int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface*, void* context);

    int (*gateway_network_connect)(struct MqttSnGatewayNetworkInterface*, void* context);

    void (*gateway_network_disconnect)(struct MqttSnGatewayNetworkInterface*, void* context);


} MqttSnGatewayNetworkInterface;

int GatewayNetworkInit(MqttSnGatewayNetworkInterface *n, void *context,
                       int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface*, void* context));

int GatewayNetworkConnect(MqttSnGatewayNetworkInterface *, void *context);

void GatewayNetworkDisconnect(MqttSnGatewayNetworkInterface *, void *context);

#endif //CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
