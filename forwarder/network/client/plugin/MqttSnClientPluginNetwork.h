//
// Created by SomeDude on 10.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTPLUGINNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTPLUGINNETWORK_H

#include "client_network_plugin_interface.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <MqttSnFixedSizeRingBuffer.h>
#include <MqttSnClientNetworkInterface.h>

typedef struct MqttSnClientPluginContext_ {
  void *dl_handle;
  int (*plugin_network_init)(const client_plugin_config *cfg, void **plugin_context);
  void (*plugin_network_deinit)(const client_plugin_config *cfg, void **plugin_context);
  int (*plugin_network_connect)(const client_plugin_config *cfg, void *plugin_context);
  void (*plugin_network_disconnect)(const client_plugin_config *cfg, void *plugin_context);
  int (*plugin_network_send)(const client_plugin_message *send_message,
                             int32_t timeout_ms,
                             const client_plugin_config *cfg,
                             void *plugin_context);
  int (*plugin_network_receive)(client_plugin_message *rec_message,
                                int32_t timeout_ms,
                                const client_plugin_config *cfg,
                                void *plugin_context);
  const char *(*plugin_get_short_network_protocol_name)();
  uint16_t (*plugin_get_maximum_message_length)();
  void *plugin_context;
  const client_plugin_config *plugin_cfg;

} MqttSnClientPluginContext;

int ClientLinuxPluginInit(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxPluginConnect(MqttSnClientNetworkInterface *n, void *context);

void ClientLinuxPluginDisconnect(MqttSnClientNetworkInterface *n, void *context);

int ClientLinuxPluginReceive(MqttSnClientNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *receiveBuffer,
                             int32_t timeout_ms,
                             void *context);

int ClientLinuxPluginSend(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *sendBuffer,
                          int32_t timeout_ms,
                          void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTPLUGINNETWORK_H
