//
// Created by bele on 26.01.19.
//

#include "MqttSnForwarder.h"
#include "ringbuffer/MqttSnFixedSizeRingBuffer.h"
#include "network/MqttSnClientNetworkInterface.h"
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <platform/platform_compatibility.h>
#include <logging/MqttSnLoggingInterface.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>

int MqttSnForwarderInit(MqttSnForwarder *mqttSnForwarder,
                        log_level_t log_level,
                        void *clientNetworkContext,
                        void *gatewayNetworkContext) {

#ifdef WITH_LOGGING
#if defined(Arduino_h) || defined(WITH_PLATFORMIO)
  if (MqttSnLoggerInit(&mqttSnForwarder->logger, log_level, arduino_serial_log_init) != 0) {
#else
  if (MqttSnLoggerInit(&mqttSnForwarder->logger, log_level, stdout_log_init) != 0) {
#endif
    MqttSnLoggerDeinit(&mqttSnForwarder->logger);
    return -1;
  }
  mqttSnForwarder->gatewayNetwork.logger = &mqttSnForwarder->logger;
  mqttSnForwarder->clientNetwork.logger = &mqttSnForwarder->logger;
#endif

  mqttSnForwarder->clientNetworkContext = clientNetworkContext;
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->clientNetworkReceiveBuffer);
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->clientNetworkSendBuffer);

  mqttSnForwarder->gatewayNetworkContext = gatewayNetworkContext;
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->gatewayNetworkReceiveBuffer);
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->gatewayNetworkSendBuffer);

  if (ClientNetworkConnect(&mqttSnForwarder->clientNetwork, mqttSnForwarder->clientNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

  if (GatewayNetworkConnect(&mqttSnForwarder->gatewayNetwork, mqttSnForwarder->gatewayNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

#ifdef WITH_LOGGING
  if (log_status(&mqttSnForwarder->logger)) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }
#endif

  return 0;
}

/**
 * disconnects the gateway and client network
 * @param forwarder
 */
void MqttSnForwarderDeinit(MqttSnForwarder *forwarder) {
  GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  GatewayNetworkDeinitialize(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  ClientNetworkDeinitialize(&forwarder->clientNetwork, forwarder->clientNetworkContext);
#ifdef WITH_LOGGING
  forwarder->logger.log_deinit(&forwarder->logger);
#endif
}

int MqttSnForwarderLoop(MqttSnForwarder *forwarder) {

  if (ClientNetworkReceive(&forwarder->clientNetwork,
                           &forwarder->clientNetworkReceiveBuffer,
                           forwarder->clientNetworkReceiveTimeout,
                           forwarder->clientNetworkContext)) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  }
#ifdef Arduino_h
  yield();
#endif
  {
    MqttSnMessageData clientMessageData = {0};
    MqttSnMessageData gatewayMessageData = {0};
    if (AddForwardingHeaderToClientMessages(forwarder, &clientMessageData, &gatewayMessageData) != 0) {
      ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
      GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    }
  }
#ifdef Arduino_h
  yield();
#endif
  if (GatewayNetworkSend(&forwarder->gatewayNetwork,
                         &forwarder->gatewayNetworkSendBuffer,
                         forwarder->gatewayNetworkSendTimeout,
                         forwarder->gatewayNetworkContext)) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }
#ifdef Arduino_h
  yield();
#endif
  if (GatewayNetworkReceive(&forwarder->gatewayNetwork,
                            &forwarder->gatewayNetworkReceiveBuffer,
                            forwarder->gatewayNetworkReceiveTimeout,
                            forwarder->gatewayNetworkContext)) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }
#ifdef Arduino_h
  yield();
#endif
  {
    MqttSnMessageData gatewayMessageData = {0};
    MqttSnMessageData clientMessageData = {0};
    if (RemoveForwardingHeaderFromGatewayMessages(forwarder, &gatewayMessageData, &clientMessageData) != 0) {
      ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
      GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    }
  }
#ifdef Arduino_h
  yield();
#endif
  if (ClientNetworkSend(&forwarder->clientNetwork,
                        &forwarder->clientNetworkSendBuffer,
                        forwarder->clientNetworkSendTimeout,
                        forwarder->clientNetworkContext)) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  }
#ifdef Arduino_h
  yield();
#endif
  if (forwarder->clientNetwork.status <= 0) {
    if (forwarder->gatewayNetwork.status <= 0) {
      // both networks down => end
      MqttSnForwarderDeinit(forwarder);
      return -1;
    }
    // only client network is down
    // send out buffered MqttSnMessageData to Gateway
    sendBufferedMessagesToGateway(forwarder);
    MqttSnForwarderDeinit(forwarder);
    return -1;
  }

  if (forwarder->gatewayNetwork.status <= 0) {
    if (forwarder->clientNetwork.status <= 0) {
      // both networks down => end
      MqttSnForwarderDeinit(forwarder);
      return -1;
    }
    // only gateway network is down
    // send out buffered MqttSnMessageData to Client
    sendBufferedMessagesToClients(forwarder);
    MqttSnForwarderDeinit(forwarder);
    return -1;
  }

  return 0;
}

void sendBufferedMessagesToClients(MqttSnForwarder *forwarder) {
  while (!isEmpty(&forwarder->gatewayNetworkReceiveBuffer) |
      !isEmpty(&forwarder->clientNetworkSendBuffer)) {
    {
      MqttSnMessageData gatewayMessageData = {0};
      MqttSnMessageData clientMessageData = {0};
      if (RemoveForwardingHeaderFromGatewayMessages(forwarder, &gatewayMessageData, &clientMessageData) < 0) {
        break;
      }
    }
    if (ClientNetworkSend(&forwarder->clientNetwork,
                          &forwarder->clientNetworkSendBuffer,
                          forwarder->clientNetworkSendTimeout,
                          forwarder->clientNetworkContext) < 0) {
      // TODO add Timeout or starvation detection
      break;
    }
  }
}

void sendBufferedMessagesToGateway(MqttSnForwarder *forwarder) {
  while (!isEmpty(&forwarder->clientNetworkReceiveBuffer) |
      !isEmpty(&forwarder->gatewayNetworkSendBuffer)) {
    {
      MqttSnMessageData gatewayMessageData = {0};
      MqttSnMessageData clientMessageData = {0};
      if (AddForwardingHeaderToClientMessages(forwarder, &clientMessageData, &gatewayMessageData) != 0) {
        break;
      }
    }
    if (GatewayNetworkSend(&forwarder->gatewayNetwork,
                           &forwarder->gatewayNetworkSendBuffer,
                           forwarder->gatewayNetworkSendTimeout,
                           forwarder->gatewayNetworkContext) < 0) {
      // TODO add Timeout or starvation detection
      break;
    }
  }
}

int RemoveForwardingHeaderFromGatewayMessages(MqttSnForwarder *forwarder,
                                              MqttSnMessageData *gatewayMessageData,
                                              MqttSnMessageData *clientMessageData) {
  memset(gatewayMessageData, 0, sizeof(MqttSnMessageData));
  memset(clientMessageData, 0, sizeof(MqttSnMessageData));

  if (isFull(&forwarder->clientNetworkSendBuffer)) {
    return 0;
  }

  if (pop(&forwarder->gatewayNetworkReceiveBuffer, gatewayMessageData) != 0) {
    return 0;
  }

  if (RemoveMqttSnForwardingHeader(gatewayMessageData,
                                   clientMessageData,
                                   forwarder->clientNetwork.client_network_broadcast_address) != 0) {
#ifdef WITH_DEBUG_LOGGING
    log_gateway_mqtt_sn_message_malformed(&forwarder->logger,
                                          &gatewayMessageData->from,
                                          gatewayMessageData->data,
                                          gatewayMessageData->data_length,
                                          gatewayMessageData->signal_strength);
#endif
    return 0;
  }

#ifdef WITH_LOGGING
  if (log_gateway_mqtt_sn_message(&forwarder->logger,
                                  &clientMessageData->to,
                                  clientMessageData->data,
                                  clientMessageData->data_length,
                                  NULL)) {
    return -1;
  }
#endif

  if (put(&forwarder->clientNetworkSendBuffer, clientMessageData) != 0) {
    // we do our best to not drop message due to RAM loss
    put(&forwarder->gatewayNetworkReceiveBuffer, gatewayMessageData);
  }
  return 0;
}

int AddForwardingHeaderToClientMessages(MqttSnForwarder *forwarder,
                                        MqttSnMessageData *clientMessageData,
                                        MqttSnMessageData *gatewayMessageData) {
  memset(clientMessageData, 0, sizeof(MqttSnMessageData));
  memset(gatewayMessageData, 0, sizeof(MqttSnMessageData));

  if (isFull(&forwarder->gatewayNetworkSendBuffer)) {
    return 0;
  }

  if (pop(&forwarder->clientNetworkReceiveBuffer, clientMessageData) != 0) {
    return 0;
  }

#ifdef WITH_LOGGING
  log_client_mqtt_sn_message(&forwarder->logger,
                             &clientMessageData->from,
                             clientMessageData->data,
                             clientMessageData->data_length, NULL);
#endif
#ifdef WITH_DEBUG_LOGGING
  ParsedMqttSnHeader header = {0};
  if (parse_message_tolerant(&header, ANY_MESSAGE_TYPE, clientMessageData->data, clientMessageData->data_length) < 0) {
    log_client_mqtt_sn_message_malformed(&forwarder->logger,
                                         &clientMessageData->from,
                                         clientMessageData->data,
                                         clientMessageData->data_length,
                                         clientMessageData->signal_strength);
  }
#endif
  if (AddMqttSnForwardingHeader(clientMessageData,
                                gatewayMessageData,
                                forwarder->gatewayNetwork.mqtt_sn_gateway_address,
                                forwarder->clientNetwork.client_network_broadcast_address) != 0) {
#ifdef WITH_DEBUG_LOGGING
    log_could_not_generate_encapsulation_message(&forwarder->logger,
                                                 MQTT_SN_FORWARDER_NETWORK_GATEWAY,
                                                 &clientMessageData->from,
                                                 clientMessageData->data,
                                                 clientMessageData->data_length,
                                                 clientMessageData->signal_strength);
#endif
    return 0;
  }
  if (put(&forwarder->gatewayNetworkSendBuffer, gatewayMessageData) != 0) {
    // we do our best to not drop message due to RAM loss
    put(&forwarder->clientNetworkReceiveBuffer, clientMessageData);
  }
  return 0;
}

int RemoveMqttSnForwardingHeader(MqttSnMessageData *gatewayMessageData,
                                 MqttSnMessageData *clientMessageData,
                                 const device_address *client_network_broadcast_address) {
  uint8_t broadcast = 0;
  int32_t rc = parse_encapsulation_message(&broadcast,
                                           &clientMessageData->to,
                                           clientMessageData->data,
                                           &clientMessageData->data_length,
                                           CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH,
                                           gatewayMessageData->data,
                                           gatewayMessageData->data_length);
  if (rc < 0) {
    return -1;
  }
  if (broadcast == MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BROADCAST_RADIUS) {
    clientMessageData->to = *client_network_broadcast_address;
  }
  clientMessageData->from = gatewayMessageData->from;
  return 0;
}
int AddMqttSnForwardingHeader(MqttSnMessageData *clientMessageData,
                              MqttSnMessageData *gatewayMessageData,
                              const device_address *mqtt_sn_gateway_network_address,
                              const device_address *client_network_broadcast_address) {
  uint8_t broadcast = 0;
  if (client_network_broadcast_address
      && memcmp(&gatewayMessageData->to, client_network_broadcast_address, sizeof(device_address)) == 0) {
    broadcast = MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BROADCAST_RADIUS;
  }
  int rc = generate_encapsulation_message(gatewayMessageData->data,
                                          CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH,
                                          broadcast,
                                          &clientMessageData->from,
                                          clientMessageData->data,
                                          clientMessageData->data_length);
  if (rc < 0) {
    return -1;
  }
  gatewayMessageData->data_length = rc;
  gatewayMessageData->to = *mqtt_sn_gateway_network_address;
  gatewayMessageData->from = clientMessageData->from;

  return 0;
}
