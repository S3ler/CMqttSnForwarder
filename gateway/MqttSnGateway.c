//
// Created by SomeDude on 20.06.2019.
//

#include "MqttSnGateway.h"
#include "MqttSnGatewayDiscoveryHandler.h"
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
#include <gateway/logging/MqttSnGatewayLogging.h>
#include <ringbuffer/logging/MqttSnFixedSizeRingBufferLogging.h>

uint16_t gateway_add_forwarder_encapsulation_frames(MqttSnMessageData *msg,
                                                    int32_t gen_bytes,
                                                    MqttSnGatewayForwarder *forwarders,
                                                    const MqttSnLogger *logger);
int32_t MqttSnGatewayHandleClientMessageData(MqttSnGateway *mqttSnGateway, MqttSnMessageData *clientMessageData);
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

  mqttSnGateway->gateway_status_ = MQTT_SN_GATEWAY_STATUS_STAND_BY;

  mqttSnGateway->logger = (*logger);
  mqttSnGateway->db_handler_.logger = &mqttSnGateway->logger;
  mqttSnGateway->clientNetwork.logger = &mqttSnGateway->logger;

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
    if (init_client_connection_handler(&mqttSnGateway->client_con_handler_,
                                       &mqttSnGateway->clientNetwork,
                                       &mqttSnGateway->clientNetworkSendBuffer,
                                       &mqttSnGateway->db_handler_,
                                       gccccfg,
                                       &mqttSnGateway->logger) < 0) {
      MqttSnGatewayDeinitialize(mqttSnGateway);
      return -1;
    }
  }

  //assert(gacfg != NULL);
  if (gacfg) {
    mqttSnGateway->advertisement_config = (*gacfg);
    if (init_discovery_handler(&mqttSnGateway->discovery_handler,
                               &mqttSnGateway->clientNetwork,
                               &mqttSnGateway->clientNetworkSendBuffer,
                               &mqttSnGateway->db_handler_,
                               &mqttSnGateway->advertisement_config,
                               &mqttSnGateway->logger) < 0) {
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

  if (mqttSnGateway->client_con_handler_.client_connection_timeout_check_enabled) {
    if (check_client_connection_timeouts(&mqttSnGateway->client_con_handler_) < 0) {
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

int32_t parse_and_handle_stand_by_messages(MqttSnGateway *gateway,
                                           const ParsedMqttSnHeader *header,
                                           MqttSnMessageData *msg,
                                           int32_t parsed_bytes,
                                           MqttSnGatewayForwarder *forwarders) {
  switch (header->msg_type) {
    case ADVERTISE:return parse_and_handle_advertise(&gateway->discovery_handler, msg, parsed_bytes, forwarders);
    case SEARCHGW:return parse_and_handle_search_gw(&gateway->discovery_handler, msg, parsed_bytes, forwarders);
    case GWINFO:return parse_and_handle_gwinfo(&gateway->discovery_handler, msg, parsed_bytes, forwarders);
    case ENCAPSULATED_MESSAGE:return parse_and_handle_encapsulated_messages(gateway, msg, parsed_bytes, forwarders);
    default:return 1;
  }
  return 0;
}
int32_t parse_and_handle_active_messages(MqttSnGateway *gateway,
                                         const ParsedMqttSnHeader *header,
                                         MqttSnMessageData *msg,
                                         int32_t parsed_bytes,
                                         MqttSnGatewayForwarder *forwarders) {
  switch (header->msg_type) {
    case ADVERTISE:return parse_and_handle_advertise(&gateway->discovery_handler, msg, parsed_bytes, forwarders);
    case SEARCHGW:return parse_and_handle_search_gw(&gateway->discovery_handler, msg, parsed_bytes, forwarders);
    case GWINFO:return parse_and_handle_gwinfo(&gateway->discovery_handler, msg, parsed_bytes, forwarders);
    case CONNECT:return parse_and_handle_connect(&gateway->client_con_handler_, msg, parsed_bytes, forwarders);
    case CONNACK:return 1;
    case WILLTOPICREQ:return 1;
    case WILLTOPIC:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLMSGREQ:return 1;
    case WILLMSG:
      // TODO parse_searchgw(address, bytes);
      break;
    case REGISTER:
      // TODO parse_searchgw(address, bytes);
      break;
    case REGACK:return 1;
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
    case SUBACK:return 1;
    case UNSUBSCRIBE:
      // TODO parse_searchgw(address, bytes);
      break;
    case UNSUBACK:return 1;
    case PINGREQ:return parse_and_handle_ping_req(&gateway->client_con_handler_, msg, parsed_bytes, forwarders);
    case PINGRESP:return parse_and_handle_ping_resp(&gateway->client_con_handler_, msg, parsed_bytes, forwarders);
    case DISCONNECT:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLTOPICUPD:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLTOPICRESP:return 1;
    case WILLMSGUPD:
      // TODO parse_searchgw(address, bytes);
      break;
    case WILLMSGRESP:return 1;
    case ENCAPSULATED_MESSAGE:return parse_and_handle_encapsulated_messages(gateway, msg, parsed_bytes, forwarders);
    default:return 1;
  }
  return 0;
}
int32_t parse_and_handle_message(MqttSnGateway *mqttSnGateway,
                                 MqttSnMessageData *clientMessageData,
                                 int32_t parsed_bytes,
                                 MqttSnGatewayForwarder *forwarders) {
  if (forwarders->forwarder_len == 0) {
    forwarders->wireless_node_id = clientMessageData->from;
  }

  ParsedMqttSnHeader header = {0};
  int32_t parsed_header_bytes = 0;
  if (parse_header(&header,
                   ANY_MESSAGE_TYPE,
                   clientMessageData->data + parsed_bytes,
                   clientMessageData->data_length - parsed_bytes,
                   &parsed_header_bytes) < 0) {
#ifdef WITH_LOGGING
    log_malformed_message_header(&mqttSnGateway->logger,
                                 &forwarders->wireless_node_id,
                                 clientMessageData->signal_strength,
                                 clientMessageData->data + parsed_bytes,
                                 clientMessageData->data_length - parsed_bytes);
#endif
    return 0;
  }

  switch (mqttSnGateway->gateway_status_) {
    case MQTT_SN_GATEWAY_STATUS_STAND_BY: {
      int32_t handle_rc = parse_and_handle_stand_by_messages(mqttSnGateway,
                                                             &header,
                                                             clientMessageData,
                                                             parsed_bytes,
                                                             forwarders);
      if (handle_rc <= 0) {
        return handle_rc;
      }
      break;
    }
    case MQTT_SN_GATEWAY_STATUS_ACTIVE: {
      int32_t handle_rc = parse_and_handle_active_messages(mqttSnGateway,
                                                           &header,
                                                           clientMessageData,
                                                           parsed_bytes,
                                                           forwarders);
      if (handle_rc <= 0) {
        return handle_rc;
      }
      break;
    }
    case MQTT_SN_GATEWAY_STATUS_MQTT_DISCONNECTED: {
      // TODO
      return -1;
    }
    default: break;
  }
#ifdef WITH_LOGGING
  log_unhandled_message(&mqttSnGateway->logger,
                        mqttSnGateway->gateway_status_,
                        &forwarders->wireless_node_id,
                        clientMessageData->signal_strength,
                        clientMessageData->data + parsed_bytes,
                        clientMessageData->data_length - parsed_bytes);
#endif
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

