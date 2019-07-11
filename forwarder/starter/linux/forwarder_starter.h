//
// Created by SomeDude on 07.04.2019.
//
#ifndef CMQTTSNFORWARDER_FORWARDER_STARTER_H
#define CMQTTSNFORWARDER_FORWARDER_STARTER_H

#include <netdb.h>
#include <forwarder/MqttSnForwarder.h>
#include <forwarder/config/forwarder_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnForwarder_fcfg_ptr_ {
  MqttSnForwarder *mqttSnForwarder_ptr;
  const forwarder_config *fcfg_ptr;
} MqttSnForwarder_fcfg_ptr;



int start_gateway_udp(const forwarder_config *cfg,
                      const MqttSnLogger *logger,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext);

int start_gateway_tcp(const forwarder_config *cfg,
                      const MqttSnLogger *logger,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext);

int start_gateway_plugin(const forwarder_config *cfg,
                         const MqttSnLogger *logger,
                         MqttSnForwarder *mqttSnForwarder,
                         void *gatewayNetworkContext,
                         void *clientNetworkContext);

int start_client_udp(const forwarder_config *fcfg,
                     const MqttSnLogger *logger,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext);

int start_client_tcp(const forwarder_config *fcfg,
                     const MqttSnLogger *logger,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext);

int start_client_plugin(const forwarder_config *fcfg,
                        const MqttSnLogger *logger,
                        MqttSnForwarder *mqttSnForwarder,
                        void *gatewayNetworkContext,
                        void *clientNetworkContext);

int start_forwarder(const forwarder_config *fcfg,
                    const MqttSnLogger *logger,
                    MqttSnForwarder *mqttSnForwarder,
                    void *gatewayNetworkContext,
                    void *clientNetworkContext);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_STARTER_H
