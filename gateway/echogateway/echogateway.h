//
// Created by SomeDude on 30.05.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_ECHOGATEWAY_H_
#define CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_ECHOGATEWAY_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <config/common/mqtt_sn_version_config.h>
#include <gateway/echogateway/config/echogateway_config.h>

typedef struct EchoGateway_ {

  MqttSnClientNetworkInterface clientNetwork;
  MqttSnFixedSizeRingBuffer clientNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer clientNetworkSendBuffer;

  void *clientNetworkContext;
  int32_t clientNetworkSendTimeout;
  int32_t clientNetworkReceiveTimeout;

#ifdef WITH_LOGGING
  MqttSnLogger logger;
#endif

} EchoGateway;

int32_t EchoGatewayInit(EchoGateway *egw, log_level_t log_level, void *clientNetworkContext);
int32_t EchoGatewayConnect(EchoGateway *egw);
int32_t EchoGatewayDisconnect(EchoGateway *egw);
int32_t EchoGatewayDeinit(EchoGateway *egw);
int32_t EchoGatewayLoop(EchoGateway *egw);

#endif //CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_ECHOGATEWAY_H_
