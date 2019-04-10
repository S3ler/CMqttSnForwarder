//
// Created by SomeDude on 10.04.2019.
//

#include <dlfcn.h>
#include <network/plugin/MqttSnPluginLogger.h>
#include "MqttSnClientPluginNetwork.h"

int ClientLinuxPluginInit(MqttSnClientNetworkInterface *n, void *context) {
  return 0;
}
int ClientLinuxPluginConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  if (pluginClientNetwork->plugin_network_connect(pluginClientNetwork->plugin_cfg,
                                                  pluginClientNetwork->plugin_context)) {
    return -1;
  }
#ifdef WITH_LOGGING
  if (n->logger) {
    log_network_connect(n->logger,
                        n->logger->log_level,
                        pluginClientNetwork->plugin_get_short_network_protocol_name(), NULL,
                        n->client_network_address);
  }
#endif
  return 0;
}
void ClientLinuxPluginDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;

  pluginClientNetwork->plugin_network_disconnect(pluginClientNetwork->plugin_cfg,
                                                 pluginClientNetwork->plugin_context);
  char *error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
  }
#ifdef WITH_LOGGING
  if (n->logger) {
    log_network_disconnect(n->logger,
                           n->logger->log_level,
                           pluginClientNetwork->plugin_get_short_network_protocol_name(), NULL,
                           n->client_network_address);
  }
#endif
  if (dlclose(pluginClientNetwork->dl_handle)) {
    error = dlerror();
    if (error != NULL) {
#ifdef WITH_LOGGING
      log_dlerror(n->logger, n->logger->log_level, error);
#endif
    }
  }
}
int ClientLinuxPluginSend(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *sendBuffer,
                          int timeout_ms,
                          void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;

  MqttSnMessageData gatewaySendMessageData = {0};
  if (pop(sendBuffer, &gatewaySendMessageData) != 0) {
    return 0;
  }
  if (pluginClientNetwork->plugin_get_maximum_message_length() <= gatewaySendMessageData.data_length) {
    // too long messages for the protocol are ignored by the standard
#ifdef WITH_DEBUG_LOGGING
    if (log_too_long_message(n->logger, n->logger->log_level,
                             &gatewaySendMessageData.address,
                             gatewaySendMessageData.data,
                             gatewaySendMessageData.data_length)) {
      return -1;
    }
#endif
    return 0;
  }

  client_plugin_device_address dest = {
      .bytes = gatewaySendMessageData.address.bytes,
      .length = sizeof(device_address)};
  const client_plugin_message message = {
      .address = dest,
      .data = gatewaySendMessageData.data,
      .data_length = gatewaySendMessageData.data_length};
  int send_bytes = pluginClientNetwork->plugin_network_send(
      &message,
      timeout_ms,
      pluginClientNetwork->plugin_cfg,
      pluginClientNetwork->plugin_context);

  if (send_bytes == -1) {
    return -1;
  }
  if (send_bytes != gatewaySendMessageData.data_length) {
    put(sendBuffer, &gatewaySendMessageData);
#ifdef WITH_DEBUG_LOGGING
    if (log_incomplete_message(n->logger, n->logger->log_level,
                               &gatewaySendMessageData.address,
                               gatewaySendMessageData.data,
                               gatewaySendMessageData.data_length)) {
      return -1;
    }
#endif
    return -1;
  }

  return 0;
}
int ClientLinuxPluginReceive(MqttSnClientNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *receiveBuffer,
                             int timeout_ms,
                             void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;

  MqttSnMessageData receiveMessageData = {0};

  client_plugin_device_address rec_address = {
      .length = sizeof(device_address),
      .bytes = receiveMessageData.address.bytes};
  client_plugin_message rec_message = {
      .address = rec_address,
      .data_length = 0,
      .data = receiveMessageData.data};

  int rc = pluginClientNetwork->plugin_network_receive(&rec_message,
                                                       timeout_ms,
                                                       pluginClientNetwork->plugin_cfg,
                                                       pluginClientNetwork->plugin_context);
  if (rc < -1) {
    return -1;
  }
  if (rc == 0) {
    // no message received
    return 0;
  }
  put(receiveBuffer, &receiveMessageData);

#ifdef WITH_DEBUG_LOGGING
  if (n->logger) {
    const MqttSnMessageData *msg = back(receiveBuffer);
    log_gateway_message(n->logger,
                        n->logger->log_level,
                        &msg->address,
                        msg->data,
                        msg->data_length);
  }
#endif

  return 0;
}
