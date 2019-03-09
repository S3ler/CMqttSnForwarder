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
                         void *gatewayNetworkContext) {

  if (MqttSnForwarderInit(mqttSnForwarder, clientNetworkContext, gatewayNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

  while (MqttSnForwarderLoop(mqttSnForwarder) == 0) {}

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

  if (ClientNetworkConnect(&mqttSnForwarder->clientNetwork, mqttSnForwarder->clientNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

  if (GatewayNetworkConnect(&mqttSnForwarder->gatewayNetwork, mqttSnForwarder->gatewayNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return -1;
  }

  return 0;
}

void MqttSnForwarderDeinit(MqttSnForwarder *forwarder) {
  GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
}

int MqttSnForwarderLoop(MqttSnForwarder *forwarder) {
  // TODO when clientNetwork.status == -1 then clientNetwork shall functions shall return -1
  // TODO test this in clientnetworktests
  if (forwarder->clientNetwork.client_network_receive(
      &forwarder->clientNetwork,
      &forwarder->clientNetworkReceiveBuffer,
      CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
      forwarder->clientNetworkContext) != 0) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  }
  // TODO when gatewayNetwork.status == -1 then clientNetwork shall functions shall return -1
  // TODO test this in gatewaynetworktests
  if (forwarder->gatewayNetwork.gateway_receive(
      &forwarder->gatewayNetwork,
      &forwarder->gatewayNetworkReceiveBuffer,
      GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
      forwarder->gatewayNetworkContext) != 0) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
  }

  {
    MqttSnMessageData clientMessageData = {0};
    MqttSnMessageData gatewayMessageData = {0};
    if (AddForwardingHeaderToClientMessages(forwarder, &clientMessageData, &gatewayMessageData) != 0) {
      ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
      GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    }
  }

  {
    MqttSnMessageData gatewayMessageData = {0};
    MqttSnMessageData clientMessageData = {0};
    if (RemoveForwardingHeaderFromGatewayMessages(forwarder, &gatewayMessageData, &clientMessageData) != 0) {
      ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
      GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
    }
  }
  // TODO when gatewayNetwork.status == -1 then clientNetwork shall functions shall return -1
  // TODO test this in gatewaynetworktests
  if (forwarder->gatewayNetwork.gateway_send(
      &forwarder->gatewayNetwork,
      &forwarder->gatewayNetworkSendBuffer,
      GATEWAY_NETWORK_DEFAULT_SEND_TIMEOUT,
      forwarder->gatewayNetworkContext) != 0) {
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetworkContext);
  }
  // TODO when clientNetwork.status == -1 then clientNetwork shall functions shall return -1
  // TODO test this in clientnetworktests
  if (forwarder->clientNetwork.client_network_send(
      &forwarder->clientNetwork,
      &forwarder->clientNetworkSendBuffer,
      CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT,
      forwarder->clientNetworkContext) != 0) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetworkContext);
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
    /*
    if (forwarder->clientNetwork.client_network_receive(
        &forwarder->clientNetwork,
        &forwarder->clientNetworkReceiveBuffer,
        CLIENT_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
        forwarder->clientNetworkContext) != 0) {
      break;
    }
     */


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
        CLIENT_NETWORK_DEFAULT_SEND_TIMEOUT,
        forwarder->clientNetworkContext) != 0) {
      break;
    }
  }
}
void sendBufferedMessagesToGateway(MqttSnForwarder *forwarder) {
  while (!isEmpty(&forwarder->clientNetworkReceiveBuffer) |
      !isEmpty(&forwarder->gatewayNetworkSendBuffer)) {
    /*
    if (forwarder->gatewayNetwork.gateway_receive(
        &forwarder->gatewayNetwork,
        &forwarder->gatewayNetworkReceiveBuffer,
        GATEWAY_NETWORK_DEFAULT_RECEIVE_TIMEOUT,
        forwarder->gatewayNetworkContext) != 0) {
      break;
    }
    */
    {
      MqttSnMessageData gatewayMessageData = {0};
      MqttSnMessageData clientMessageData = {0};
      if (AddForwardingHeaderToClientMessages(forwarder, &clientMessageData, &gatewayMessageData) != 0) {
        break;
      }
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

int RemoveForwardingHeaderFromGatewayMessages(MqttSnForwarder *forwarder,
                                              MqttSnMessageData *gatewayMessageData,
                                              MqttSnMessageData *clientMessageData) {
  memset(gatewayMessageData, 0, sizeof(MqttSnMessageData));
  memset(clientMessageData, 0, sizeof(MqttSnMessageData));

  if (pop(&forwarder->gatewayNetworkReceiveBuffer, gatewayMessageData) != 0) {
    return 0;
  }
  if (RemoveMqttSnForwardingHeader(gatewayMessageData, clientMessageData) != 0) {
    return 0;
  }
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

  if (pop(&forwarder->clientNetworkReceiveBuffer, clientMessageData) != 0) {
    return 0;
  }
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
  if (gatewayMessageData->data_length == 0) {
    return -1;
  }
  if (gatewayMessageData->data_length > CLIENT_NETWORK_MAX_DATA_LEN) {
    return -1;
  }
  if (gatewayMessageData->data_length < FORWARDER_HEADER_LEN + sizeof(device_address)) {
    return -1;
  }
  // TODO: empty MQTT-SN messages are not accepted, at least the header byte count must be equal
  // TODO: at the moment the maximum data_length supported is 255
  // TODO: no validation if the MQTT-SN Encapsulation Message really contains a MQTT-SN Message
  if (gatewayMessageData->data_length != gatewayMessageData->data[0]) {
    return -1;
  }

  MQTT_SN_FORWARD_ENCAPSULATION *packet = (MQTT_SN_FORWARD_ENCAPSULATION *) gatewayMessageData->data;
  clientMessageData->address = packet->wireless_node_id;
  clientMessageData->data_length = packet->mqtt_sn_message[0];
  memcpy(clientMessageData->data, packet->mqtt_sn_message, clientMessageData->data_length);
  // clientMessageData->data = packet->mqtt_sn_message;

  // TODO rework the data adding part
  // TODO: alternative validation of MQTT-SN header

  return 0;
}

int AddMqttSnForwardingHeader(MqttSnMessageData *clientMessageData, MqttSnMessageData *gatewayMessageData) {
  // empty messages are also converted?
  if (clientMessageData->data_length == 0) {
    return -1;
  }
  if (clientMessageData->data_length
      > GATEWAY_NETWORK_MAX_DATA_LEN - (FORWARDER_HEADER_LEN + sizeof(device_address))) {
    return -1;
  }

  // create encapsulation message
  MQTT_SN_FORWARD_ENCAPSULATION packet = {0};
  packet.length = clientMessageData->data_length + FORWARDER_HEADER_LEN + sizeof(device_address);
  packet.msg_type = 0xFE;
  packet.crtl = 0x0; // TODO implement detection of broadcast
  packet.wireless_node_id = clientMessageData->address;
  // TODO check if this works
  memcpy(packet.mqtt_sn_message, clientMessageData->data, clientMessageData->data_length);
  // packet.mqtt_sn_message = clientMessageData->data;

  // TODO rework the data adding part
  memcpy(gatewayMessageData, clientMessageData, sizeof(MqttSnMessageData));
  gatewayMessageData->data_length = packet.length;
  memcpy(gatewayMessageData->data, &packet, sizeof(MQTT_SN_FORWARD_ENCAPSULATION));

  // TODO: alternative validation of MQTT-SN header

  return 0;
}