//
// Created by SomeDude on 10.04.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_NETWORK_PLUGIN_INTERFACE_H
#define CMQTTSNFORWARDER_CLIENT_NETWORK_PLUGIN_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct client_plugin_device_address_ {
  uint16_t length;
  uint8_t *bytes;
} client_plugin_device_address;

typedef struct client_plugin_config_ {
  const char *plugin_path;
  const char *protocol;
  const uint16_t forwarder_maximum_message_length;
  const uint16_t client_plugin_device_address_length;
  const client_plugin_device_address *forwarder_client_network_address;
} client_plugin_config;

typedef struct client_plugin_message_ {
  client_plugin_device_address address;
  uint8_t *data;
  uint16_t *data_length;
} client_plugin_message;

const char *client_plugin_get_short_network_protocol_name();
uint16_t client_plugin_get_maximum_message_length();

int client_plugin_network_init(const client_plugin_config *cfg, void **plugin_context);
int client_plugin_network_connect(const client_plugin_config *cfg, void *plugin_context);
void client_plugin_network_disconnect(const client_plugin_config *cfg, void *plugin_context);
void client_plugin_network_deinit(const client_plugin_config *cfg, void **plugin_context);

int client_plugin_network_send(const client_plugin_message *send_message,
                               int timeout_ms,
                               const client_plugin_config *cfg,
                               void *plugin_context);
int client_plugin_network_receive(client_plugin_message *rec_message,
                                  int timeout_ms,
                                  const client_plugin_config *cfg,
                                  void *plugin_context);
#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_CLIENT_NETWORK_PLUGIN_INTERFACE_H
