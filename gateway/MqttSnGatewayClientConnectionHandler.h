//
// Created by SomeDude on 01.07.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYCLIENTCONNECTIONHANDLER_H_
#define CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYCLIENTCONNECTIONHANDLER_H_

#include <stdint.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#include <gateway/database/db_handler.h>
#include <config/gateway/gateway_client_connection_config.h>
#include "MqttSnGatewayForwarder.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnGatewayClientConnectionHandler_ {
  MqttSnClientNetworkInterface *clientNetwork;
  MqttSnFixedSizeRingBuffer *clientNetworkSendBuffer;
  db_handler *db_handler_;

  uint8_t client_connection_timeout_check_enabled;
  uint64_t last_client_connection_timeout_check;
  uint64_t client_connection_timeout_check_period;
  int32_t client_connection_timeout_offset;

  MqttSnLogger* logger;
} MqttSnGatewayClientConnectionHandler;

int32_t init_client_connection_handler(MqttSnGatewayClientConnectionHandler *handler,
                                       MqttSnClientNetworkInterface *clientNetwork,
                                       MqttSnFixedSizeRingBuffer *clientNetworkSendBuffer,
                                       db_handler *db_handler_,
                                       const gateway_client_connection_config *cfg,
                                       MqttSnLogger *logger);

int32_t check_client_connection_timeouts(MqttSnGatewayClientConnectionHandler *handler);

int32_t parse_and_handle_connect(MqttSnGatewayClientConnectionHandler *handler,
                                 MqttSnMessageData *msg,
                                 int32_t parsed_bytes,
                                 MqttSnGatewayForwarder *forwarders);
int32_t parse_and_handle_ping_req(MqttSnGatewayClientConnectionHandler *handler,
                                  MqttSnMessageData *msg,
                                  int32_t parsed_bytes,
                                  MqttSnGatewayForwarder *forwarders);
int32_t parse_and_handle_ping_resp(MqttSnGatewayClientConnectionHandler *handler,
                                   MqttSnMessageData *msg,
                                   int32_t parsed_bytes,
                                   MqttSnGatewayForwarder *forwarders);
int32_t remove_client_subscriptions(MqttSnGatewayClientConnectionHandler *gateway, const char *handler, device_address *client_address);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYCLIENTCONNECTIONHANDLER_H_
