//
// Created by SomeDude on 07.04.2019.
//

#include "MqttSnGatewayPluginNetwork.h"
#include <dlfcn.h>
#include <string.h>
#include <network/linux/shared/plugin/logging/MqttSnPluginLogger.h>
#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.h>

int32_t GatewayLinuxPluginInitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  pluginGatewayNetwork->dl_handle = NULL;
  pluginGatewayNetwork->plugin_context = NULL;

  n->initialize = GatewayLinuxPluginInitialize;
  n->deinitialize = GatewayLinuxPluginDeinitialize;
  n->connect = GatewayLinuxPluginConnect;
  n->disconnect = GatewayLinuxPluginDisconnect;
  n->send = GatewayLinuxPluginSend;
  n->receive = GatewayLinuxPluginReceive;

  char *error = NULL;

  pluginGatewayNetwork->dl_handle = dlopen(pluginGatewayNetwork->plugin_cfg->plugin_path, RTLD_NOW);
  if (!pluginGatewayNetwork->dl_handle) {
#ifdef WITH_LOGGING
    error = dlerror();
    log_dlerror(n->logger, error);
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
    log_dlerror(n->logger, error);
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
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  pluginGatewayNetwork->plugin_network_initialize =
      (int32_t (*)(const gateway_plugin_config *cfg, void **plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_initialize");
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
  pluginGatewayNetwork->plugin_network_deinitialize =
      (int32_t (*)(const gateway_plugin_config *cfg, void **plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_deinitialize");
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
  pluginGatewayNetwork->plugin_network_connect =
      (int32_t (*)(const gateway_plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_connect");
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
  pluginGatewayNetwork->plugin_network_disconnect =
      (int32_t (*)(const gateway_plugin_config *cfg, void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_disconnect");
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
  pluginGatewayNetwork->plugin_network_send =
      (int32_t (*)(const gateway_plugin_send_message *send_message,
                   int32_t timeout_ms,
                   const gateway_plugin_config *cfg,
                   void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_send");
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
  pluginGatewayNetwork->plugin_network_receive =
      (int32_t (*)(gateway_plugin_receive_message *rec_message,
                   int32_t timeout_ms,
                   const gateway_plugin_config *cfg,
                   void *plugin_context))
          dlsym(pluginGatewayNetwork->dl_handle, "gateway_plugin_network_receive");
#pragma GCC diagnostic pop
  error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }

  if (strcmp(pluginGatewayNetwork->plugin_cfg->protocol,
             pluginGatewayNetwork->plugin_get_short_network_protocol_name())
      != 0) {
#ifdef WITH_LOGGING
    log_protocol_mismatch(n->logger,
                          pluginGatewayNetwork->plugin_cfg->protocol,
                          pluginGatewayNetwork->plugin_get_short_network_protocol_name());
#endif
    return -1;
  }
  if (pluginGatewayNetwork->plugin_cfg->forwarder_maximum_message_length
      < pluginGatewayNetwork->plugin_get_maximum_message_length()) {
#ifdef WITH_LOGGING
    log_max_msg_length_mismatch(n->logger,
                                n->logger->log_level,
                                pluginGatewayNetwork->plugin_cfg->forwarder_maximum_message_length,
                                pluginGatewayNetwork->plugin_get_maximum_message_length());
#endif
    return -1;
  }
  return pluginGatewayNetwork->plugin_network_initialize(pluginGatewayNetwork->plugin_cfg,
                                                         &pluginGatewayNetwork->plugin_context);
}

int32_t GatewayLinuxPluginDeinitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  if (pluginGatewayNetwork->dl_handle == NULL) {
    return 0;
  }
  pluginGatewayNetwork->plugin_network_deinitialize(pluginGatewayNetwork->plugin_cfg,
                                                    &pluginGatewayNetwork->plugin_context);
  char *error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  log_client_network_deinitialized(n->logger,
                                   pluginGatewayNetwork->plugin_get_short_network_protocol_name(),
                                   n->gateway_network_address);
#endif
  if (dlclose(pluginGatewayNetwork->dl_handle)) {
    error = dlerror();
    if (error != NULL) {
#ifdef WITH_LOGGING
      log_dlerror(n->logger, error);
#endif
    }
  }
  pluginGatewayNetwork->dl_handle = NULL;
  return 0;
}

int32_t GatewayLinuxPluginDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  pluginGatewayNetwork->plugin_network_disconnect(pluginGatewayNetwork->plugin_cfg,
                                                  pluginGatewayNetwork->plugin_context);
  char *error = dlerror();
  if (error != NULL) {
#ifdef WITH_LOGGING
    log_dlerror(n->logger, error);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_gateway_network_disconnect(n->logger,
                                 pluginGatewayNetwork->plugin_get_short_network_protocol_name(),
                                 n->gateway_network_address);
#endif

  return 0;
}

int32_t GatewayLinuxPluginConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;
  if (pluginGatewayNetwork->plugin_network_connect(pluginGatewayNetwork->plugin_cfg,
                                                   pluginGatewayNetwork->plugin_context)) {
#ifdef WITH_LOGGING
    log_gateway_network_connect_fail(n->logger,
                                     pluginGatewayNetwork->plugin_get_short_network_protocol_name(),
                                     n->gateway_network_address,
                                     n->mqtt_sn_gateway_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_gateway_network_connect(n->logger,
                              pluginGatewayNetwork->plugin_get_short_network_protocol_name(),
                              n->gateway_network_address,
                              n->mqtt_sn_gateway_address);
#endif
  return 0;
}

int32_t GatewayLinuxPluginSend(MqttSnGatewayNetworkInterface *n,
                               const device_address *from,
                               const device_address *to,
                               const uint8_t *data,
                               uint16_t data_length,
                               uint8_t signal_strength,
                               int32_t timeout_ms,
                               void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;

  if (pluginGatewayNetwork->plugin_get_maximum_message_length() <= data_length) {
    // too long messages for the protocol are ignored by the standard
#ifdef WITH_DEBUG_LOGGING
    log_too_long_message(n->logger, from, MQTT_SN_FORWARDER_NETWORK_GATEWAY, data, data_length);
#endif
    return data_length;
  }

#ifdef WITH_DEBUG_LOGGING
  log_db_send_gateway_message(n->logger, from, to, data, data_length);
#endif

  const gateway_plugin_send_message msg = {
      .from = {.bytes = from->bytes, .length = sizeof(device_address)},
      .to = {.bytes = to->bytes, .length = sizeof(device_address)},
      .data = data,
      .data_length = data_length,
      .signal_strength = signal_strength};

  int send_rc = pluginGatewayNetwork->plugin_network_send(&msg,
                                                          timeout_ms,
                                                          pluginGatewayNetwork->plugin_cfg,
                                                          pluginGatewayNetwork->plugin_context);

#ifdef WITH_DEBUG_LOGGING
  if (send_rc > 0 && send_rc != data_length) {
    log_incomplete_gateway_message(n->logger, from, data, data_length);
  }
#endif

  return send_rc;
}

int32_t GatewayLinuxPluginReceive(MqttSnGatewayNetworkInterface *n,
                                  device_address *from,
                                  device_address *to,
                                  uint8_t *data,
                                  uint16_t max_data_length,
                                  uint8_t *signal_strength,
                                  int32_t timeout_ms,
                                  void *context) {
  MqttSnGatewayPluginContext *pluginGatewayNetwork = (MqttSnGatewayPluginContext *) context;

  gateway_plugin_receive_message rec_message = {
      .from = {.length = 0, .bytes = from->bytes},
      .to = {.length = 0, .bytes = to->bytes},
      .data = data,
      .max_data_length = max_data_length,
      .signal_strength = signal_strength};

  int receive_rc = pluginGatewayNetwork->plugin_network_receive(&rec_message,
                                                                timeout_ms,
                                                                pluginGatewayNetwork->plugin_cfg,
                                                                pluginGatewayNetwork->plugin_context);

#ifdef WITH_DEBUG_LOGGING
  if (receive_rc > 0) {
    log_db_rec_gateway_message(n->logger, from, to, data, receive_rc);
  }
#endif

  return receive_rc;
}
