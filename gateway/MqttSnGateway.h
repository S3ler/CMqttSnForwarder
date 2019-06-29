//
// Created by SomeDude on 20.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAY_H_
#define CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAY_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <gateway/database/db_handler.h>
#include <config/gateway/gateway_advertisment_config.h>
#include "MqttClient.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct MqttSnGateway_ {
  db_handler db_handler_;
  MqttClient *mqttClient;
  MqttSnFixedSizeRingBuffer mqttClientNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer mqttClientNetworkSendBuffer;

  MqttSnClientNetworkInterface clientNetwork;
  MqttSnFixedSizeRingBuffer clientNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer clientNetworkSendBuffer;
  int32_t clientNetworkSendTimeout;
  int32_t clientNetworkReceiveTimeout;
  void *clientNetworkContext;
#ifdef WITH_LOGGING
  MqttSnLogger logger;
#endif

  // TODO gateway configs
  gateway_advertise_config advertisement_config;

  uint64_t last_client_timeout_check;
  uint64_t client_timeout_check_period;
  int32_t connection_timeout_offset;
} MqttSnGateway;

int32_t MqttSnGatewayInitialize(MqttSnGateway *mqttSnGateway,
                                const MqttSnLogger *logger,
                                MqttClient *mqttClient,
                                void *clientNetworkContext,
                                const gateway_advertise_config *gacfg);
int32_t MqttSnGatewayDeinitialize(MqttSnGateway *mqttSnGateway);

int32_t MqttSnGatewayConnect(MqttSnGateway *mqttSnGateway);
int32_t MqttSnGatewayDisconnect(MqttSnGateway *mqttSnGateway);

int32_t MqttSnGatewayLoop(MqttSnGateway *mqttSnGateway);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAY_H_
