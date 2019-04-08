//
// Created by SomeDude on 07.04.2019.
//

#include "MqttSnGatewayPluginNetwork.h"
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GatewayLinuxPluginInit(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  pluginGatewayNetwork->dl_handle = NULL;
  pluginGatewayNetwork->plugin_context = NULL;

  n->gateway_network_init = GatewayLinuxPluginInit;
  n->gateway_receive = GatewayLinuxPluginReceive;
  n->gateway_send = GatewayLinuxPluginSend;
  n->gateway_network_connect = GatewayLinuxPluginConnect;
  n->gateway_network_disconnect = GatewayLinuxPluginDisconnect;

  pluginGatewayNetwork->dl_handle = dlopen(pluginGatewayNetwork->plugin_path, RTLD_NOW);
  if (!pluginGatewayNetwork->dl_handle) {
    fprintf(stderr, "%s\n", dlerror());
    return -1;
  }
  dlerror();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_init =
      (int (*)(const plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "plugin_network_init");
#pragma GCC diagnostic pop
  char *error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_disconnect =
      (void (*)(const plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "plugin_network_disconnect");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_connect =
      (int (*)(const plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "plugin_network_connect");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_send =
      (int (*)(const plugin_message *send_message, int timeout_ms, const plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "plugin_network_send");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_receive =
      (int (*)(plugin_message *rec_message, int timeout_ms, const plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "plugin_network_receive");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_get_short_network_protocol_name =
      (const char *(*)())
          dlsym(pluginGatewayNetwork->dl_handle, "plugin_get_short_network_protocol_name");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_get_maximum_message_length =
      (uint16_t (*)())
          dlsym(pluginGatewayNetwork->dl_handle, "plugin_get_maximum_message_length");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "%s\n", error);
    return -1;
  }

  if (strcmp(pluginGatewayNetwork->plugin_cfg->protocol, pluginGatewayNetwork->plugin_get_short_network_protocol_name())
      != 0) {
    fprintf(stderr, "gateway plugin protocol does not match gateway network protocol\n");
    return -1;
  }
  if (pluginGatewayNetwork->plugin_cfg->forwarder_maximum_message_length
      <= pluginGatewayNetwork->plugin_get_maximum_message_length()) {
    fprintf(stderr, "maximum plugin message length is greater than forwarder maximum message length\n");
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
    fprintf(stderr, "%s\n", error);
  }
  dlclose(pluginGatewayNetwork->dl_handle);
}

int GatewayLinuxPluginConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  return pluginGatewayNetwork->plugin_network_connect(pluginGatewayNetwork->plugin_cfg,
                                                      pluginGatewayNetwork->plugin_context);
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
    // TODO log it
    return 0;
  }

  plugin_device_address dest = {
      .bytes = gatewaySendMessageData.address.bytes,
      .length = sizeof(device_address)};
  const plugin_message message = {
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

  plugin_device_address rec_address = {
      .length = sizeof(device_address),
      .bytes = receiveMessageData.address.bytes};
  plugin_message rec_message = {
      .address = rec_address,
      .data_length = 0,
      .data = receiveMessageData.data};

  int rc = pluginGatewayNetwork->plugin_network_receive(&rec_message,
                                                        timeout_ms,
                                                        pluginGatewayNetwork->plugin_cfg,
                                                        pluginGatewayNetwork->plugin_context);
  if (rc == -1) {
    return -1;
  }
  if (rc == 1) {
    // no message received
    return 0;
  }

  put(receiveBuffer, &receiveMessageData);
  return 0;
}