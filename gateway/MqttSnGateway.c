//
// Created by SomeDude on 20.06.2019.
//

#include "MqttSnGateway.h"
#include <string.h>
#include <parser/MqttSnMessageParser.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <gateway/database/db_handler.h>
#include <platform/platform_compatibility.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <parser/MqttSnConnectMessage.h>
#include <parser/MqttSnConnackMessage.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/logging/common/MqttSnMessageLogging.h>
#include <assert.h>
#include <math.h>
#include <parser/MqttSnPingReqMessage.h>
#include <parser/MqttSnPingRespMessage.h>
#include <parser/MqttSnForwarderEncapsulationMessage.h>
#include <config/gateway/gateway_client_connection_config.h>

int32_t MqttSnGatewayHandleClientMessageData(MqttSnGateway *mqttSnGateway, MqttSnMessageData *clientMessageData);
int32_t parse_and_handle_advertise(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_gwinfo(MqttSnGateway *gateway, MqttSnMessageData *msg);

int32_t parse_and_handle_searchgw(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_ping_req(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t mgw_parse_and_handle_ping_resp(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_encapsulated_messages(MqttSnGateway *gateway,
                                               MqttSnMessageData *msg,
                                               int32_t parsed_bytes,
                                               MqttSnGatewayForwarder *forwarders);
int32_t parse_and_handle_message(MqttSnGateway *mqttSnGateway,
                                 MqttSnMessageData *clientMessageData,
                                 int32_t parsed_bytes,
                                 MqttSnGatewayForwarder *forwarders);
int32_t MqttSnGatewayInitialize(MqttSnGateway *mqttSnGateway,
                                const MqttSnLogger *logger,
                                MqttClient *mqttClient,
                                void *clientNetworkContext,
                                const gateway_advertise_config *gacfg,
                                const gateway_client_connection_config *gccccfg) {

  mqttSnGateway->logger = (*logger);
  mqttSnGateway->db_handler_.logger = &mqttSnGateway->logger;
  mqttSnGateway->clientNetwork.logger = &mqttSnGateway->logger;

  assert(gacfg != NULL);
  mqttSnGateway->advertisement_config = (*gacfg);
  if (db_handler_initialize(&mqttSnGateway->db_handler_, &mqttSnGateway->logger) < 0) {
    MqttSnGatewayDeinitialize(mqttSnGateway);
    return -1;
  }

  mqttSnGateway->mqttClient = mqttClient;
  MqttSnFixedSizeRingBufferInit(&mqttSnGateway->mqttClientNetworkReceiveBuffer);
  MqttSnFixedSizeRingBufferInit(&mqttSnGateway->mqttClientNetworkSendBuffer);

  mqttSnGateway->clientNetworkContext = clientNetworkContext;
  MqttSnFixedSizeRingBufferInit(&mqttSnGateway->clientNetworkReceiveBuffer);
  MqttSnFixedSizeRingBufferInit(&mqttSnGateway->clientNetworkSendBuffer);

#ifdef WITH_LOGGING
  if (log_status(&mqttSnGateway->logger) < 0) {
    MqttSnGatewayDeinitialize(mqttSnGateway);
    return -1;
  }
#endif

  if (gccccfg) {
    if (init_client_connection_handler(&mqttSnGateway->client_connection_handler_,
                                       &mqttSnGateway->clientNetwork,
                                       &mqttSnGateway->clientNetworkSendBuffer,
                                       &mqttSnGateway->db_handler_,
                                       gccccfg) < 0) {
      MqttSnGatewayDeinitialize(mqttSnGateway);
      return -1;
    }
  }

  return 0;
}
int32_t MqttSnGatewayDeinitialize(MqttSnGateway *mqttSnGateway) {
  ClientNetworkDeinitialize(&mqttSnGateway->clientNetwork, mqttSnGateway->clientNetworkContext);
  MqttClientDeinitialize(mqttSnGateway->mqttClient);
  return 0;
}
int32_t MqttSnGatewayConnect(MqttSnGateway *mqttSnGateway) {
  if (MqttClientNetworkConnect(mqttSnGateway->mqttClient) < 0) {
    MqttSnGatewayDeinitialize(mqttSnGateway);
    return -1;
  }
  if (ClientNetworkConnect(&mqttSnGateway->clientNetwork, mqttSnGateway->clientNetworkContext) < 0) {
    MqttSnGatewayDeinitialize(mqttSnGateway);
    return -1;
  }
  return 0;
}
int32_t MqttSnGatewayDisconnect(MqttSnGateway *mqttSnGateway) {
  ClientNetworkDisconnect(&mqttSnGateway->clientNetwork, mqttSnGateway->clientNetworkContext);
  MqttClientNetworkDisconnect(mqttSnGateway->mqttClient);
  return 0;
}
int32_t MqttSnGatewayLoop(MqttSnGateway *mqttSnGateway) {

  if (ClientNetworkReceive(&mqttSnGateway->clientNetwork,
                           &mqttSnGateway->clientNetworkReceiveBuffer,
                           mqttSnGateway->clientNetworkReceiveTimeout,
                           mqttSnGateway->clientNetworkContext) < 0) {
    ClientNetworkDisconnect(&mqttSnGateway->clientNetwork, mqttSnGateway->clientNetworkContext);
  }
#ifdef Arduino_h
  yield();
#endif
  {
    MqttSnMessageData clientMessageData = {0};
    MqttSnGatewayHandleClientMessageData(mqttSnGateway, &clientMessageData);
  }
  if (ClientNetworkSend(&mqttSnGateway->clientNetwork,
                        &mqttSnGateway->clientNetworkSendBuffer,
                        mqttSnGateway->clientNetworkSendTimeout,
                        mqttSnGateway->clientNetworkContext) < 0) {
    ClientNetworkDisconnect(&mqttSnGateway->clientNetwork, mqttSnGateway->clientNetworkContext);
  }

  if (mqttSnGateway->client_connection_handler_.client_connection_timeout_check_enabled) {
    if (check_client_connection_timeouts(&mqttSnGateway->client_connection_handler_) < 0) {
      return -1;
    }
  }


// receive client network
// receive mqtt network
// regular stuff
  return 0;
}

int32_t MqttSnGatewayHandleClientMessageData(MqttSnGateway *mqttSnGateway, MqttSnMessageData *clientMessageData) {
  if (isFull(&mqttSnGateway->mqttClientNetworkSendBuffer)) {
    return 0;
  }
  if (pop(&mqttSnGateway->clientNetworkReceiveBuffer, clientMessageData)) {
    return 0;
  }

  // TODO add receive time to message
  if (PlatformCompatibilityGetTimestamp(&clientMessageData->create_time_s) < 0) {
    return -1;
  }
  // TODO add receive time to message



  int32_t parsed_bytes = 0;
  MqttSnGatewayForwarder mqtt_sn_gateway_forwarder;
  MqttSnGatewayForwarderInit(&mqtt_sn_gateway_forwarder);
  return parse_and_handle_message(mqttSnGateway, clientMessageData, parsed_bytes, &mqtt_sn_gateway_forwarder);

}
int32_t parse_and_handle_message(MqttSnGateway *mqttSnGateway,
                                 MqttSnMessageData *clientMessageData,
                                 int32_t parsed_bytes,
                                 MqttSnGatewayForwarder *forwarders) {
  ParsedMqttSnHeader header = {0};
  int32_t parsed_header_bytes = 0;
  if (parse_header(&header,
                   ANY_MESSAGE_TYPE,
                   clientMessageData->data + parsed_bytes,
                   clientMessageData->data_length - parsed_bytes,
                   &parsed_header_bytes) < 0) {
#ifdef WITH_DEBUG_LOGGING
    log_client_mqtt_sn_message_malformed(&mqttSnGateway->logger,
                                         &clientMessageData->from,
                                         clientMessageData->data + parsed_bytes,
                                         clientMessageData->data_length - parsed_bytes,
                                         clientMessageData->signal_strength);
#endif
    return 0;
  }
  if (header.msg_type == RESERVED_INVALID || MQTT_SN_MESSAGE_TYPE_RESERVED(header.msg_type)) {
#ifdef WITH_DEBUG_LOGGING
    log_client_mqtt_sn_message_malformed(&mqttSnGateway->logger,
                                         &clientMessageData->from,
                                         clientMessageData->data + parsed_bytes,
                                         clientMessageData->data_length - parsed_bytes,
                                         clientMessageData->signal_strength);
#endif
    return 0;
  }

  if (forwarders->forwarder_len == 0) {
    forwarders->wireless_node_id = clientMessageData->from;
  }

  switch (header.msg_type) {
    case ADVERTISE: return parse_and_handle_advertise(mqttSnGateway, clientMessageData);
      break;
    case SEARCHGW: return parse_and_handle_searchgw(mqttSnGateway, clientMessageData);
      break;
    case GWINFO: return parse_and_handle_gwinfo(mqttSnGateway, clientMessageData);
      break;
    case CONNECT:
      return parse_and_handle_connect(&mqttSnGateway->client_connection_handler_,
                                      clientMessageData,
                                      parsed_bytes,
                                      forwarders);
      break;
    case CONNACK:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLTOPICREQ:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLTOPIC:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLMSGREQ:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLMSG:
      // TODO parse_searchgw(address, bytes);
      break;
    case REGISTER:
      // TODO parse_searchgw(address, bytes);
      break;
    case REGACK:
      // TODO parse_searchgw(address, bytes);
      break;
    case PUBLISH:
      // TODO parse_searchgw(address, bytes);
      break;
    case PUBACK:
      // TODO parse_searchgw(address, bytes);
      break;
    case PUBCOMP:
      // TODO parse_searchgw(address, bytes);
      break;
    case PUBREC:
      // TODO parse_searchgw(address, bytes);
      break;
    case PUBREL:
      // TODO parse_searchgw(address, bytes);
      break;
    case SUBSCRIBE:
      // TODO parse_searchgw(address, bytes);
      break;
    case SUBACK:
      // TODO parse_searchgw(address, bytes);
      break;
    case UNSUBSCRIBE:
      // TODO parse_searchgw(address, bytes);
      break;
    case UNSUBACK:
      // TODO parse_searchgw(address, bytes);
      break;
    case PINGREQ:return parse_and_handle_ping_req(mqttSnGateway, clientMessageData);
    case PINGRESP:return mgw_parse_and_handle_ping_resp(mqttSnGateway, clientMessageData);
    case DISCONNECT:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLTOPICUPD:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLTOPICRESP:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLMSGUPD:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLMSGRESP:
      // TODO parse_searchgw(address, bytes);
      break;
    case ENCAPSULATED_MESSAGE:
      return parse_and_handle_encapsulated_messages(mqttSnGateway,
                                                    clientMessageData,
                                                    parsed_bytes,
                                                    forwarders);
    default:break;
  }
  return 0;
}
int32_t parse_and_handle_encapsulated_message(MqttSnGateway *gateway,
                                              device_address *forwarder_addresses,
                                              uint16_t forwarder_address_count,
                                              const uint8_t *data,
                                              uint16_t data_length,
                                              MqttSnMessageData *msg,
                                              int32_t parsed_bytes) {
  return -1;
}

int32_t mgw_parse_and_handle_ping_resp(MqttSnGateway *gateway, MqttSnMessageData *msg) {
  if (parse_ping_resp_byte(msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
    // TODO log cannot parse ping resp
#endif
    return 0;
  }
#ifdef WITH_DEBUG_LOGGING
  // TODO log client ping resp
#endif

  db_start_client_transaction(&gateway->db_handler_, &msg->from, NULL);

  db_set_client_last_ping_resp_received(&gateway->db_handler_, msg->create_time_s);
  db_set_ping_req_await_msg_type(&gateway->db_handler_, ANY_MESSAGE_TYPE);

  DB_ENTRY_MQTT_SN_CLIENT_RESULT db_rc = db_end_client_transaction(&gateway->db_handler_);
  if (db_rc != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS
      && get_db_handler_result(&gateway->db_handler_) == DB_HANDLER_RESULT_ERROR) {
#ifdef WITH_LOGGING
    // TODO log db client status/error
#endif
    return -1;
  }
  return 0;
}
int32_t parse_and_handle_ping_req(MqttSnGateway *gateway, MqttSnMessageData *msg) {
  assert(!isFull(&gateway->mqttClientNetworkSendBuffer));

  if (parse_ping_req_byte(msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
    // TODO log cannot parse ping req
#endif
    return 0;
  }
#ifdef WITH_DEBUG_LOGGING
  // TODO log client ping req
#endif

  db_start_client_transaction(&gateway->db_handler_, &msg->from, NULL);
  db_set_client_last_ping_req_received(&gateway->db_handler_, msg->create_time_s);
  DB_ENTRY_MQTT_SN_CLIENT_RESULT db_rc = db_end_client_transaction(&gateway->db_handler_);

  if (db_rc != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS
      && get_db_handler_result(&gateway->db_handler_) == DB_HANDLER_RESULT_ERROR) {
#ifdef WITH_LOGGING
    // TODO log db client status/error
#endif
    return -1;
  }

  // reuse msg to save memory
  if (reuse_mqtt_sn_message_data(msg) < 0) {
    return -1;
  }
  int32_t gen_rc = generate_ping_resp(msg->data, sizeof(msg->data));
  if (gen_rc < 0) {
    // TODO log
    return -1;
  }
  msg->data_length = gen_rc;
  if (put(&gateway->clientNetworkSendBuffer, msg) < 0) {
    // TODO log
    return -1;
  }
  return 0;
}
int32_t parse_and_handle_advertise(MqttSnGateway *gateway, MqttSnMessageData *msg) {
  MqttSnAdvertise mqtt_sn_advertise = {0};
  if (parse_advertise(&mqtt_sn_advertise, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
    // TODO implement parse error
    log_client_mqtt_sn_message_malformed(&gateway->logger,
                                         &msg->from,
                                         msg->data,
                                         msg->data_length,
                                         msg->signal_strength);
#endif
    return 0;
  }
  if (mqtt_sn_discovered_gateway_update(&gateway->db_handler_,
                                        mqtt_sn_advertise.gwId,
                                        mqtt_sn_advertise.duration,
                                        &msg->from,
                                        msg->create_time_s)
      != DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS) {
    return -1;
  }
  return 0;
}
int32_t parse_and_handle_searchgw(MqttSnGateway *gateway, MqttSnMessageData *msg) {
  if (isFull(&gateway->mqttClientNetworkSendBuffer)) {
    return 0;
  }
  MqttSnSearchGw search_gw;
  if (parse_searchgw(&search_gw, msg->data, msg->data_length) < 0) {
    // TODO disconnect client if connected?
    return 0;
  }

  MqttSnMessageData toSend = {0};
  toSend.from = (*gateway->clientNetwork.client_network_address);
  toSend.to = (*gateway->clientNetwork.client_network_broadcast_address);
  toSend.signal_strength = msg->signal_strength;
  int32_t gen_rc =
      generate_gwinfo_gateway_message(toSend.data, sizeof(toSend.data), gateway->advertisement_config.gateway_id);
  if (gen_rc < 0) {
    // should never happen
#if WITH_LOGGING
    // TODO log internal error + where
#endif
    return -1;
  }
  toSend.data_length = gen_rc;
  if (put(&gateway->clientNetworkSendBuffer, &toSend) < 0) {
    // should never happen
#if WITH_LOGGING
    // TODO log internal error + where
#endif
    return -1;
  }

  return 0;
}
int32_t parse_and_handle_gwinfo(MqttSnGateway *gateway, MqttSnMessageData *msg) {
  MqttSnGwInfo mqtt_sn_gw_info = {0};
  if (parse_gwinfo(&mqtt_sn_gw_info, msg->data, msg->data_length) < 0) {
    // TODO disconnect client?
    return 0;
  }
  if (mqtt_sn_discovered_gateway_update_address(&gateway->db_handler_,
                                                mqtt_sn_gw_info.gwId,
                                                &msg->from,
                                                msg->create_time_s)
      != DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS) {
    return -1;
  }
  return 0;
}

int32_t parse_and_handle_encapsulated_messages(MqttSnGateway *gateway,
                                               MqttSnMessageData *msg,
                                               int32_t parsed_bytes,
                                               MqttSnGatewayForwarder *forwarders) {
  if (forwarders->forwarder_max_len == 0) {
    return 0;
  }

  int32_t parse_rc = parse_multiple_forwarder_encapsulation_headers_byte(forwarders->forwarder_radiuses,
                                                                         forwarders->forwarder_addresses,
                                                                         &forwarders->forwarder_len,
                                                                         forwarders->forwarder_max_len,
                                                                         &msg->from,
                                                                         &forwarders->wireless_node_id,
                                                                         msg->data,
                                                                         msg->data_length);
  if (parse_rc < 0) {
    return 0;
  }

  parsed_bytes += parse_rc;
  ParsedMqttSnHeader h = {0};
  int32_t h_bytes = 0;
  int32_t h_rc = 0;
  h_rc = parse_header(&h, ANY_MESSAGE_TYPE, msg->data + parsed_bytes, msg->data_length - parsed_bytes, &h_bytes);
  if (h_rc < 0) {
    // TODO log
    return 0;
  }
  if (h.msg_type == ENCAPSULATED_MESSAGE) {
    // too much encapsulation messages
    // we cannot save all forwarder_addresses
    // TODO log
    return 0;
  }

  // update database
  return parse_and_handle_message(gateway, msg, parsed_bytes, forwarders);
}
