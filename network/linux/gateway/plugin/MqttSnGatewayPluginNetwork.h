//
// Created by SomeDude on 07.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYPLUGINNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYPLUGINNETWORK_H
#ifdef __cplusplus
extern "C" {
#endif

#include "gateway_network_plugin_interface.h"
#include <network/MqttSnGatewayNetworkInterface.h>

typedef struct MqttSnGatewayPluginContext_ {
  void *dl_handle;
  void *plugin_context;
  const gateway_plugin_config *plugin_cfg;

  const char *(*plugin_get_short_network_protocol_name)();
  uint16_t (*plugin_get_maximum_message_length)();

  int32_t (*plugin_network_initialize)(const gateway_plugin_config *cfg, void **plugin_context);
  int32_t (*plugin_network_deinitialize)(const gateway_plugin_config *cfg, void **plugin_context);

  int32_t (*plugin_network_connect)(const gateway_plugin_config *cfg, void *plugin_context);
  int32_t (*plugin_network_disconnect)(const gateway_plugin_config *cfg, void *plugin_context);

  int32_t (*plugin_network_send)(const gateway_plugin_send_message *send_message,
                                 int32_t timeout_ms,
                                 const gateway_plugin_config *cfg,
                                 void *plugin_context);
  int32_t (*plugin_network_receive)(gateway_plugin_receive_message *rec_message,
                                    int32_t timeout_ms,
                                    const gateway_plugin_config *cfg,
                                    void *plugin_context);
} MqttSnGatewayPluginContext;

int32_t GatewayLinuxPluginInitialize(MqttSnGatewayNetworkInterface *n, void *context);
int32_t GatewayLinuxPluginDeinitialize(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayLinuxPluginConnect(MqttSnGatewayNetworkInterface *networkInterface, void *context);
int32_t GatewayLinuxPluginDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayLinuxPluginSend(MqttSnGatewayNetworkInterface *n,
                               const device_address *from,
                               const device_address *to,
                               const uint8_t *data,
                               uint16_t data_length,
                               uint8_t signal_strength,
                               int32_t timeout_ms,
                               void *context);
int32_t GatewayLinuxPluginReceive(MqttSnGatewayNetworkInterface *n,
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
#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYPLUGINNETWORK_H
