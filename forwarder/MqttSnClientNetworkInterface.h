//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H

#include <stdint.h>
#include "MqttSnFixedSizeRingBuffer.h"

typedef struct MqttSnClientNetworkInterface {

    int status;

    int (*client_receive)(struct MqttSnClientNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context);

    int (*client_send)(struct MqttSnClientNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context);

    int (*client_network_init)(struct MqttSnClientNetworkInterface*, void* context);

    int (*client_network_connect)(struct MqttSnClientNetworkInterface*, void* context);

    void (*client_network_disconnect)(struct MqttSnClientNetworkInterface*, void* context);

} MqttSnClientNetworkInterface;

int ClientNetworkInit(MqttSnClientNetworkInterface *, void* context,
                      int (*client_network_init)(struct MqttSnClientNetworkInterface*, void* context));

int ClientNetworkConnect(MqttSnClientNetworkInterface *, void* context);

void ClientNetworkDisconnect(MqttSnClientNetworkInterface *, void* context);

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
