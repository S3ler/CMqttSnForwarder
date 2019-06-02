//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_PUB_LINUX_STARTER_PUBLISH_CLIENT_STARTER_H_
#define CMQTTSNFORWARDER_CLIENT_PUB_LINUX_STARTER_PUBLISH_CLIENT_STARTER_H_

#include <logging/MqttSnLoggingInterface.h>
#include <client/pub/config/publish_client_config.h>
#include <client/MqttSnClient.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PublishClient_cfg_ptr_ {
  MqttSnClient *c_ptr;
  const publish_client_config *cfg_ptr;
} PublishClient_cfg_ptr;

int32_t start_publish_client(const publish_client_config *cfg,
                             const MqttSnLogger *logger,
                             MqttSnClient *publish_client,
                             void *gatewayNetworkContext);
int32_t start_publish_client_plugin(const publish_client_config *cfg,
                                    MqttSnClient *publish_client,
                                    const MqttSnLogger *logger,
                                    void *gatewayNetworkContext);
int32_t start_publish_client_udp(const publish_client_config *cfg,
                                 MqttSnClient *publish_client,
                                 const MqttSnLogger *logger,
                                 void *gatewayNetworkContext);
int32_t start_publish_client_tcp(const publish_client_config *cfg,
                                 MqttSnClient *publish_client,
                                 const MqttSnLogger *logger,
                                 void *gatewayNetworkContext);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_PUB_LINUX_STARTER_PUBLISH_CLIENT_STARTER_H_
