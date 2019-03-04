//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDER_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDER_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "MqttSnGatewayNetworkInterface.h"
#include "MqttSnFixedSizeRingBuffer.h"
#include "MqttSnClientNetworkInterface.h"

#define CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#define CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT 5000

#define GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT 1000
#define GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT 1000

typedef struct MqttSnForwarder {
  MqttSnClientNetworkInterface clientNetwork;
  MqttSnGatewayNetworkInterface gatewayNetwork;
  MqttSnFixedSizeRingBuffer clientNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer clientNetworkSendBuffer;
  MqttSnFixedSizeRingBuffer gatewayNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer gatewayNetworkSendBuffer;
  void *clientNetworkContext;
  void *gatewayNetworkContext;
} MqttSnForwarder;

void sendBufferedMessagesToGateway(MqttSnForwarder *forwarder);

void sendBufferedMessagesToClients(MqttSnForwarder *forwarder);

int MqttSnForwarderStart(MqttSnForwarder *mqttSnForwarder,
                         void *clientNetworkContext,
                         void *gatewayNetworkContext,
                         volatile sig_atomic_t *keep_running);

int MqttSnForwarderInit(MqttSnForwarder *mqttSnForwarder,
                        void *clientNetworkContext,
                        void *gatewayNetworkContext);

int MqttSnForwarderLoop(MqttSnForwarder *);

void MqttSnForwarderDeinit(MqttSnForwarder *);

int AddForwardingHeaderToClientMessages(MqttSnForwarder *forwarder);

int RemoveForwardingHeaderFromGatewayMessages(MqttSnForwarder *forwarder);

int AddMqttSnForwardingHeader(MqttSnMessageData *receiveMessageData, MqttSnMessageData *sendMessageData);

int RemoveMqttSnForwardingHeader(MqttSnMessageData *receiveMessageData, MqttSnMessageData *sendMessageData);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDER_H
