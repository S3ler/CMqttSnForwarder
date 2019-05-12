//
// Created by SomeDude on 10.04.2019.
//
#include "MqttSnClientPluginNetwork.h"
#include <dlfcn.h>
#include <string.h>
#ifdef WITH_LOGGING
#include <logging/MqttSnLoggingInterface.h>
#include <network/linux/shared/plugin/logging/MqttSnPluginLogger.h>
#include <network/shared/client/logging/MqttSnDebugMessageLogging.h>
#endif

int ClientLinuxPluginInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  pluginClientNetwork->dl_handle = NULL;
  pluginClientNetwork->plugin_context = NULL;

  n->client_network_init = ClientLinuxPluginInit;
  n->client_network_receive = ClientLinuxPluginReceive;
  n->client_network_send = ClientLinuxPluginSend;
  n->client_network_connect = ClientLinuxPluginConnect;
  n->client_network_disconnect = ClientLinuxPluginDisconnect;

  char *error = NULL;

  pluginClientNetwork->dl_handle = dlopen(pluginClientNetwork->plugin_cfg->plugin_path, RTLD_NOW);
  if (!pluginClientNetwork->dl_handle) {
#ifdef WITH_LOGGING
    error = dlerror();
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }
  dlerror();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginClientNetwork->plugin_get_short_network_protocol_name =
      (const char *(*)())
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_get_short_network_protocol_name");
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
  pluginClientNetwork->plugin_get_maximum_message_length =
      (uint16_t (*)())
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_get_maximum_message_length");
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
  pluginClientNetwork->plugin_network_init =
      (int (*)(const client_plugin_config *cfg, void **plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_init");
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
  pluginClientNetwork->plugin_network_deinit =
      (void (*)(const client_plugin_config *cfg, void **plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_deinit");
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
  pluginClientNetwork->plugin_network_connect =
      (int (*)(const client_plugin_config *cfg, void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_connect");
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
  pluginClientNetwork->plugin_network_disconnect =
      (void (*)(const client_plugin_config *cfg, void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_disconnect");
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
  pluginClientNetwork->plugin_network_send =
      (int (*)(const client_plugin_message *send_message,
               int timeout_ms,
               const client_plugin_config *cfg,
               void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_send");
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
  pluginClientNetwork->plugin_network_receive =
      (int (*)(client_plugin_message *rec_message,
               int timeout_ms,
               const client_plugin_config *cfg,
               void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_receive");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
    return -1;
  }

  if (strcmp(pluginClientNetwork->plugin_cfg->protocol, pluginClientNetwork->plugin_get_short_network_protocol_name())
      != 0) {
#ifdef WITH_LOGGING
    log_protocol_mismatch(n->logger,
                          pluginClientNetwork->plugin_cfg->protocol,
                          pluginClientNetwork->plugin_get_short_network_protocol_name());
#endif
    return -1;
  }
  if (pluginClientNetwork->plugin_cfg->forwarder_maximum_message_length
      < pluginClientNetwork->plugin_get_maximum_message_length()) {
#ifdef WITH_LOGGING
    log_max_msg_length_mismatch(n->logger,
                                n->logger->log_level,
                                pluginClientNetwork->plugin_cfg->forwarder_maximum_message_length,
                                pluginClientNetwork->plugin_get_maximum_message_length());
#endif
    return -1;
  }
  return pluginClientNetwork->plugin_network_init(pluginClientNetwork->plugin_cfg,
                                                  &pluginClientNetwork->plugin_context);
}
int ClientLinuxPluginConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  if (pluginClientNetwork->plugin_network_connect(pluginClientNetwork->plugin_cfg,
                                                  pluginClientNetwork->plugin_context)) {
#ifdef WITH_LOGGING
    log_network_connect_fail(n->logger,
                             pluginClientNetwork->plugin_get_short_network_protocol_name(),
                             "client",
                             n->client_network_address,
                             NULL);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  if (n->logger) {
    log_network_connect(n->logger,
                        pluginClientNetwork->plugin_get_short_network_protocol_name(),
                        "client",
                        n->client_network_address,
                        NULL);
  }
#endif
  return 0;
}
void ClientLinuxPluginDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  if (pluginClientNetwork->dl_handle == NULL) {
    return;
  }
  pluginClientNetwork->plugin_network_disconnect(pluginClientNetwork->plugin_cfg,
                                                 pluginClientNetwork->plugin_context);
  pluginClientNetwork->plugin_network_deinit(pluginClientNetwork->plugin_cfg,
                                             &pluginClientNetwork->plugin_context);
  char *error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, n->logger->log_level, error);
#endif
  }
#ifdef WITH_LOGGING
  if (n->logger) {
    log_network_disconnect(n->logger,
                           pluginClientNetwork->plugin_get_short_network_protocol_name(),
                           "client",
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
  pluginClientNetwork->dl_handle = NULL;
}
int ClientLinuxPluginSend(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *sendBuffer,
                          int32_t timeout_ms,
                          void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;

  MqttSnMessageData clientSendMessageData = {0};
  if (pop(sendBuffer, &clientSendMessageData) != 0) {
    return 0;
  }
  if (pluginClientNetwork->plugin_get_maximum_message_length() <= clientSendMessageData.data_length) {
    // too long messages for the protocol are ignored by the standard
#ifdef WITH_DEBUG_LOGGING
    if (log_too_long_message(n->logger,
                             &clientSendMessageData.address,
                             MQTT_SN_FORWARDER_NETWORK_CLIENT,
                             clientSendMessageData.data,
                             clientSendMessageData.data_length)) {
      return -1;
    }
#endif
    return 0;
  }

#ifdef WITH_DEBUG_LOGGING
  if (n->logger) {
    if (log_db_send_client_message(n->logger,
                                   n->mqtt_sn_gateway_address,
                                   &clientSendMessageData.address,
                                   clientSendMessageData.data,
                                   clientSendMessageData.data_length)) {
      return -1;
    }
  }
#endif

  client_plugin_device_address dest = {
      .bytes = clientSendMessageData.address.bytes,
      .length = sizeof(device_address)};
  const client_plugin_message message = {
      .address = dest,
      .data = clientSendMessageData.data,
      .data_length = &clientSendMessageData.data_length};

  int send_bytes = pluginClientNetwork->plugin_network_send(
      &message,
      timeout_ms,
      pluginClientNetwork->plugin_cfg,
      pluginClientNetwork->plugin_context);

  if (send_bytes == -1) {
    return -1;
  }
  if (send_bytes != clientSendMessageData.data_length) {
    put(sendBuffer, &clientSendMessageData);
#ifdef WITH_DEBUG_LOGGING
    log_incomplete_client_message(n->logger,
                                  &clientSendMessageData.address,
                                  clientSendMessageData.data,
                                  clientSendMessageData.data_length);
#endif
    return -1;
  }

  return 0;
}
int ClientLinuxPluginReceive(MqttSnClientNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *receiveBuffer,
                             int32_t timeout_ms,
                             void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;

  MqttSnMessageData receiveMessageData = {0};

  client_plugin_device_address rec_address = {
      .length = 0,
      .bytes = receiveMessageData.address.bytes};
  client_plugin_message rec_message = {
      .address = rec_address,
      .data_length = &receiveMessageData.data_length,
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
    log_db_rec_client_message(n->logger,
                              &msg->address,
                              n->mqtt_sn_gateway_address,
                              msg->data,
                              msg->data_length);
  }
#endif

  return 0;
}
