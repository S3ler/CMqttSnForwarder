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

int32_t MqttSnGatewayHandleClientMessageData(MqttSnGateway *mqttSnGateway, MqttSnMessageData *clientMessageData);
int32_t parse_and_handle_advertise(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_gwinfo(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_connect(MqttSnGateway *gateway, MqttSnMessageData *msg);
void remove_client_subscriptions(MqttSnGateway *gateway, const char *client_id, device_address *client_address);
int32_t parse_and_handle_searchgw(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t MqttSnGatewayInitialize(MqttSnGateway *mqttSnGateway,
                                const MqttSnLogger *logger,
                                MqttClient *mqttClient,
                                void *clientNetworkContext,
                                const gateway_advertise_config *gacfg) {
#ifdef WITH_LOGGING
  if (logger) {
    mqttSnGateway->logger = (*logger);
    mqttSnGateway->db_handler_.logger = &mqttSnGateway->logger;
    mqttSnGateway->clientNetwork.logger = &mqttSnGateway->logger;
  }
#endif

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

  return 0;
}
int32_t MqttSnGatewayDeinitialize(MqttSnGateway *mqttSnGateway) {
  ClientNetworkDeinitialize(&mqttSnGateway->clientNetwork, mqttSnGateway->clientNetworkContext);
  MqttClientDeinitialize(mqttSnGateway->mqttClient);
#ifdef WITH_LOGGING
  mqttSnGateway->logger.log_deinit(&mqttSnGateway->logger);
#endif
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
  ParsedMqttSnHeader header = {0};
  int32_t parsed_bytes = 0;
  if (parse_header(&header, ANY_MESSAGE_TYPE, clientMessageData->data, clientMessageData->data_length, &parsed_bytes)
      < 0) {
#ifdef WITH_DEBUG_LOGGING
    log_client_mqtt_sn_message_malformed(&mqttSnGateway->logger,
                                         &clientMessageData->from,
                                         clientMessageData->data,
                                         clientMessageData->data_length,
                                         clientMessageData->signal_strength);
#endif
    return 0;
  }
  if (header.msg_type == RESERVED_INVALID || MQTT_SN_MESSAGE_TYPE_RESERVED(header.msg_type)) {
#ifdef WITH_DEBUG_LOGGING
    log_client_mqtt_sn_message_malformed(&mqttSnGateway->logger,
                                         &clientMessageData->from,
                                         clientMessageData->data,
                                         clientMessageData->data_length,
                                         clientMessageData->signal_strength);
#endif
    return 0;
  }

  // TODO add receive time to message
  if (PlatformCompatibilityGetTimestamp(&clientMessageData->received_time) < 0) {
    return -1;
  }
  // TODO add receive time to message

  switch (header.msg_type) {
    case ADVERTISE: return parse_and_handle_advertise(mqttSnGateway, clientMessageData);
      break;
    case SEARCHGW: return parse_and_handle_searchgw(mqttSnGateway, clientMessageData);
      break;
    case GWINFO: return parse_and_handle_gwinfo(mqttSnGateway, clientMessageData);
      break;
    case CONNECT: return parse_and_handle_connect(mqttSnGateway, clientMessageData);
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
    case PINGREQ:
      // TODO parse_searchgw(address, bytes);
      break;
    case PINGRESP:
      // TODO parse_searchgw(address, bytes);
      break;
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
      // TODO parse_searchgw(address, bytes);
      break;
    default:break;
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
                                        msg->received_time)
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
                                                msg->received_time)
      != DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS) {
    return -1;
  }
  return 0;
}
int32_t parse_and_handle_connect(MqttSnGateway *gateway, MqttSnMessageData *msg) {
  if (isFull(&gateway->mqttClientNetworkSendBuffer)) {
    return 0;
  }

  // parse
  ParsedMqttSnConnect connect = {0};
  if (parse_connect(&connect, msg->data, msg->data_length) < 0) {
    return 0;
  }

  // update database
  db_start_client_transaction(&gateway->db_handler_, &msg->from, NULL);
  if (client_exist(&gateway->db_handler_) == DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
    if (!connect.clean_session) {
      db_reset_client(&gateway->db_handler_, connect.client_id, &msg->from, connect.duration, msg->received_time);
    } else {
      // before we can remove the client, we must unsubscribe from all subscriptions
      remove_client_subscriptions(gateway, connect.client_id, &msg->from);
      delete_client(&gateway->db_handler_);
      add_client(&gateway->db_handler_, connect.client_id, &msg->from, connect.duration, msg->received_time);
    }
  } else {
    add_client(&gateway->db_handler_, connect.client_id, &msg->from, connect.duration, msg->received_time);
  }
  if (connect.will) {

    // TODO await WILL_TOPIC_RESP
  } else {
    // TODO await ANY_MESSAGE
  }

  if (connect.will) {
    // TODO send will topic request
  } else {
    MqttSnMessageData toSend = {0};
    toSend.to = msg->from;
    toSend.signal_strength = msg->signal_strength;
    if (generate_connack(toSend.data, sizeof(toSend.data), RETURN_CODE_ACCEPTED) < 0) {
      // should never happen
#if WITH_LOGGING
      // TODO log internal error + where
#endif
      return -1;
    }
    if (put(&gateway->clientNetworkSendBuffer, &toSend) < 0) {
      // should never happen
#if WITH_LOGGING
      // TODO log internal error + where
#endif
      return -1;
    }
  }
  DB_ENTRY_MQTT_SN_CLIENT_RESULT client_result = db_end_client_transaction(&gateway->db_handler_);
  if (client_result != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#if WITH_LOGGING
    // TODO log error
#endif
    return -1;
  }
#if WITH_LOGGING
  // TODO log success
#endif
  return 0;
}
void remove_client_subscriptions(MqttSnGateway *gateway, const char *client_id, device_address *client_address) {
  // TODO implement me
}
