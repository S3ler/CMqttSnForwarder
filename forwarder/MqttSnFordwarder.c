//
// Created by bele on 26.01.19.
//

#include <memory.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include "MqttSnForwarder.h"
#include "MqttSnFixedSizeRingBuffer.h"
#include "MqttSnClientNetworkInterface.h"
#include "MqttSnMessageParser.h"
#include "logging/MqttSnForwarderLoggingMessages.h"

int MqttSnForwarderInit(MqttSnForwarder *mqttSnForwarder,
                        log_level_t log_level,
                        void *clientNetworkContext,
                        void *gatewayNetworkContext) {

#ifdef WITH_LOGGING
  if (MqttSnLoggerInit(&mqttSnForwarder->logger, log_level) != 0) {
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
#ifdef WITH_LOGGING
  if (mqttSnForwarder->logger.status) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }
#endif

  if (GatewayNetworkConnect(&mqttSnForwarder->gatewayNetwork, mqttSnForwarder->gatewayNetworkContext) != 0) {
#ifdef WITH_LOGGING
    log_network_connect_fail(&mqttSnForwarder->logger,
                             NULL,
                             "gateway",
                             mqttSnForwarder->gatewayNetwork.forwarder_network_address,
                             mqttSnForwarder->gatewayNetwork.gateway_network_address);
#endif
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }
#ifdef WITH_LOGGING
  if (mqttSnForwarder->logger.status) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }
#endif

  return 0;
}

/**
 * disconncts the gateway and client network
 * @param forwarder
 */
void MqttSnForwarderDeinit(MqttSnForwarder *forwarder) {
  GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
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
  {
    MqttSnMessageData clientMessageData = {0};
    MqttSnMessageData gatewayMessageData = {0};
    if (AddForwardingHeaderToClientMessages(forwarder, &clientMessageData, &gatewayMessageData) != 0) {
      ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
      GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    }
  }

  if (GatewayNetworkSend(&forwarder->gatewayNetwork,
                         &forwarder->gatewayNetworkSendBuffer,
                         forwarder->gatewayNetworkSendTimeout,
                         forwarder->gatewayNetworkContext)) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }

  if (GatewayNetworkReceive(&forwarder->gatewayNetwork,
                            &forwarder->gatewayNetworkReceiveBuffer,
                            forwarder->gatewayNetworkReceiveTimeout,
                            forwarder->gatewayNetworkContext)) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }

  {
    MqttSnMessageData gatewayMessageData = {0};
    MqttSnMessageData clientMessageData = {0};
    if (RemoveForwardingHeaderFromGatewayMessages(forwarder, &gatewayMessageData, &clientMessageData) != 0) {
      ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
      GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    }
  }

  if (ClientNetworkSend(&forwarder->clientNetwork,
                        &forwarder->clientNetworkSendBuffer,
                        forwarder->clientNetworkSendTimeout,
                        forwarder->clientNetworkContext)) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  }

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
      if (RemoveForwardingHeaderFromGatewayMessages(forwarder, &gatewayMessageData, &clientMessageData) != 0) {
        break;
      }
    }
    if (forwarder->clientNetwork.client_network_send(
        &forwarder->clientNetwork,
        &forwarder->clientNetworkSendBuffer,
        forwarder->clientNetworkSendTimeout,
        forwarder->clientNetworkContext) != 0) {
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

    if (forwarder->gatewayNetwork.gateway_network_send(
        &forwarder->gatewayNetwork,
        &forwarder->gatewayNetworkSendBuffer,
        forwarder->gatewayNetworkSendTimeout,
        forwarder->gatewayNetworkContext) != 0) {
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

  if (RemoveMqttSnForwardingHeader(gatewayMessageData, clientMessageData) != 0) {
    return 0;
  }

#ifdef WITH_LOGGING
  if (log_gateway_mqtt_sn_message(&forwarder->logger,
                                  forwarder->logger.log_level,
                                  &clientMessageData->address,
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
                             forwarder->logger.log_level,
                             &clientMessageData->address,
                             clientMessageData->data,
                             clientMessageData->data_length, NULL);
#endif

  if (AddMqttSnForwardingHeader(clientMessageData, gatewayMessageData) != 0) {
    return 0;
  }
  if (put(&forwarder->gatewayNetworkSendBuffer, gatewayMessageData) != 0) {
    // we do our best to not drop message due to RAM loss
    put(&forwarder->clientNetworkReceiveBuffer, clientMessageData);
  }
  return 0;
}

int RemoveMqttSnForwardingHeader(MqttSnMessageData *gatewayMessageData, MqttSnMessageData *clientMessageData) {

  ParsedMqttSnHeader gatewayMessageHeader = {0};
  if (parse_encapsulation(&gatewayMessageHeader, gatewayMessageData->data, gatewayMessageData->data_length)) {
    // not valid enough
    return -1;
  }

  MqttSnEncapsulatedMessage *encapsulatedMessage = (MqttSnEncapsulatedMessage *) gatewayMessageHeader.payload;
  ParsedMqttSnHeader encapsulatedMessageHeader = {0};
  if (parse_header(&encapsulatedMessageHeader,
                   gatewayMessageData->data
                       + MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(gatewayMessageHeader.indicator),
                   gatewayMessageHeader.length
                       - MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(gatewayMessageHeader.indicator))) {
    // not valid header in encapsulated message
    return -1;
  }
  if (encapsulatedMessageHeader.length > CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) {
    return -1;
  }
  memcpy(&clientMessageData->address, &encapsulatedMessage->wireless_node_id, sizeof(device_address));
  clientMessageData->data_length = encapsulatedMessageHeader.length;

  memcpy(clientMessageData->data,
         gatewayMessageData->data + MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(gatewayMessageHeader.indicator),
         encapsulatedMessageHeader.length);
  return 0;
}
int AddMqttSnForwardingHeader(MqttSnMessageData *clientMessageData, MqttSnMessageData *gatewayMessageData) {

  ParsedMqttSnHeader clientMessageHeader = {0};
  if (parse_header(&clientMessageHeader, clientMessageData->data, clientMessageData->data_length)) {
    // not valid enough
    return -1;
  }

  if (clientMessageHeader.length + MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(clientMessageHeader.indicator)
      > CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) {
    return -1;
  }
  gatewayMessageData->data_length = clientMessageHeader.length
      + MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(clientMessageHeader.indicator);
  gatewayMessageData->address = clientMessageData->address;

  if (clientMessageHeader.indicator) {
    MqttSnMessageHeaderThreeOctetsLengthField
        *headerThreeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) gatewayMessageData->data;
    headerThreeOctetsLengthField->indicator = clientMessageHeader.indicator;
    headerThreeOctetsLengthField->length = htons(gatewayMessageData->data_length);
    headerThreeOctetsLengthField->msg_type = ENCAPSULATED_MESSAGE;
  } else {
    MqttSnMessageHeaderOneOctetLengthField
        *headerThreeOctetsLengthField = (MqttSnMessageHeaderOneOctetLengthField *) gatewayMessageData->data;
    headerThreeOctetsLengthField->length = gatewayMessageData->data_length;
    headerThreeOctetsLengthField->msg_type = ENCAPSULATED_MESSAGE;
  }

  MqttSnEncapsulatedMessage *encapsulatedMessage = (MqttSnEncapsulatedMessage *) (gatewayMessageData->data
      + MQTT_SN_HEADER_LENGTH(clientMessageHeader.indicator));
  encapsulatedMessage->crtl = 0;
  memcpy(&encapsulatedMessage->wireless_node_id, &clientMessageData->address, sizeof(device_address));

  memcpy(&gatewayMessageData->data[MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(clientMessageHeader.indicator)],
         clientMessageData->data,
         clientMessageHeader.length);
  return 0;
}
