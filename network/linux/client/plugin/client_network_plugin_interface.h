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
  const uint16_t length;
  const uint8_t *bytes;
} client_plugin_device_address;

typedef struct client_plugin_config_ {
  const char *plugin_path;
  const char *protocol;
  const uint16_t forwarder_maximum_message_length;
  const uint16_t client_plugin_device_address_length;
  const client_plugin_device_address *forwarder_client_network_address;
} client_plugin_config;

typedef struct client_plugin_send_device_address_ {
  const uint16_t length;
  const uint8_t *bytes;
} client_plugin_send_device_address;
typedef struct client_plugin_send_message_ {
  const client_plugin_send_device_address from;
  const client_plugin_send_device_address to;
  const uint8_t *data;
  const uint16_t data_length;
  const uint8_t signal_strength;
} client_plugin_send_message;

typedef struct client_plugin_receive_device_address_ {
  uint16_t length;
  uint8_t *bytes;
} client_plugin_receive_device_address;
typedef struct client_plugin_receive_message_ {
  client_plugin_receive_device_address from;
  client_plugin_receive_device_address to;
  uint8_t *data;
  uint16_t max_data_length;
  uint8_t *signal_strength;
} client_plugin_receive_message;

const char *client_plugin_get_short_network_protocol_name();
uint16_t client_plugin_get_maximum_message_length();

int32_t client_plugin_network_initialize(const client_plugin_config *cfg, void **plugin_context);
int32_t client_plugin_network_deinitialize(const client_plugin_config *cfg, void **plugin_context);

int32_t client_plugin_network_connect(const client_plugin_config *cfg, void *plugin_context);
int32_t client_plugin_network_disconnect(const client_plugin_config *cfg, void *plugin_context);

int32_t client_plugin_network_send(const client_plugin_send_message *send_message,
                                   int32_t timeout_ms,
                                   const client_plugin_config *cfg,
                                   void *plugin_context);
int32_t client_plugin_network_receive(client_plugin_receive_message *rec_message,
                                      int32_t timeout_ms,
                                      const client_plugin_config *cfg,
                                      void *plugin_context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_CLIENT_NETWORK_PLUGIN_INTERFACE_H
