//
// Created by SomeDude on 15.04.2019.
//

#include "gateway_plugin_interface.h"

const char *gateway_plugin_get_short_network_protocol_name() {
  return "redis";
}

uint16_t gateway_plugin_get_maximum_message_length() {
  return UINT16_MAX;
}

int gateway_plugin_network_init(const gateway_plugin_config *cfg, void *plugin_context) {

  return -1;
}

int gateway_plugin_network_deinit(const gateway_plugin_config *cfg, void *plugin_context) {

  return -1;
}

void gateway_plugin_network_disconnect(const gateway_plugin_config *cfg, void *plugin_context) {

}

int gateway_plugin_network_connect(const gateway_plugin_config *cfg, void *plugin_context) {

  return -1;
}

int gateway_plugin_network_receive(gateway_plugin_message *rec_message,
                                   int timeout_ms,
                                   const gateway_plugin_config *cfg,
                                   void *plugin_context) {

  return -1;
}

int gateway_plugin_network_send(const gateway_plugin_message *send_message,
                                int timeout_ms,
                                const gateway_plugin_config *cfg,
                                void *plugin_context) {

  return -1;
}
