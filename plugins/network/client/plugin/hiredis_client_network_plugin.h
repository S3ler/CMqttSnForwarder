//
// Created by SomeDude on 15.04.2019.
//

#ifndef CMQTTSNFORWARDER_PLUGINS_NETWORK_CLIENT_PLUGIN_HIREDIS_CLIENT_NETWORK_PLUGIN_H_
#define CMQTTSNFORWARDER_PLUGINS_NETWORK_CLIENT_PLUGIN_HIREDIS_CLIENT_NETWORK_PLUGIN_H_

#include "client_network_plugin_interface.h"
#include <hiredis.h>

typedef struct hiredis_client_context_ {
  int8_t status;
  redisContext *redis_context;
  char *redis_send_list;
  char *redis_receive_list;
} hiredis_client_context;

#endif //CMQTTSNFORWARDER_PLUGINS_NETWORK_CLIENT_PLUGIN_HIREDIS_CLIENT_NETWORK_PLUGIN_H_

