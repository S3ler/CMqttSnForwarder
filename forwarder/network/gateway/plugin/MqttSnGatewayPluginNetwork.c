//
// Created by SomeDude on 07.04.2019.
//

#include "MqttSnGatewayPluginNetwork.h"
#include "network/plugin/MqttSnPluginLogger.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GatewayLinuxPluginInit(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  pluginGatewayNetwork->dl_handle = NULL;
  pluginGatewayNetwork->plugin_context = NULL;

  n->gateway_network_init = GatewayLinuxPluginInit;
  n->gateway_network_receive = GatewayLinuxPluginReceive;
  n->gateway_network_send = GatewayLinuxPluginSend;
  n->gateway_network_connect = GatewayLinuxPluginConnect;
  n->gateway_network_disconnect = GatewayLinuxPluginDisconnect;

  char *error = NULL;

  pluginGatewayNetwork->dl_handle = dlopen(pluginGatewayNetwork->plugin_cfg->plugin_path, RTLD_NOW);
  if (!pluginGatewayNetwork->dl_handle) {
#ifdef WITH_LOGGING
    error = dlerror();
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }
  dlerror();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_get_short_network_protocol_name =
      (const char *(*)())
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_get_short_network_protocol_name");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_get_maximum_message_length =
      (uint16_t (*)())
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_get_maximum_message_length");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_init =
      (int (*)(const gateway_plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_init");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_deinit =
      (int (*)(const gateway_plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_deinit");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_connect =
      (int (*)(const gateway_plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_connect");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_disconnect =
      (void (*)(const gateway_plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_disconnect");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_send =
      (int (*)(const gateway_plugin_message *send_message,
               int timeout_ms,
               const gateway_plugin_config *cfg,
               void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_send");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_receive =
      (int (*)(gateway_plugin_message *rec_message,
               int timeout_ms,
               const gateway_plugin_config *cfg,
               void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_receive");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

  if (strcmp(pluginGatewayNetwork->plugin_cfg->protocol, pluginGatewayNetwork->plugin_get_short_network_protocol_name())
      != 0) {
#ifdef WITH_LOGGING
    log_protocol_mismatch(n->logger,
                          n->logger->log_level,
                          pluginGatewayNetwork->plugin_cfg->protocol,
                          pluginGatewayNetwork->plugin_get_short_network_protocol_name());
#endif
    return -1;
  }
  if (pluginGatewayNetwork->plugin_cfg->forwarder_maximum_message_length
      <= pluginGatewayNetwork->plugin_get_maximum_message_length()) {
#ifdef WITH_LOGGING
    log_max_msg_length_mismatch(n->logger,
                                n->logger->log_level,
                                pluginGatewayNetwork->plugin_cfg->forwarder_maximum_message_length,
                                pluginGatewayNetwork->plugin_get_maximum_message_length());
#endif
    return -1;
  }
  return pluginGatewayNetwork->plugin_network_init(pluginGatewayNetwork->plugin_cfg,
                                                   pluginGatewayNetwork->plugin_context);
}

void GatewayLinuxPluginDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;

  pluginGatewayNetwork->plugin_network_disconnect(pluginGatewayNetwork->plugin_cfg,
                                                  pluginGatewayNetwork->plugin_context);
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
                           pluginGatewayNetwork->plugin_get_short_network_protocol_name(), NULL,
                           n->gateway_network_address);
  }
#endif
  if (dlclose(pluginGatewayNetwork->dl_handle)) {
    error = dlerror();
    if (error != NULL) {
#ifdef WITH_LOGGING
      log_dlerror(n->logger, n->logger->log_level, error);
#endif
    }
  }
}

int GatewayLinuxPluginConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  if (pluginGatewayNetwork->plugin_network_connect(pluginGatewayNetwork->plugin_cfg,
                                                   pluginGatewayNetwork->plugin_context)) {
    return -1;
  }
#ifdef WITH_LOGGING
  if (n->logger) {
    log_network_connect(n->logger,
                        n->logger->log_level,
                        pluginGatewayNetwork->plugin_get_short_network_protocol_name(), NULL,
                        n->gateway_network_address);
  }
#endif
  return 0;
}

int GatewayLinuxPluginSend(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *sendBuffer,
                           int timeout_ms,
                           void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;

  MqttSnMessageData gatewaySendMessageData = {0};
  if (pop(sendBuffer, &gatewaySendMessageData) != 0) {
    return 0;
  }
  if (pluginGatewayNetwork->plugin_get_maximum_message_length() <= gatewaySendMessageData.data_length) {
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
#ifdef WITH_DEBUG_LOGGING
  log_send_gateway_message(n->logger,
                           n->logger->log_level,
                           &gatewaySendMessageData.address,
                           n->gateway_network_address,
                           gatewaySendMessageData.data,
                           gatewaySendMessageData.data_length);
#endif
  gateway_plugin_device_address dest = {
      .bytes = gatewaySendMessageData.address.bytes,
      .length = sizeof(device_address)};
  const gateway_plugin_message message = {
      .address = dest,
      .data = gatewaySendMessageData.data,
      .data_length = gatewaySendMessageData.data_length};
  int send_bytes = pluginGatewayNetwork->plugin_network_send(
      &message,
      timeout_ms,
      pluginGatewayNetwork->plugin_cfg,
      pluginGatewayNetwork->plugin_context);

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

int GatewayLinuxPluginReceive(MqttSnGatewayNetworkInterface *n,
                              MqttSnFixedSizeRingBuffer *receiveBuffer,
                              int timeout_ms,
                              void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;

  MqttSnMessageData receiveMessageData = {0};

  gateway_plugin_device_address rec_address = {
      .length = sizeof(device_address),
      .bytes = receiveMessageData.address.bytes};
  gateway_plugin_message rec_message = {
      .address = rec_address,
      .data_length = 0,
      .data = receiveMessageData.data};

  int rc = pluginGatewayNetwork->plugin_network_receive(&rec_message,
                                                        timeout_ms,
                                                        pluginGatewayNetwork->plugin_cfg,
                                                        pluginGatewayNetwork->plugin_context);
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
    log_rec_gateway_message(n->logger,
                            n->logger->log_level,
                            n->gateway_network_address,
                            msg->data,
                            msg->data_length);
  }
#endif

  return 0;
}
