//
// Created by SomeDude on 07.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_STARTER_H
#define CMQTTSNFORWARDER_FORWARDER_STARTER_H

#include <MqttSnForwarder.h>
#include "forwarder_config.h"
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <network/plugin/plugin_interface.h>


int convert_string_to_device_address(const char *string, device_address *address);

int start_gateway_udp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext);

int start_gateway_tcp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext);

int start_gateway_plugin(const forwarder_config *fcfg,
                         MqttSnForwarder *mqttSnForwarder,
                         void *gatewayNetworkContext,
                         void *clientNetworkContext);

int start_client_udp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext);

int start_client_tcp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext);

int start_client_plugin(const forwarder_config *fcfg,
                        MqttSnForwarder *mqttSnForwarder,
                        void *gatewayNetworkContext,
                        void *clientNetworkContext);

int start_forwarder(const forwarder_config *fcfg,
                    MqttSnForwarder *mqttSnForwarder,
                    void *gatewayNetworkContext,
                    void *clientNetworkContext);

#endif //CMQTTSNFORWARDER_FORWARDER_STARTER_H
