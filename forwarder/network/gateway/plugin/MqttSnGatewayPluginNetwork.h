//
// Created by SomeDude on 07.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYPLUGINNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYPLUGINNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MqttSnGatewayNetworkInterface.h"
#include "gateway_plugin_interface.h"

typedef struct MqttSnGatewayPluginContext_ {
  void *dl_handle;
  int (*plugin_network_init)(const gateway_plugin_config *cfg, void **plugin_context);
  int (*plugin_network_deinit)(const gateway_plugin_config *cfg, void **plugin_context);
  int (*plugin_network_connect)(const gateway_plugin_config *cfg, void *plugin_context);
  void (*plugin_network_disconnect)(const gateway_plugin_config *cfg, void *plugin_context);
  int (*plugin_network_send)(const gateway_plugin_message *send_message,
                             int32_t timeout_ms,
                             const gateway_plugin_config *cfg,
                             void *plugin_context);
  int (*plugin_network_receive)(gateway_plugin_message *rec_message,
                                int32_t timeout_ms,
                                const gateway_plugin_config *cfg,
                                void *plugin_context);
  const char *(*plugin_get_short_network_protocol_name)();
  uint16_t (*plugin_get_maximum_message_length)();
  void *plugin_context;
  const gateway_plugin_config *plugin_cfg;

} MqttSnGatewayPluginContext;

int GatewayLinuxPluginInit(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxPluginConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context);

void GatewayLinuxPluginDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int GatewayLinuxPluginReceive(MqttSnGatewayNetworkInterface *n,
                              MqttSnFixedSizeRingBuffer *receiveBuffer,
                              int32_t timeout_ms,
                              void *context);

int GatewayLinuxPluginSend(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *sendBuffer,
                           int32_t timeout_ms,
                           void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYPLUGINNETWORK_H
