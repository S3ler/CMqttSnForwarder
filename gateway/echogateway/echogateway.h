//
// Created by SomeDude on 30.05.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_ECHOGATEWAY_H_
#define CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_ECHOGATEWAY_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <config/common/mqtt_sn_version_config.h>

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
int32_t EchoGatewayDeinit(EchoGateway *egw);
int32_t EchoGatewayLoop(EchoGateway *egw);

int32_t log_echogateway_started(const MqttSnLogger *logger, const mqtt_sn_version_config *msvcfg);
int32_t log_echogateway_terminated(const MqttSnLogger *logger, const mqtt_sn_version_config *msvcfg);

#endif //CMQTTSNFORWARDER_GATEWAY_ECHOGATEWAY_ECHOGATEWAY_H_
