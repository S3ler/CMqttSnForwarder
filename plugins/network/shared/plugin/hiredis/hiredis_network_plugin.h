//
// Created by SomeDude on 19.04.2019.
//

#ifndef CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_HIREDIS_HIREDIS_NETWORK_PLUGIN_H_
#define CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_HIREDIS_HIREDIS_NETWORK_PLUGIN_H_

#include <hiredis.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct hiredis_context_ {
  int8_t status;
  redisContext *redis_context;
  char *redis_send_list;
  char *redis_receive_list;
} hiredis_context;

const char *hiredis_plugin_get_short_network_protocol_name();

uint16_t hiredis_plugin_get_maximum_message_length();

int hiredis_gateway_plugin_network_init(void **plugin_context);

int hiredis_client_plugin_network_init(void **plugin_context);

void hiredis_plugin_network_disconnect(void *plugin_context);

void hiredis_plugin_network_deinit(void **plugin_context);

int hiredis_plugin_network_connect(const uint8_t *address,
                                   uint16_t address_len,
                                   uint16_t network_max_device_address_length,
                                   void *plugin_context);

int hiredis_plugin_network_send(const uint8_t *to,
                                uint16_t to_len,
                                uint16_t network_max_device_address_length,
                                const uint8_t *data,
                                uint16_t data_len,
                                uint16_t network_max_message_length,
                                int timeout_ms,
                                void *plugin_context);

int hiredis_plugin_network_receive(uint8_t *from,
                                   uint16_t *from_len,
                                   uint16_t network_max_device_address_length,
                                   uint8_t *data,
                                   uint16_t *data_len,
                                   uint16_t network_max_data_length,
                                   int timeout_ms,
                                   void *plugin_context);

#endif //CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_HIREDIS_HIREDIS_NETWORK_PLUGIN_H_
