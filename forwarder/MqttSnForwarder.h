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
#include "MqttSnForwarderLogging.h"

typedef struct MqttSnForwarder {
  MqttSnClientNetworkInterface clientNetwork;
  MqttSnGatewayNetworkInterface gatewayNetwork;
  MqttSnFixedSizeRingBuffer clientNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer clientNetworkSendBuffer;
  MqttSnFixedSizeRingBuffer gatewayNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer gatewayNetworkSendBuffer;
  int clientNetworkSendTimeout;
  int clientNetworkReceiveTimeout;
  int gatewayNetworkSendTimeout;
  int gatewayNetworkReceiveTimeout;

  void *clientNetworkContext;
  void *gatewayNetworkContext;
#ifdef WITH_LOGGING
  MqttSnLogger logger;
#endif
} MqttSnForwarder;

void sendBufferedMessagesToGateway(MqttSnForwarder *forwarder);

void sendBufferedMessagesToClients(MqttSnForwarder *forwarder);

int MqttSnForwarderInit(MqttSnForwarder *mqttSnForwarder,
                        log_level_t log_level,
                        void *clientNetworkContext,
                        void *gatewayNetworkContext);

int MqttSnForwarderLoop(MqttSnForwarder *);

void MqttSnForwarderDeinit(MqttSnForwarder *);

int AddForwardingHeaderToClientMessages(MqttSnForwarder *forwarder,
                                        MqttSnMessageData *clientMessageData,
                                        MqttSnMessageData *gatewayMessageData);

int RemoveForwardingHeaderFromGatewayMessages(MqttSnForwarder *forwarder,
                                              MqttSnMessageData *gatewayMessageData,
                                              MqttSnMessageData *clientMessageData);

int AddMqttSnForwardingHeader(MqttSnMessageData *clientMessageData, MqttSnMessageData *gatewayMessageData);

int RemoveMqttSnForwardingHeader(MqttSnMessageData *gatewayMessageData, MqttSnMessageData *clientMessageData);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDER_H
