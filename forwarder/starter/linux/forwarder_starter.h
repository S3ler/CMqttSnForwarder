//
// Created by SomeDude on 07.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_STARTER_H
#define CMQTTSNFORWARDER_FORWARDER_STARTER_H

#include <netdb.h>
#include <forwarder/MqttSnForwarder.h>
#include <forwarder/config/forwarder_config.h>

typedef struct MqttSnForwarder_fcfg_ptr_ {
  MqttSnForwarder *mqttSnForwarder_ptr;
  const forwarder_config *fcfg_ptr;
} MqttSnForwarder_fcfg_ptr;


int convert_hostname_port_to_device_address(const char *hostname,
                                            int port,
                                            device_address *address,
                                            const char *address_name);

int get_device_address_from_hostname(const char *hostname, device_address *dst);
int get_device_address_from_addrinfo(struct addrinfo *ai_addr, device_address *dst);


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
