//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_PUB_LINUX_STARTER_INTERACTIVE_CLIENT_STARTER_H_
#define CMQTTSNFORWARDER_CLIENT_PUB_LINUX_STARTER_INTERACTIVE_CLIENT_STARTER_H_

#include <logging/MqttSnLoggingInterface.h>
#include <client/interactive/config/interactive_client_config.h>
#include <client/MqttSnClient.h>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PublishClient_cfg_ptr_ {
  MqttSnClient *c_ptr;
  const interactive_client_config *qcfg_ptr;
} PublishClient_cfg_ptr;

int32_t start_interactive_client(const interactive_client_config *cfg,
                                 const MqttSnLogger *logger,
                                 MqttSnClient *interactive_client,
                                 void *gatewayNetworkContext);
int32_t start_interactive_client_plugin(const interactive_client_config *cfg,
                                        MqttSnClient *interactive_client,
                                        const MqttSnLogger *logger,
                                        void *gatewayNetworkContext);

int32_t start_interactive_client_udp(const interactive_client_config *cfg,
                                     MqttSnClient *interactive_client,
                                     const MqttSnLogger *logger,
                                     void *gatewayNetworkContext);
int32_t start_interactive_client_tcp(const interactive_client_config *cfg,
                                     MqttSnClient *interactive_client,
                                     const MqttSnLogger *logger,
                                     void *gatewayNetworkContext);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_PUB_LINUX_STARTER_INTERACTIVE_CLIENT_STARTER_H_
