//
// Created by SomeDude on 30.06.2019.
//

#include "MqttSnGatewayClientConnectionHandler.h"
#include <parser/MqttSnPingReqMessage.h>
#include <gateway/database/db_handler.h>
#include <platform/MqttSnMessageData.h>

int32_t init_client_connection_handler(MqttSnGatewayClientConnectionHandler *handler,
                                       MqttSnClientNetworkInterface *clientNetwork,
                                       MqttSnFixedSizeRingBuffer *clientNetworkSendBuffer,
                                       db_handler *db_handler_,
                                       const gateway_client_connection_config *cfg) {
  assert(handler != NULL);
  assert(clientNetwork != NULL);
  assert(clientNetworkSendBuffer != NULL);
  assert(db_handler_ != NULL);
  assert(cfg != NULL);

  memset(handler, 0, sizeof(*handler));

  handler->db_handler_ = db_handler_;
  handler->clientNetworkSendBuffer = clientNetworkSendBuffer;
  handler->clientNetwork = clientNetwork;

  handler->client_connection_timeout_check_enabled = cfg->client_connection_timeout_check_enabled;
  handler->client_connection_timeout_check_period = cfg->client_connection_timeout_check_period;
  handler->client_connection_timeout_offset = cfg->client_connection_timeout_offset;

  if (get_timestamp_s(&handler->last_client_connection_timeout_check) < 0) {
    return -1;
  }
  return 0;
}

int32_t check_active_client_connection_timeout(MqttSnGatewayClientConnectionHandler *handler,
                                               uint64_t current_time);
uint64_t get_connection_timeout_offset_duration(uint16_t duration, int32_t offset);
uint64_t get_client_tolerance_timeout(uint16_t timeout);
int32_t send_client_connection_ping_req(MqttSnGatewayClientConnectionHandler *handler, device_address *client_address);
uint64_t get_active_last_timeout_reset(uint64_t ping_resp_received, uint64_t ping_req_received);

int32_t check_client_connection_timeouts(MqttSnGatewayClientConnectionHandler *handler) {
  uint64_t current_time;
  if (get_timestamp_s(&current_time) < 0) {
    return -1;
  }
  uint64_t elapsed_time = current_time - handler->last_client_connection_timeout_check;
  if (elapsed_time < handler->client_connection_timeout_check_period) {
    return 0;
  }
  // ToDo log

  uint64_t client_count = 0;
  if (db_get_client_count(handler->db_handler_, &client_count) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
    return -1;
  }
  for (uint64_t client_number = 0; client_number < client_count; client_number++) {

    if (db_start_client_transaction_by_client_count(handler->db_handler_, client_number)
        != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
      return -1;
    }
    DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status;
    if (db_get_client_status(handler->db_handler_, &client_status) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
      return -1;
    }

    int32_t check_rc = 0;
    switch (client_status) {
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_EMPTY:break;
      case DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE:
        check_rc = check_active_client_connection_timeout(
            handler,
            current_time);
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

    assert(db_is_client_transaction_started(handler->db_handler_));
    if (db_end_client_transaction(handler->db_handler_) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
      return -1;
    }
    if (check_rc < 0) {
      return -1;
    }
  }
  handler->last_client_connection_timeout_check = current_time;
  return 0;
}

int32_t check_active_client_connection_timeout(MqttSnGatewayClientConnectionHandler *handler,
                                               uint64_t current_time) {
  assert(db_is_client_transaction_started(handler->db_handler_));
  // TODO when we have a lot of client
  // TODO it may happen that we need to send so much PingReq that the SendBuffer is full
  // TODO some client may starve from the PingReq
  uint16_t connect_duration;
  uint64_t ping_req_received;
  uint64_t ping_resp_received;
  MQTT_SN_MESSAGE_TYPE ping_req_await_message_type;

  // TODO könnte man auch mit einem call machen
  DB_HANDLER_CK_CLIENT_RC(db_get_client_connect_duration(handler->db_handler_, &connect_duration))
  DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_req_received(handler->db_handler_, &ping_req_received))
  DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_resp_received(handler->db_handler_, &ping_resp_received))
  DB_HANDLER_CK_CLIENT_RC(db_get_ping_req_await_msg_type(handler->db_handler_, &ping_req_await_message_type))

  uint64_t last_timeout_reset = get_active_last_timeout_reset(ping_resp_received, ping_req_received);
  uint64_t elapsed_time = current_time - last_timeout_reset;

  uint64_t connection_timeout_offset_duration =
      get_connection_timeout_offset_duration(connect_duration,
                                             handler->client_connection_timeout_offset);

  if (elapsed_time >= connection_timeout_offset_duration && ping_req_await_message_type == ANY_MESSAGE_TYPE) {
    // timeout of the connect duration without tolerance
    // no ping request in flight for this client
    // send ping request from the gateway to check client status
    device_address client_address;
    DB_HANDLER_CK_CLIENT_RC(db_get_client_address(handler->db_handler_, &client_address))
    if (send_client_connection_ping_req(handler, &client_address) < 0) {
      return -1;
    }
    return 0;
  }

  uint64_t tolerance_timeout = get_client_tolerance_timeout(connect_duration);
  if (elapsed_time > tolerance_timeout) {
    // tolerance timeout exceeded
    // client is timed out, set client lost
    DB_HANDLER_CK_CLIENT_RC(db_set_client_status(handler->db_handler_, DB_ENTRY_MQTT_SN_CLIENT_STATUS_LOST))
  }
  return 0;
}
uint64_t get_active_last_timeout_reset(uint64_t ping_resp_received, uint64_t ping_req_received) {
  return ping_resp_received > ping_req_received ? ping_resp_received : ping_req_received;
}
uint64_t get_connection_timeout_offset_duration(uint16_t duration, int32_t offset) {
  if (offset < 0 && duration <= offset) {
    return duration;
  }
  return (((int32_t) duration) + offset);
}
uint64_t get_client_tolerance_timeout(uint16_t duration) {
  uint64_t tolerance_part;
  if (duration <= 60) {
    tolerance_part = (uint64_t) ((double) duration * (double) 0.5);
  } else {
    tolerance_part = (uint64_t) ((double) duration * (double) 0.1);
  }
  return tolerance_part + (uint64_t) duration;
}
int32_t send_client_connection_ping_req(MqttSnGatewayClientConnectionHandler *handler, device_address *client_address) {
  if (isFull(handler->clientNetworkSendBuffer)) {
    return 0;
  }
  MqttSnMessageData msg = {0};
  if (get_timestamp_s(&msg.create_time_s)) {
    return -1;
  }

  //msg.from = (*handler->clientNetwork.client_network_address);
  msg.to = (*client_address);

  int32_t gen_rc = generate_ping_req(msg.data, sizeof(msg.data));
  if (gen_rc < 0) {
    return -1;
  }
  msg.data_length = gen_rc;

  if (put(handler->clientNetworkSendBuffer, &msg) < 0) {
    // TODO should never happen
    return 0;
  }
  DB_HANDLER_CK_CLIENT_RC(db_set_ping_req_await_msg_type(handler->db_handler_, PINGRESP))
  // TODO log pinging client XYZ
  // TODO  signal strength dropped?
  return 0;
}
