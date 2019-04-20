//
// Created by SomeDude on 07.04.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_PLUGIN_INTERFACE_H
#define CMQTTSNFORWARDER_GATEWAY_PLUGIN_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct gateway_plugin_device_address_ {
  uint16_t length;
  uint8_t *bytes;
} gateway_plugin_device_address;

typedef struct gateway_plugin_config_ {
  const char *plugin_path;
  const char *protocol;
  uint16_t forwarder_maximum_message_length;
  uint16_t gateway_plugin_device_address_length;
  const gateway_plugin_device_address *mqtt_sn_gateway_network_address;
  const gateway_plugin_device_address *forwarder_gateway_network_address;
} gateway_plugin_config;

typedef struct gateway_plugin_message_ {
  gateway_plugin_device_address address;
  uint8_t *data;
  uint16_t data_length;
} gateway_plugin_message;

int gateway_plugin_network_init(const gateway_plugin_config *cfg, void **plugin_context);

void gateway_plugin_network_deinit(const gateway_plugin_config *cfg, void **plugin_context);

const char *gateway_plugin_get_short_network_protocol_name();

uint16_t gateway_plugin_get_maximum_message_length();

void gateway_plugin_network_disconnect(const gateway_plugin_config *cfg, void *plugin_context);

int gateway_plugin_network_connect(const gateway_plugin_config *cfg, void *plugin_context);

int gateway_plugin_network_receive(gateway_plugin_message *rec_message,
                                   int timeout_ms,
                                   const gateway_plugin_config *cfg,
                                   void *plugin_context);

int gateway_plugin_network_send(const gateway_plugin_message *send_message,
                                int timeout_ms,
                                const gateway_plugin_config *cfg,
                                void *plugin_context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_GATEWAY_PLUGIN_INTERFACE_H
