//
// Created by SomeDude on 13.07.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYDISCOVERYHANDLER_H_
#define CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYDISCOVERYHANDLER_H_

#include <stdint.h>
#include <platform/MqttSnMessageData.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <gateway/database/db_handler.h>
#include <config/gateway/gateway_advertisment_config.h>
#include "MqttSnGatewayForwarder.h"
#include "MqttSnGatewayStatus.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_GATEWAY_DISCOVERY_SUCCESS   0
#define MQTT_SN_GATEWAY_DISCOVERY_ERROR     -1

typedef struct MqttSnGatewayDiscoveryHandler_ {
  MqttSnClientNetworkInterface *clientNetwork;
  MqttSnFixedSizeRingBuffer *clientNetworkSendBuffer;
  db_handler *db_handler_;

  gateway_advertise_config *advertise_config;
  MqttSnLogger *logger;
} MqttSnGatewayDiscoveryHandler;

int32_t init_discovery_handler(MqttSnGatewayDiscoveryHandler *handler,
                               MqttSnClientNetworkInterface *clientNetwork,
                               MqttSnFixedSizeRingBuffer *clientNetworkSendBuffer,
                               db_handler *db_handler_,
                               gateway_advertise_config *advertise_config,
                               MqttSnLogger *logger);

int32_t parse_and_handle_advertise(MqttSnGatewayDiscoveryHandler *handler,
                                   MqttSnMessageData *msg,
                                   int32_t parsed_bytes,
                                   MqttSnGatewayForwarder *forwarders);
int32_t parse_and_handle_gwinfo(MqttSnGatewayDiscoveryHandler *handler,
                                MqttSnMessageData *msg,
                                int32_t parsed_bytes,
                                MqttSnGatewayForwarder *forwarders);
int32_t parse_and_handle_search_gw(MqttSnGatewayDiscoveryHandler *handler,
                                   MqttSnMessageData *msg,
                                   int32_t parsed_bytes,
                                   MqttSnGatewayForwarder *forwarders);

int32_t check_discoveries(MqttSnGatewayDiscoveryHandler *handler);
int32_t check_standy_by_monitoring(MqttSnGatewayDiscoveryHandler *handler, MQTT_SN_GATEWAY_STATUS *gateway_status);
// TODO check all gateway and update status
// TODO check only own gateway id and update status

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYDISCOVERYHANDLER_H_
