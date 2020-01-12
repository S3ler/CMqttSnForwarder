//
// Created by SomeDude on 18.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_STARTER_LINUX_GATEWAY_STARTER_H_
#define CMQTTSNFORWARDER_GATEWAY_STARTER_LINUX_GATEWAY_STARTER_H_

#include <logging/MqttSnLogging.h>
#include <gateway/MqttSnGateway.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnGateway_fcfg_ptr_ {
  MqttSnGateway *msgw_ptr;
  const mqtt_sn_gateway__config *msgw_cfg;
} MqttSnGateway_fcfg_ptr;

int start_mqtt_sn_gateway(const mqtt_sn_gateway__config *fcfg,
                          const MqttSnLogger *logger,
                          MqttSnGateway *mqtt_sn_gateway,
                          void *clientNetworkContext);
int start_mqtt_sn_gateway_plugin(const mqtt_sn_gateway__config *fcfg,
                                 const MqttSnLogger *logger,
                                 MqttSnGateway *mqtt_sn_gateway,
                                 void *clientNetworkContext);
int start_mqtt_sn_gateway_client_udp(const mqtt_sn_gateway__config *fcfg,
                                     const MqttSnLogger *logger,
                                     MqttSnGateway *echo_gateway,
                                     void *clientNetworkContext);
int start_mqtt_sn_gateway_tcp(const mqtt_sn_gateway__config *fcfg,
                              const MqttSnLogger *logger,
                              MqttSnGateway *mqtt_sn_gateway,
                              void *clientNetworkContext);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_STARTER_LINUX_GATEWAY_STARTER_H_
