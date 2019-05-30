//
// Created by SomeDude on 30.05.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_LINUX_STARTER_ECHOGATEWAY_STARTER_H_
#define CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_LINUX_STARTER_ECHOGATEWAY_STARTER_H_

#include <gateway/echogateway/config/echogateway_config.h>
#include <gateway/echogateway/echogateway.h>

typedef struct EchoGateway_fcfg_ptr_ {
  EchoGateway *egw_ptr;
  const echogateway_config *egw_cfg;
} EchoGateway_fcfg_ptr;

int start_echogateway(const echogateway_config *fcfg,
                      const MqttSnLogger *logger,
                      EchoGateway *echo_gateway,
                      void *clientNetworkContext);

int start_echogateway_client_plugin(const echogateway_config *fcfg,
                                    const MqttSnLogger *logger,
                                    EchoGateway *echo_gateway,
                                    void *clientNetworkContext);

int start_echogateway_client_udp(const echogateway_config *fcfg,
                                 const MqttSnLogger *logger,
                                 EchoGateway *gw,
                                 void *clientNetworkContext);

int start_echogateway_client_tcp(const echogateway_config *fcfg,
                                 const MqttSnLogger *logger,
                                 EchoGateway *echo_gateway,
                                 void *clientNetworkContext);



#endif //CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_LINUX_STARTER_ECHOGATEWAY_STARTER_H_
