//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDER_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "network/MqttSnGatewayNetworkInterface.h"
#include "network/MqttSnClientNetworkInterface.h"
#include <stdint.h>

typedef struct MqttSnForwarder_ {
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
                        MqttSnLogger *logger,
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

int AddMqttSnForwardingHeader(MqttSnMessageData *clientMessageData,
                              MqttSnMessageData *gatewayMessageData,
                              const device_address *mqtt_sn_gateway_network_address,
                              const device_address *client_network_broadcast_address);

int RemoveMqttSnForwardingHeader(MqttSnMessageData *gatewayMessageData,
                                 MqttSnMessageData *clientMessageData,
                                 const device_address *client_network_broadcast_address);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNFORWARDER_H
