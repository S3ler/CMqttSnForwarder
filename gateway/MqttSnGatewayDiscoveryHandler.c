//
// Created by SomeDude on 13.07.2019.
//

#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/logging/common/MqttSnMessageLogging.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <gateway/logging/MqttSnGatewayDiscoveryLogging.h>
#include <parser/logging/MqttSnGwInfoMessageLogging.h>
#include <ringbuffer/logging/MqttSnFixedSizeRingBufferLogging.h>
#include "MqttSnGatewayDiscoveryHandler.h"

int32_t init_discovery_handler(MqttSnGatewayDiscoveryHandler *handler,
                               MqttSnClientNetworkInterface *clientNetwork,
                               MqttSnFixedSizeRingBuffer *clientNetworkSendBuffer,
                               db_handler *db_handler_,
                               gateway_advertise_config *advertise_config,
                               MqttSnLogger *logger) {
  memset(handler, 0, sizeof(*handler));
  handler->logger = logger;
  handler->clientNetwork = clientNetwork;
  handler->clientNetworkSendBuffer = clientNetworkSendBuffer;
  handler->db_handler_ = db_handler_;
  handler->advertise_config = advertise_config;
#if WITH_DEBUG_LOGGING
  log_discovery_handler_initialized(logger, MQTT_SN_GATEWAY_DISCOVERY_SUCCESS);
#endif
  return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
}

int32_t parse_and_handle_advertise(MqttSnGatewayDiscoveryHandler *handler,
                                   MqttSnMessageData *msg,
                                   int32_t parsed_bytes,
                                   MqttSnGatewayForwarder *forwarders) {
  MqttSnAdvertise mqtt_sn_advertise = {0};
  if (parse_advertise(&mqtt_sn_advertise, msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
#if WITH_LOGGING
    log_malformed_message(handler->logger,
                          ADVERTISE,
                          &forwarders->wireless_node_id,
                          msg->signal_strength,
                          msg->data + parsed_bytes,
                          msg->data_length - parsed_bytes);
#endif
    return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
  }
  if (mqtt_sn_discovered_gateway_update(handler->db_handler_,
                                        mqtt_sn_advertise.gwId,
                                        mqtt_sn_advertise.duration,
                                        &forwarders->wireless_node_id,
                                        msg->signal_strength,
                                        msg->create_time_s,
                                        forwarders->forwarder_addresses,
                                        forwarders->forwarder_len)
      != DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS) {
#if WITH_LOGGING
    log_fatal_discovery_db_error(handler->logger,
                                 __FILE__,
                                 __func__,
                                 __LINE__,
                                 get_gateway_discovered_result(handler->db_handler_),
                                 get_gateway_discovered_transaction_result(handler->db_handler_));
#endif
    return MQTT_SN_GATEWAY_DISCOVERY_ERROR;
  }
  return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
}
int32_t parse_and_handle_search_gw(MqttSnGatewayDiscoveryHandler *handler,
                                   MqttSnMessageData *msg,
                                   int32_t parsed_bytes,
                                   MqttSnGatewayForwarder *forwarders) {
  if (isFull(handler->clientNetworkSendBuffer)) {
    return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
  }
  MqttSnSearchGw search_gw;
  if (parse_searchgw(&search_gw, msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
#if WITH_LOGGING
    log_malformed_message(handler->logger,
                          SEARCHGW,
                          &forwarders->wireless_node_id,
                          msg->signal_strength,
                          msg->data + parsed_bytes,
                          msg->data_length - parsed_bytes);
#endif
    return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
  }

  MqttSnMessageData toSend = {0};
  toSend.from = (*handler->clientNetwork->client_network_address);
  toSend.to = (*handler->clientNetwork->client_network_broadcast_address);
  toSend.signal_strength = msg->signal_strength;
  int32_t gen_rc = generate_gwinfo_gateway_message(toSend.data,
                                                   sizeof(toSend.data),
                                                   handler->advertise_config->gateway_id);
  if (gen_rc < 0) {
#if WITH_LOGGING
    log_gwinfo_gen_error(handler->logger, __FILE__, __func__, __LINE__);
#endif
    return MQTT_SN_GATEWAY_DISCOVERY_ERROR;
  }
  // TODO encapsulation
  toSend.data_length = gen_rc;
  if (put(handler->clientNetworkSendBuffer, &toSend) < 0) {
#if WITH_LOGGING
    log_fatal_queue_error(handler->logger, handler->clientNetworkSendBuffer, __FILE__, __func__, __LINE__, "put");
#endif
    return MQTT_SN_GATEWAY_DISCOVERY_ERROR;
  }

  return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
}
int32_t parse_and_handle_gwinfo(MqttSnGatewayDiscoveryHandler *handler,
                                MqttSnMessageData *msg,
                                int32_t parsed_bytes,
                                MqttSnGatewayForwarder *forwarders) {
  MqttSnGwInfo mqtt_sn_gw_info = {0};
  if (parse_gwinfo(&mqtt_sn_gw_info, msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
#if WITH_LOGGING
    log_malformed_message(handler->logger,
                          GWINFO,
                          &forwarders->wireless_node_id,
                          msg->signal_strength,
                          msg->data + parsed_bytes,
                          msg->data_length - parsed_bytes);
#endif
    return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
  }
  if (mqtt_sn_discovered_gateway_update_address(handler->db_handler_,
                                                mqtt_sn_gw_info.gwId,
                                                &forwarders->wireless_node_id,
                                                msg->create_time_s,
                                                msg->signal_strength,
                                                forwarders->forwarder_addresses,
                                                forwarders->forwarder_len)
      != DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS) {
#if WITH_LOGGING
    log_fatal_discovery_db_error(handler->logger,
                                 __FILE__,
                                 __func__,
                                 __LINE__,
                                 get_gateway_discovered_result(handler->db_handler_),
                                 get_gateway_discovered_transaction_result(handler->db_handler_));
#endif
    return MQTT_SN_GATEWAY_DISCOVERY_ERROR;
  }
  return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
}
int32_t check_standy_by_monitoring(MqttSnGatewayDiscoveryHandler *handler, MQTT_SN_GATEWAY_STATUS *gateway_status) {
  // TODO mabye directly into MqttSnGateway
  if ((*gateway_status) == MQTT_SN_GATEWAY_STATUS_STAND_BY
      && get_gateway_status(handler->db_handler_, handler->advertise_config->gateway_id)
          != DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_ACTIVE) {
    (*gateway_status) = MQTT_SN_GATEWAY_STATUS_ACTIVE;
  }
  return MQTT_SN_GATEWAY_DISCOVERY_SUCCESS;
}


