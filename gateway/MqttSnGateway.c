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

int32_t MqttSnGatewayHandleClientMessageData(MqttSnGateway *mqttSnGateway, MqttSnMessageData *clientMessageData);
int32_t parse_and_handle_advertise(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_gwinfo(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_connect(MqttSnGateway *gateway, MqttSnMessageData *msg);
void remove_client_subscriptions(MqttSnGateway *gateway, const char *client_id, device_address *client_address);
int32_t parse_and_handle_searchgw(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t check_client_timeouts(MqttSnGateway *gateway, uint64_t current_time);
int32_t check_connected_client_timeout(MqttSnGateway *gateway, uint64_t current_time);
uint64_t get_tolerance_timeout(uint16_t duration);
int32_t send_ping_req(MqttSnGateway *gateway, device_address *client_address);
int32_t parse_and_handle_ping_req(MqttSnGateway *gateway, MqttSnMessageData *msg);
int32_t parse_and_handle_ping_resp(MqttSnGateway *gateway, MqttSnMessageData *msg);
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

  mqttSnGateway->client_timeout_check_period = 1;
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

  uint64_t current_time;
  if (get_timestamp_s(&current_time) < 0) {
    return -1;
  }
  uint64_t elapsed_time = current_time - mqttSnGateway->last_client_timeout_check;
  if (elapsed_time >= mqttSnGateway->client_timeout_check_period) {
    // ToDo log
    if (check_client_timeouts(mqttSnGateway, current_time) < 0) {
      return -1;
    }
    mqttSnGateway->last_client_timeout_check = current_time;
  }
  // receive client network
  // receive mqtt network
  // regular stuff
  return 0;
}
int32_t check_client_timeouts(MqttSnGateway *gateway, uint64_t current_time) {
  uint64_t client_count = 0;
  if (db_get_client_count(&gateway->db_handler_, &client_count) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
    return -1;
  }
  for (uint64_t client_number = 0; client_number < client_count; client_number++) {

    if (db_start_client_transaction_by_client_count(&gateway->db_handler_, client_number)
        != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
      return -1;
    }
    DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status;
    if (db_get_client_status(&gateway->db_handler_, &client_status)
        != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
      return -1;
    }

    int32_t check_rc = 0;
    switch (client_status) {
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_EMPTY:break;
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE:check_rc = check_connected_client_timeout(gateway, current_time);
        break;
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP:// TODO implement me: check_asleep_client_timeout();
        break;
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE:// TODO implement me: check_awake_client_timeout();
        break;
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_DISCONNECTED:// TODO implement me: check_disconnected_client_timeout();
        break;
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_LOST:// TODO implement me: check_lost_client_timeout();
        break;
      default:break;
    }

    assert(db_is_client_transaction_started(&gateway->db_handler_));
    if (db_end_client_transaction(&gateway->db_handler_) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
      return -1;
    }
    if (check_rc < 0) {
      return -1;
    }
  }
  return 0;
}
int32_t check_connected_client_timeout(MqttSnGateway *gateway, uint64_t current_time) {
  assert(db_is_client_transaction_started(&gateway->db_handler_));
  // TODO when we have a lot of client
  // TODO it may happen that we need to send so much PingReq that the SendBuffer is full
  // TODO some client may starve from the PingReq
  uint16_t connect_duration;
  uint64_t ping_req_received;
  uint64_t ping_resp_received;
  MQTT_SN_MESSAGE_TYPE ping_req_await_message_type;

  DB_HANDLER_CK_CLIENT_RC(db_get_client_connect_duration(&gateway->db_handler_, &connect_duration))
  DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_req_received(&gateway->db_handler_, &ping_req_received))
  DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_resp_received(&gateway->db_handler_, &ping_resp_received))
  DB_HANDLER_CK_CLIENT_RC(db_get_ping_req_await_msg_type(&gateway->db_handler_, &ping_req_await_message_type))

  uint64_t last_timeout_reset = ping_resp_received > ping_req_received ? ping_resp_received : ping_req_received;
  uint64_t elapsed_time = current_time - last_timeout_reset;

  if (elapsed_time > connect_duration && ping_req_await_message_type == ANY_MESSAGE_TYPE) {
    // timeout of the connect duration without tolerance
    // no ping request in flight for this client
    // send ping request from the gateway to check client status
    device_address client_address;
    DB_HANDLER_CK_CLIENT_RC(db_get_client_address(&gateway->db_handler_, &client_address))
    if (send_ping_req(gateway, &client_address) < 0) {
      return -1;
    }
  }

  uint64_t tolerance_timeout = get_tolerance_timeout(connect_duration);
  if (elapsed_time > tolerance_timeout) {
    // tolerance timeout exceeded
    // client is timed out, set client lost
    DB_HANDLER_CK_CLIENT_RC(db_set_client_status(&gateway->db_handler_, DB_ENTRY_MQTT_SN_CLIENT_STATUS_LOST))
  }
  return 0;
}
int32_t send_ping_req(MqttSnGateway *gateway, device_address *client_address) {
  if (isFull(&gateway->mqttClientNetworkSendBuffer)) {
    return 0;
  }
  MqttSnMessageData msg = {0};
  if (get_timestamp_s(&msg.create_time_s)) {
    return -1;
  }
  msg.from = (*gateway->clientNetwork.client_network_address);
  msg.to = (*client_address);
  int32_t gen_rc;
  MQTT_SN_MESSAGE_PARSER_CH_RC(gen_rc, generate_ping_req(msg.data, sizeof(msg.data)))
  msg.data_length = gen_rc;

  if (put(&gateway->clientNetworkSendBuffer, &msg) < 0) {
    // TODO should never happen
    return 0;
  }
  DB_HANDLER_CK_CLIENT_RC(db_set_ping_req_await_msg_type(&gateway->db_handler_, PINGRESP))
  // TODO log pinging client XYZ
  // TODO  signal strength dropped?
  return 0;
}
uint64_t get_tolerance_timeout(uint16_t duration) {
  uint64_t tolerance_part;
  if (duration <= 60) {
    tolerance_part = (uint64_t) ((double) duration * (double) 0.5);
  } else {
    tolerance_part = (uint64_t) ((double) duration * (double) 0.1);
  }
  return tolerance_part + (uint64_t) duration;
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
  if (PlatformCompatibilityGetTimestamp(&clientMessageData->create_time_s) < 0) {
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
    case PINGREQ:return parse_and_handle_ping_req(mqttSnGateway, clientMessageData);
    case PINGRESP:return parse_and_handle_ping_resp(mqttSnGateway, clientMessageData);
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
int32_t parse_and_handle_ping_resp(MqttSnGateway *gateway, MqttSnMessageData *msg) {
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
      db_reset_client(&gateway->db_handler_, connect.client_id, &msg->from, connect.duration, msg->create_time_s);
    } else {
      // before we can remove the client, we must unsubscribe from all subscriptions
      remove_client_subscriptions(gateway, connect.client_id, &msg->from);
      delete_client(&gateway->db_handler_);
      add_client(&gateway->db_handler_, connect.client_id, &msg->from, connect.duration, msg->create_time_s);
    }
  } else {
    add_client(&gateway->db_handler_, connect.client_id, &msg->from, connect.duration, msg->create_time_s);
  }

  DB_ENTRY_MQTT_SN_CLIENT_RESULT client_result = db_end_client_transaction(&gateway->db_handler_);
  if (client_result != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#if WITH_LOGGING
    // TODO log error
#endif
    return -1;
  }

  if (connect.will) {

    // TODO await WILL_TOPIC_RESP
  } else {
    // TODO await ANY_MESSAGE
  }

  if (connect.will) {
    // TODO send will topic request
  } else {
    if (reuse_mqtt_sn_message_data(msg) < 0) {
      return -1;
    }
    int32_t gen_rc;
    if ((gen_rc = generate_connack(msg->data, sizeof(msg->data), RETURN_CODE_ACCEPTED)) < 0) {
      // should never happen
#if WITH_LOGGING
      // TODO log internal error + where
#endif
      return -1;
    }
    msg->data_length = gen_rc;
    if (put(&gateway->clientNetworkSendBuffer, msg) < 0) {
      // should never happen
#if WITH_LOGGING
      // TODO log internal error + where
#endif
      return -1;
    }
  }

#if WITH_LOGGING
  // TODO log success
#endif
  return 0;
}
void remove_client_subscriptions(MqttSnGateway *gateway, const char *client_id, device_address *client_address) {
  // TODO implement me
}
