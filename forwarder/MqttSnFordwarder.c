//
// Created by bele on 26.01.19.
//

#include <memory.h>
#include <stdio.h>
#include <signal.h>
#include "MqttSnForwarder.h"
#include "MqttSnFixedSizeRingBuffer.h"
#include "MqttSnClientNetworkInterface.h"

int MqttSnForwarderStart(MqttSnForwarder *mqttSnForwarder,
                         void *clientNetworkContext,
                         void *gatewayNetworkContext,
                         volatile sig_atomic_t *keep_running) {

  if (MqttSnForwarderInit(mqttSnForwarder, clientNetworkContext, gatewayNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

  if (ClientNetworkConnect(&mqttSnForwarder->clientNetwork, mqttSnForwarder->clientNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

  if (GatewayNetworkConnect(&mqttSnForwarder->gatewayNetwork, mqttSnForwarder->gatewayNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

  while ((*keep_running == 1) && MqttSnForwarderLoop(mqttSnForwarder) == 0) {}

  MqttSnForwarderDeinit(mqttSnForwarder);

  return 0;
}

int MqttSnForwarderInit(MqttSnForwarder *mqttSnForwarder, void *clientNetworkContext, void *gatewayNetworkContext) {
  mqttSnForwarder->clientNetworkContext = clientNetworkContext;
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->clientNetworkReceiveBuffer);
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->clientNetworkSendBuffer);

  mqttSnForwarder->gatewayNetworkContext = gatewayNetworkContext;
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->gatewayNetworkReceiveBuffer);
  MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->gatewayNetworkSendBuffer);

  return 0;
}

void MqttSnForwarderDeinit(MqttSnForwarder *forwarder) {
  GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
}

int MqttSnForwarderLoop(MqttSnForwarder *forwarder) {

  if (forwarder->clientNetwork.client_network_receive(
      &forwarder->clientNetwork,
      &forwarder->clientNetworkReceiveBuffer,
      CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
      forwarder->clientNetworkContext) != 0) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  }

  if (forwarder->gatewayNetwork.gateway_receive(
      &forwarder->gatewayNetwork,
      &forwarder->gatewayNetworkReceiveBuffer,
      GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
      forwarder->gatewayNetworkContext) != 0) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }

  if (AddForwardingHeaderToClientMessages(forwarder) != 0) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }

  if (RemoveForwardingHeaderFromGatewayMessages(forwarder) != 0) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }

  if (forwarder->gatewayNetwork.gateway_send(
      &forwarder->gatewayNetwork,
      &forwarder->gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      forwarder->gatewayNetworkContext) != 0) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  }

  if (forwarder->clientNetwork.client_network_send(
      &forwarder->clientNetwork,
      &forwarder->clientNetworkSendBuffer,
      CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT,
      forwarder->clientNetworkContext)) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }

  if (forwarder->clientNetwork.status < 0 && forwarder->gatewayNetwork.status < 0) {
    // both networks failed => end
    // printf("both networks failed\n");
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
    return -1;
  } else if (forwarder->clientNetwork.status < 0 && forwarder->gatewayNetwork.status > 0) {
    // only client network is down
    // buffered message will be tried to sendNetwork out to the gateway network
    sendBufferedMessagesToGateway(forwarder);
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    // printf("client network failed\n");
    return -1;
  } else if (forwarder->clientNetwork.status > 0 && forwarder->gatewayNetwork.status < 0) {
    // only gateway network is down
    // buffered message will be tried to sendNetwork out to the clients
    sendBufferedMessagesToClients(forwarder);
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
    // printf("gateway network failed\n");
    return -1;
  }
  return 0;
}

void sendBufferedMessagesToClients(MqttSnForwarder *forwarder) {
  while (!isEmpty(&forwarder->gatewayNetworkReceiveBuffer) &&
      !isEmpty(&forwarder->clientNetworkSendBuffer)) {
    if (forwarder->clientNetwork.client_network_receive(
        &forwarder->clientNetwork,
        &forwarder->clientNetworkReceiveBuffer,
        CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
        forwarder->clientNetworkContext) != 0) {
      break;
    }
    if (RemoveForwardingHeaderFromGatewayMessages(forwarder) != 0) {
      break;
    }
    if (forwarder->clientNetwork.client_network_send(
        &forwarder->clientNetwork,
        &forwarder->clientNetworkSendBuffer,
        CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT,
        forwarder->clientNetworkContext) != 0) {
      break;
    }
  }
}
void sendBufferedMessagesToGateway(MqttSnForwarder *forwarder) {
  while (!isEmpty(&forwarder->clientNetworkReceiveBuffer) &&
      !isEmpty(&forwarder->gatewayNetworkSendBuffer)) {
    if (forwarder->gatewayNetwork.gateway_receive(
        &forwarder->gatewayNetwork,
        &forwarder->gatewayNetworkReceiveBuffer,
        GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
        forwarder->gatewayNetworkContext) != 0) {
      break;
    }
    if (AddForwardingHeaderToClientMessages(forwarder) != 0) {
      break;
    }
    if (forwarder->gatewayNetwork.gateway_send(
        &forwarder->gatewayNetwork,
        &forwarder->gatewayNetworkSendBuffer,
        GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
        forwarder->gatewayNetworkContext) != 0) {
      break;
    }
  }
}

int RemoveForwardingHeaderFromGatewayMessages(MqttSnForwarder *forwarder) {
  MqttSnMessageData gatewayMessageData = {0};
  MqttSnMessageData clientMessageData = {0};

  if (pop(&forwarder->gatewayNetworkReceiveBuffer, &gatewayMessageData) != 0) {
    return 0;
  }
  if (RemoveMqttSnForwardingHeader(&gatewayMessageData, &clientMessageData) != 0) {
    return 0;
  }
  if (put(&forwarder->clientNetworkSendBuffer, &clientMessageData) != 0) {
    // we do our best to not drop message due to RAM loss
    put(&forwarder->clientNetworkReceiveBuffer, &gatewayMessageData);
  }
  return 0;
}

int AddForwardingHeaderToClientMessages(MqttSnForwarder *forwarder) {
  MqttSnMessageData clientMessageData = {0};
  MqttSnMessageData gatewayMessageData = {0};

  if (pop(&forwarder->clientNetworkReceiveBuffer, &clientMessageData) != 0) {
    return 0;
  }
  if (AddMqttSnForwardingHeader(&clientMessageData, &gatewayMessageData) != 0) {
    return 0;
  }
  if (put(&forwarder->gatewayNetworkSendBuffer, &gatewayMessageData) != 0) {
    // we do our best to not drop message due to RAM loss
    put(&forwarder->clientNetworkReceiveBuffer, &clientMessageData);
  }
  return 0;
}

int RemoveMqttSnForwardingHeader(MqttSnMessageData *receiveMessageData, MqttSnMessageData *sendMessageData) {
  if (receiveMessageData->data_length > CLIENT_NETWORK_MAX_DATA_LEN) {
    return -1;
  }
  // at least some validation
  if (receiveMessageData->data_length < FORWARDER_HEADER_LEN + sizeof(device_address)) {
    return -1;
  }
  // TODO: empty MQTT-SN messages are accepted
  if (receiveMessageData->data_length != receiveMessageData->data[0]) {
    return -1;
  }

  MQTT_SN_FORWARD_ENCAPSULATION *packet = (MQTT_SN_FORWARD_ENCAPSULATION *) receiveMessageData->data;
  sendMessageData->address = packet->wireless_node_id;
  sendMessageData->data_length = packet->mqtt_sn_message[0];
  memcpy(sendMessageData->data, packet->mqtt_sn_message, sendMessageData->data_length);
  // sendMessageData->data = packet->mqtt_sn_message;

  // TODO rework the data adding part
  // TODO: alternative validation of MQTT-SN header

  return 0;
}

int AddMqttSnForwardingHeader(MqttSnMessageData *receiveMessageData, MqttSnMessageData *sendMessageData) {
  if ((receiveMessageData->data_length + FORWARDER_HEADER_LEN + sizeof(device_address)) >
      GATEWAY_NETWORK_MAX_DATA_LEN) {
    return -1;
  }

  // create encapsulation message
  MQTT_SN_FORWARD_ENCAPSULATION packet = {0};
  packet.length = receiveMessageData->data_length + FORWARDER_HEADER_LEN + sizeof(device_address);
  packet.msg_type = 0xFE;
  packet.crtl = 0x0; // TODO implement detection of broadcast
  packet.wireless_node_id = receiveMessageData->address;
  // TODO check if this works
  memcpy(packet.mqtt_sn_message, receiveMessageData->data, receiveMessageData->data_length);
  // packet.mqtt_sn_message = receiveMessageData->data;

  // TODO rework the data adding part
  memcpy(sendMessageData, receiveMessageData, sizeof(MqttSnMessageData));
  sendMessageData->data_length = packet.length;
  memcpy(sendMessageData->data, &packet, sizeof(MQTT_SN_FORWARD_ENCAPSULATION));

  // TODO: alternative validation of MQTT-SN header

  return 0;
}