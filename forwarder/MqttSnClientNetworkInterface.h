//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "MqttSnFixedSizeRingBuffer.h"

typedef struct MqttSnClientNetworkInterface {

    int status;

    int (*client_network_receive)(struct MqttSnClientNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context);

    int (*client_network_send)(struct MqttSnClientNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context);

    int (*client_network_init)(struct MqttSnClientNetworkInterface*, void* context);

    int (*client_network_connect)(struct MqttSnClientNetworkInterface*, void* context);

    void (*client_network_disconnect)(struct MqttSnClientNetworkInterface*, void* context);

    device_address *client_network_address;

} MqttSnClientNetworkInterface;

int ClientNetworkInit(MqttSnClientNetworkInterface *,
                      device_address *client_network_address,
                      void *context,
                      int (*client_network_init)(MqttSnClientNetworkInterface *, void *));

int ClientNetworkConnect(MqttSnClientNetworkInterface *, void* context);

void ClientNetworkDisconnect(MqttSnClientNetworkInterface *, void* context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
