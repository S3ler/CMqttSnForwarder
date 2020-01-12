//
// Created by SomeDude on 15.04.2019.
//

#include "hiredis_gateway_network_plugin.h"
#include "../../../shared/plugin/hiredis/hiredis_network_plugin.h"
#include "gateway_network_plugin_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *gateway_plugin_get_short_network_protocol_name() {
  return hiredis_plugin_get_short_network_protocol_name();
}

uint16_t gateway_plugin_get_maximum_message_length() {
  return hiredis_plugin_get_maximum_message_length();
}

int gateway_plugin_network_init(const gateway_plugin_config *cfg, void **plugin_context) {
  return hiredis_gateway_plugin_network_init(plugin_context);
}

void gateway_plugin_network_disconnect(const gateway_plugin_config *cfg, void *plugin_context) {
  hiredis_plugin_network_disconnect(plugin_context);
}

void gateway_plugin_network_deinit(const gateway_plugin_config *cfg, void **plugin_context) {
  hiredis_plugin_network_deinit(plugin_context);
}

int gateway_plugin_network_connect(const gateway_plugin_config *cfg, void *plugin_context) {
  return hiredis_plugin_network_connect(cfg->forwarder_gateway_network_address->bytes,
                                        cfg->forwarder_gateway_network_address->length,
                                        cfg->gateway_plugin_device_address_length,
                                        plugin_context);
}

int gateway_plugin_network_receive(gateway_plugin_message *rec_message,
                                   int timeout_ms,
                                   const gateway_plugin_config *cfg,
                                   void *plugin_context) {

  return hiredis_plugin_network_receive(rec_message->address.bytes,
                                        &rec_message->address.length,
                                        cfg->gateway_plugin_device_address_length,
                                        rec_message->data,
                                        rec_message->data_length,
                                        cfg->forwarder_maximum_message_length,
                                        timeout_ms,
                                        plugin_context);
}

int gateway_plugin_network_send(const gateway_plugin_message *send_message,
                                int timeout_ms,
                                const gateway_plugin_config *cfg,
                                void *plugin_context) {
  return hiredis_plugin_network_send(send_message->address.bytes,
                                     send_message->address.length,
                                     cfg->gateway_plugin_device_address_length,
                                     send_message->data,
                                     *send_message->data_length,
                                     cfg->forwarder_maximum_message_length,
                                     timeout_ms,
                                     plugin_context);
}
