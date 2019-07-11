//
// Created by SomeDude on 10.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTPLUGINNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTPLUGINNETWORK_H
#ifdef __cplusplus
extern "C" {
#endif

#include "client_network_plugin_interface.h"
#include <network/MqttSnClientNetworkInterface.h>

typedef struct MqttSnClientPluginContext_ {
  void *dl_handle;
  void *plugin_context;

  const char *(*plugin_get_short_network_protocol_name)(void);
  uint16_t (*plugin_get_maximum_message_length)(void);

  int32_t (*plugin_network_initialize)(const client_plugin_config *cfg, void **plugin_context);
  int32_t (*plugin_network_deinitialize)(const client_plugin_config *cfg, void **plugin_context);

  int32_t (*plugin_network_connect)(const client_plugin_config *cfg, void *plugin_context);
  int32_t (*plugin_network_disconnect)(const client_plugin_config *cfg, void *plugin_context);

  int32_t (*plugin_network_send)(const client_plugin_send_message *send_message,
                                 int32_t timeout_ms,
                                 const client_plugin_config *cfg,
                                 void *plugin_context);
  int32_t (*plugin_network_receive)(client_plugin_receive_message *rec_message,
                                    int32_t timeout_ms,
                                    const client_plugin_config *cfg,
                                    void *plugin_context);

  const client_plugin_config *plugin_cfg;

#ifdef __cplusplus
  MqttSnClientPluginContext_(client_plugin_config* plugin_cfg) :  plugin_cfg(plugin_cfg) {}
#endif
} MqttSnClientPluginContext;

int32_t ClientLinuxPluginInitialize(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientLinuxPluginDeinitialize(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientLinuxPluginConnect(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientLinuxPluginDisconnect(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientLinuxPluginSend(MqttSnClientNetworkInterface *n,
                              const device_address *from,
                              const device_address *to,
                              const uint8_t *data,
                              uint16_t data_length,
                              uint8_t signal_strength,
                              int32_t timeout_ms,
                              void *context);
int32_t ClientLinuxPluginReceive(MqttSnClientNetworkInterface *n,
                                 device_address *from,
                                 device_address *to,
                                 uint8_t *data,
                                 uint16_t max_data_length,
                                 uint8_t *signal_strength,
                                 int32_t timeout_ms,
                                 void *context);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNCLIENTPLUGINNETWORK_H
