//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_LINUX_STARTER_PUBLISH_CLIENT_STARTER_H_
#define CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_LINUX_STARTER_PUBLISH_CLIENT_STARTER_H_

#include <logging/MqttSnLoggingInterface.h>
#include <client/MqttSnFindGatewayClient.h>
#include <client/findgatewaymonitor/config/find_gateway_monitor_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FindGatewayMonitor_cfg_ptr_ {
  MqttSnFindGatewayClient *c_ptr;
  const find_gateway_monitor_config *cfg_ptr;
} FindGatewayMonitor_cfg_ptr;

int32_t start_find_gateway_monitor(const find_gateway_monitor_config *cfg,
                                   MqttSnFindGatewayClient *client,
                                   const MqttSnLogger *logger,
                                   MqttSnGatewayNetworkInterface *gatewayNetwork,
                                   void *gatewayNetworkContext);
int32_t start_find_gateway_client_plugin(const find_gateway_monitor_config *cfg,
                                         MqttSnFindGatewayClient *publish_client,
                                         const MqttSnLogger *logger,
                                         MqttSnGatewayNetworkInterface *gatewayNetwork,
                                         void *gatewayNetworkContext);
int32_t start_find_gateway_client_udp(const find_gateway_monitor_config *cfg,
                                      MqttSnFindGatewayClient *publish_client,
                                      const MqttSnLogger *logger,
                                      MqttSnGatewayNetworkInterface *gatewayNetwork,
                                      void *gatewayNetworkContext);
int32_t start_find_gateway_client_tcp(const find_gateway_monitor_config *cfg,
                                      MqttSnFindGatewayClient *publish_client,
                                      const MqttSnLogger *logger,
                                      MqttSnGatewayNetworkInterface *gatewayNetwork,
                                      void *gatewayNetworkContext);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_FINDGATEWAYMONITOR_LINUX_STARTER_PUBLISH_CLIENT_STARTER_H_
