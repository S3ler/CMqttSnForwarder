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

int32_t ClientLinuxPluginInitialize(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  pluginClientNetwork->dl_handle = NULL;
  pluginClientNetwork->plugin_context = NULL;

  n->initialize = ClientLinuxPluginInitialize;
  n->deinitialize = ClientLinuxPluginDeinitialize;
  n->connect = ClientLinuxPluginConnect;
  n->disconnect = ClientLinuxPluginDisconnect;
  n->send = ClientLinuxPluginSend;
  n->receive = ClientLinuxPluginReceive;

  char *error = NULL;

  pluginClientNetwork->dl_handle = dlopen(pluginClientNetwork->plugin_cfg->plugin_path, RTLD_NOW);
  if (!pluginClientNetwork->dl_handle) {
#ifdef WITH_LOGGING
    error = dlerror();
    log_dlerror(n->logger, error);
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
    log_dlerror(n->logger, error);
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
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginClientNetwork->plugin_network_initialize =
      (int32_t (*)(const client_plugin_config *cfg, void **plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_initialize");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginClientNetwork->plugin_network_deinitialize =
      (int32_t (*)(const client_plugin_config *cfg, void **plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_deinitialize");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginClientNetwork->plugin_network_connect =
      (int32_t (*)(const client_plugin_config *cfg, void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_connect");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginClientNetwork->plugin_network_disconnect =
      (int32_t (*)(const client_plugin_config *cfg, void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_disconnect");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginClientNetwork->plugin_network_send =
      (int32_t (*)(const client_plugin_send_message *send_message,
                   int32_t timeout_ms,
                   const client_plugin_config *cfg,
                   void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_send");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginClientNetwork->plugin_network_receive =
      (int32_t (*)(client_plugin_receive_message *rec_message,
                   int32_t timeout_ms,
                   const client_plugin_config *cfg,
                   void *plugin_context))
          dlsym(pluginClientNetwork->dl_handle, "client_plugin_network_receive");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
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
  return pluginClientNetwork->plugin_network_initialize(pluginClientNetwork->plugin_cfg,
                                                        &pluginClientNetwork->plugin_context);
}
int32_t ClientLinuxPluginDeinitialize(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  if (pluginClientNetwork->dl_handle == NULL) {
    return 0;
  }
  pluginClientNetwork->plugin_network_deinitialize(pluginClientNetwork->plugin_cfg,
                                                   &pluginClientNetwork->plugin_context);
  char *error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  log_client_network_deinitialized(n->logger,
                                   pluginClientNetwork->plugin_get_short_network_protocol_name(),
                                   n->client_network_address);
#endif
  if (dlclose(pluginClientNetwork->dl_handle)) {
    error = dlerror();
    if (error != NULL) {
#ifdef WITH_LOGGING
      log_dlerror(n->logger, error);
#endif
      return -1;
    }
  }
  pluginClientNetwork->dl_handle = NULL;
  return 0;
}

int32_t ClientLinuxPluginConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  if (pluginClientNetwork->plugin_network_connect(pluginClientNetwork->plugin_cfg,
                                                  pluginClientNetwork->plugin_context)) {
#ifdef WITH_LOGGING
    log_client_network_connect_fail(n->logger,
                                    pluginClientNetwork->plugin_get_short_network_protocol_name(),
                                    n->client_network_address,
                                    NULL);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_client_network_connect(n->logger,
                             pluginClientNetwork->plugin_get_short_network_protocol_name(),
                             n->client_network_address,
                             NULL);
#endif
  return 0;
}

int32_t ClientLinuxPluginDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;
  pluginClientNetwork->plugin_network_disconnect(pluginClientNetwork->plugin_cfg,
                                                 pluginClientNetwork->plugin_context);

  char *error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_client_network_disconnect(n->logger,
                                pluginClientNetwork->plugin_get_short_network_protocol_name(),
                                n->client_network_address);
#endif
  pluginClientNetwork->dl_handle = NULL;
  return 0;
}

int32_t ClientLinuxPluginSend(MqttSnClientNetworkInterface *n,
                              const device_address *from,
                              const device_address *to,
                              const uint8_t *data,
                              uint16_t data_length,
                              uint8_t signal_strength,
                              int32_t timeout_ms,
                              void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;

  if (pluginClientNetwork->plugin_get_maximum_message_length() <= data_length) {
    // too long messages for the protocol are ignored by the standard
#ifdef WITH_DEBUG_LOGGING
    log_too_long_message(n->logger, from, MQTT_SN_FORWARDER_NETWORK_CLIENT, data, data_length);
#endif
    return data_length;
  }

#ifdef WITH_DEBUG_LOGGING
  log_db_send_client_message(n->logger, from, to, data, data_length);
#endif

  const client_plugin_send_message msg = {
      .from = {.bytes = from->bytes, .length = sizeof(device_address)},
      .to = {.bytes = to->bytes, .length = sizeof(device_address)},
      .data = data,
      .data_length = data_length,
      .signal_strength = signal_strength};

  int send_rc = pluginClientNetwork->plugin_network_send(&msg,
                                                         timeout_ms,
                                                         pluginClientNetwork->plugin_cfg,
                                                         pluginClientNetwork->plugin_context);

#ifdef WITH_DEBUG_LOGGING
  if (send_rc > 0 && send_rc != data_length) {
    log_incomplete_client_message(n->logger, from, data, data_length);
  }
#endif

  return send_rc;
}

int32_t ClientLinuxPluginReceive(MqttSnClientNetworkInterface *n,
                                 device_address *from,
                                 device_address *to,
                                 uint8_t *data,
                                 uint16_t max_data_length,
                                 uint8_t *signal_strength,
                                 int32_t timeout_ms,
                                 void *context) {
  MqttSnClientPluginContext *pluginClientNetwork = (MqttSnClientPluginContext *) context;

  client_plugin_receive_message rec_msg = {
      .from = {.length = 0, .bytes = from->bytes},
      .to = {.length = 0, .bytes = to->bytes},
      .data = data,
      .max_data_length = max_data_length,
      .signal_strength = signal_strength};

  int32_t receive_rc = pluginClientNetwork->plugin_network_receive(&rec_msg,
                                                                   timeout_ms,
                                                                   pluginClientNetwork->plugin_cfg,
                                                                   pluginClientNetwork->plugin_context);
#ifdef WITH_DEBUG_LOGGING
  if (receive_rc > 0) {
    log_db_rec_client_message(n->logger, from, to, data, receive_rc);
  }
#endif
  return receive_rc;
}
