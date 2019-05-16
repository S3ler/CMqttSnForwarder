//
// Created by bele on 28.01.19.
//

#include "MqttSnClientNetworkInterface.h"
#include <assert.h>
#include <stdio.h>

int32_t ClientNetworkInitialize(MqttSnClientNetworkInterface *n,
                                uint16_t max_data_length,
                                device_address *mqtt_sn_gateway_address,
                                device_address *client_network_address,
                                device_address *client_network_broadcast_address,
                                void *context,
                                int (*client_network_init)(MqttSnClientNetworkInterface *, void *)) {
  if (!(n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DEINITIALIZED
      || n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_INITIALIZED)) {
    return -1;
  }
  if (client_network_init == NULL) {
    return -1;
  }

  n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DEINITIALIZED;
  n->max_data_length = max_data_length;
  n->client_network_address = client_network_address;
  n->mqtt_sn_gateway_address = mqtt_sn_gateway_address;
  n->client_network_broadcast_address = client_network_broadcast_address;
  n->initialize = client_network_init;
  if (n->initialize(n, context) == 0) {
    if (n->initialize == NULL) {
      return -1;
    }
    if (n->deinitialize == NULL) {
      return -1;
    }
    if (n->connect == NULL) {
      return -1;
    }
    if (n->disconnect == NULL) {
      return -1;
    }
    if (n->send == NULL) {
      return -1;
    }
    if (n->receive == NULL) {
      return -1;
    }

    n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_INITIALIZED;
  } else {
    n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DEINITIALIZED;
  }

  if (n->status != MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_INITIALIZED) {
    return -1;
  }
  return 0;
}

int32_t ClientNetworkDeinitialize(MqttSnClientNetworkInterface *n, void *context) {
  if (!(n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DISCONNECTED
      || n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DEINITIALIZED)) {
    return -1;
  }
  if (n->deinitialize == NULL) {
    return -1;
  }
  n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DEINITIALIZED;
  if (n->deinitialize(n, context) < 0) {
    return -1;
  }
  return 0;
}

int32_t ClientNetworkConnect(MqttSnClientNetworkInterface *n, void *context) {
  if (!(n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_INITIALIZED
      || n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED)) {
    return -1;
  }
  if (n->connect == NULL) {
    return -1;
  }

  if (n->connect(n, context) < 0) {
    n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DISCONNECTED;
    return -1;
  }
  n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED;

  if (n->status != MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED) {
    return -1;
  }
  return 0;
}

int32_t ClientNetworkDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  if (!(n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED
      || n->status == MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DISCONNECTED)) {
    return -1;
  }
  if (n->disconnect == NULL) {
    return -1;
  }

  n->disconnect(n, context);
  n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DISCONNECTED;

  return 0;
}

int32_t ClientNetworkSend(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *sendBuffer,
                          int timeout_ms,
                          void *context) {
  if (n->status != MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED) {
    return -1;
  }
  if (n->send == NULL) {
    return -1;
  }

  MqttSnMessageData msg = {0};
  if (pop(sendBuffer, &msg) < 0) {
    return 0;
  }

  int send_rc = n->send(n,
                        &msg.from,
                        &msg.to,
                        msg.data,
                        msg.data_length,
                        msg.signal_strength,
                        timeout_ms,
                        context);
  if (send_rc < 0) {
    n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DISCONNECTED;
  }

  if (msg.data_length != send_rc) {
    if (put(sendBuffer, &msg) < 0) {
      return -1;
    }
  }

  if (n->status != MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED) {
    return -1;
  }
  return 0;
}
int32_t ClientNetworkReceive(MqttSnClientNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *receiveBuffer,
                             int timeout_ms,
                             void *context) {
  if (n->status != MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED) {
    return -1;
  }
  if (n->receive == NULL) {
    return -1;
  }

  if (isFull(receiveBuffer)) {
    return 0;
  }
  MqttSnMessageData msg = {0};

  int32_t receive_rc = n->receive(n,
                                  &msg.from,
                                  &msg.to,
                                  msg.data,
                                  n->max_data_length,
                                  &msg.signal_strength,
                                  timeout_ms,
                                  context);
  if (receive_rc < 0) {
    n->status = MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DISCONNECTED;
  }

  msg.data_length = receive_rc;
  if (msg.data_length > 0) {
    if (put(receiveBuffer, &msg) < 0) {
      // ignored
    }
  }

  if (n->status != MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED) {
    return -1;
  }
  return 0;
}
