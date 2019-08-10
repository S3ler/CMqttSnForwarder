//
// Created by SomeDude on 31.05.2019.
//

#include "MqttSnClient.h"
#include "MqttSnClientAwaitMessage.h"
#include "MqttSnClientLogger.h"
#include <string.h>
#include <stdbool.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <parser/MqttSnPublishMessage.h>
#include <parser/MqttSnConnectMessage.h>
#include <parser/MqttSnConnackMessage.h>
#include <parser/MqttSnPingReqMessage.h>
#include <parser/MqttSnPingRespMessage.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/MqttSnDisconnectMessage.h>

int32_t parse_and_handle_disconnect(MqttSnClient *client, MqttSnMessageData *msg);
int32_t parse_and_handle_connack(MqttSnClient *client, MqttSnMessageData *msg);
int32_t handle_client_connection(MqttSnClient *client);
int32_t client_parse_and_handle_ping_resp(MqttSnClient *client, MqttSnMessageData *msg);
int32_t client_parse_and_handle_ping_req(MqttSnClient *client, MqttSnMessageData *msg);

int32_t send_to_gateway(MqttSnClient *client, const uint8_t *msg_data, int32_t gen_rc);
uint64_t get_last_timeout_reset(MqttSnClient *client);
uint64_t get_tolerance_timeout(uint16_t timeout);
int32_t MqttSnClientInit(MqttSnClient *client, const MqttSnLogger *logger, void *gatewayNetworkContext) {
  //memset(client, 0, sizeof(*client));
  client->client_protocol_id = 0x01; // TODO define

  client->default_timeout = MQTT_SN_CLIENT_DEFAULT_TIMEOUT_MS;
  client->default_signal_strength = MQTT_SN_CLIENT_DEFAULT_SIGNAL_STRENGTH;
  client->status = 0;
  client->msg_counter = 7;
  client->connect_duration = MQTT_SN_CLIENT_DEFAULT_CONNECT_DURATION;
  client->connect_timeout_offset = 0;

  client->logger = (*logger);
  client->gatewayNetwork.logger = &client->logger;

  client->gatewayNetworkContext = gatewayNetworkContext;

  if (GatewayNetworkConnect(&client->gatewayNetwork, client->gatewayNetworkContext) != 0) {
    return -1;
  }

  for (uint16_t i = 0; i < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
    client->await_msg[i].msg_type = ANY_MESSAGE_TYPE;
    client->await_msg[i].msg_id = 0;
  }

  client->ping_req_await_msg.msg_type = ANY_MESSAGE_TYPE;
  client->ping_req_await_msg.msg_id = 0;

  client->adv_cb = global_mqtt_sn_client_adv_cb;
  client->gwinfo_cb = global_mqtt_sn_client_gwinfo_cb;
  return 0;
}
int32_t MqttSnClientDeinit(MqttSnClient *client) {
  GatewayNetworkDisconnect(&client->gatewayNetwork, client->gatewayNetworkContext);
  client->status = 0;
  return 0;
}
int32_t MqttSnClientLoop(MqttSnClient *client) {

  if (handle_client_connection(client) < 0) {
    return -1;
  }

  MqttSnMessageData msg = {0};
  int32_t msg_data_length = 0;
  if ((msg_data_length = GatewayNetworkReceiveFom(&client->gatewayNetwork,
                                                  &msg.from,
                                                  &msg.to,
                                                  msg.data,
                                                  sizeof(msg.data),
                                                  &msg.signal_strength,
                                                  client->gatewayNetworkReceiveTimeout,
                                                  client->gatewayNetworkContext)) < 0) {
    return -1;
  }
  if (msg_data_length == 0) {
    return 0;
  }
  msg.data_length = msg_data_length;
  if (memcmp(&msg.from, &client->mqtt_sn_gateway_address, sizeof(device_address)) != 0) {
    // TODO parse only advertise and search gw and gw info
    // message is not from gateway
    return 0;
  }
  ParsedMqttSnHeader header = {0};
  int32_t parsed_bytes = 0;
  if (parse_header(&header, ANY_MESSAGE_TYPE, msg.data, msg.data_length, &parsed_bytes) < 0) {
    return 0;
  }
  if (!client->connected) {
    switch (header.msg_type) {
      case PINGRESP: break; // TODO log ping resp ignored
      case PINGREQ: break;// TODO log pingrequest ignored
      case ADVERTISE:return parse_and_handle_advertise(client, &msg);// TODO update and maintain observed gateway
      case GWINFO:return parse_and_handle_gwinfo(client, &msg);// TODO update and maintain observed gateway
      case CONNACK:return parse_and_handle_connack(client, &msg);
      case DISCONNECT:return parse_and_handle_disconnect(client, &msg);
      default:
        // TODO log unknown message
        break;
    }
  } else {
    switch (header.msg_type) {
      case PINGRESP: return client_parse_and_handle_ping_resp(client, &msg);
      case PINGREQ: return client_parse_and_handle_ping_req(client, &msg);
      case ADVERTISE:return parse_and_handle_advertise(client, &msg);// TODO update and maintain observed gateway
      case GWINFO:return parse_and_handle_gwinfo(client, &msg);// TODO update and maintain observed gateway
      case CONNACK:return parse_and_handle_connack(client, &msg);
      case DISCONNECT:return parse_and_handle_disconnect(client, &msg);
      default:
        // TODO log unknown message
        break;
    }
  }

  return 0;
}
int32_t client_parse_and_handle_ping_resp(MqttSnClient *client, MqttSnMessageData *msg) {
  int32_t await_fd = 0;
  if (client->ping_req_await_msg.msg_type != PINGRESP && (await_fd = is_await_msg(client, PINGREQ, 0)) < 0) {
    return 0;
  }
  int32_t parse_rc = parse_ping_resp_byte(msg->data, msg->data_length);
  if (parse_rc < 0) {
    return 0;
  }

  if (client->ping_req_await_msg.msg_type == PINGRESP) {
    // answer to a send ping req
    client->ping_req_await_msg.msg_type = ANY_MESSAGE_TYPE;
    if (get_timestamp_s(&client->last_ping_resp_received) < 0) {
      return -1;
    }
    return 0;
  }

  if (await_fd < 0) {
    // we do not await this message - so we ignore it
    return 0;
  }

  // answer to a MqttSnPingGateway oder MqttSnPing
  if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
    return -1;
  }
  if (free_await_msg(client, await_fd) < 0) {
    return -1;
  }

  // TODO should never happen
  // TODO log

  return 0;
}
int32_t send_to_gateway(MqttSnClient *client, const uint8_t *msg_data, int32_t gen_rc) {
  int32_t send_rc = GatewayNetworkSendTo(&client->gatewayNetwork,
                                         &client->mqtt_sn_gateway_address,
                                         msg_data,
                                         gen_rc,
                                         client->default_signal_strength,
                                         client->gatewayNetworkSendTimeout,
                                         client->gatewayNetworkContext);
  return send_rc;
}

int32_t handle_client_connection(MqttSnClient *client) {
  if (!client->connected) {
    return 0;
  }

  uint64_t current_time = 0;
  if (get_timestamp_s(&current_time) < 0) {
    return -1;
  }
  uint64_t elapsed_time = current_time - get_last_timeout_reset(client);

  if (client->ping_req_await_msg.msg_type == PINGRESP) {
    // ping request already in flight

    // check if ping request timed out
    if (elapsed_time > get_tolerance_timeout(client->connect_duration)) {
      client->connected = false;
#if WITH_LOGGING
      log_mqtt_sn_client_connection_status(&client->logger,
                                           client->connected,
                                           client->gatewayNetwork.mqtt_sn_gateway_address);
#endif
    }
    return 0;
  }

  /*
  uint64_t tolerance_timeout = client->connect_duration;
  if (tolerance_timeout <= 60) {
    tolerance_timeout = (uint64_t) ((double) tolerance_timeout * (double) 1.5);
  }else{
    tolerance_timeout = (uint64_t) ((double) tolerance_timeout * (double) 1.1);
  }
  */

  if (client->connect_duration <= MQTT_SN_CLIENT_PING_REQ_ADVANCE_TIME_S) {
    if (elapsed_time < client->connect_duration) {
      return 0;
    }
  } else {
    if (elapsed_time < (uint64_t) client->connect_duration + client->connect_timeout_offset) {
      return 0;
    }
  }

  uint8_t msg_data[MQTT_SN_MESSAGE_PINGREQ_LENGTH] = {0};
  int32_t gen_rc = generate_ping_req(msg_data, sizeof(msg_data));
  if (gen_rc < 0) {
    // should never happen
    // TODO log
    return -1;
  }
  int32_t send_rc = send_to_gateway(client, msg_data, gen_rc);

  if (send_rc < 0 || send_rc != gen_rc) {
    // TODO log etc
    return -1;
  }
  client->ping_req_await_msg.msg_type = PINGRESP;

  return 0;
}
uint64_t get_tolerance_timeout(uint16_t timeout) {
  uint64_t result = timeout;
  if (timeout <= 60) {
    result += (uint64_t) ((double) timeout * (double) 0.5);
  } else {
    result += (uint64_t) ((double) timeout * (double) 0.1);
  }
  return result;
}
uint64_t get_last_timeout_reset(MqttSnClient *client) {
  uint64_t result = client->last_ping_resp_received;
  if (client->last_ping_req_received > client->last_ping_resp_received) {
    result = client->last_ping_req_received;
  }
  return result;
}
int32_t parse_and_handle_connack(MqttSnClient *client, MqttSnMessageData *msg) {
  int32_t await_fd;
  if ((await_fd = is_await_msg(client, CONNACK, 0)) < 0) {
    return 0;
  }
  ParsedMqttSnConnack mqtt_sn_connack = {0};
  if (parse_connack(&mqtt_sn_connack, msg->data, msg->data_length) < 0) {
    // TODO log
    return 0;
  }
  client->connect_return_code = mqtt_sn_connack.return_code;
  if (free_await_msg(client, await_fd) < 0) {
    return -1;
  }

  uint64_t current_time;
  if (get_timestamp_s(&current_time) < 0) {
    return -1;
  }
  client->last_ping_req_received = current_time;
  client->last_ping_resp_received = current_time;

  return 0;
}
int32_t client_parse_and_handle_ping_req(MqttSnClient *client, MqttSnMessageData *msg) {

  int32_t parse_rc = parse_ping_req_byte(msg->data, msg->data_length);
  if (parse_rc < 0) {
    return 0;
  }

  uint8_t msg_data[MQTT_SN_MESSAGE_PINGREQ_LENGTH] = {0};
  int32_t gen_rc = generate_ping_resp(msg_data, sizeof(msg_data));
  if (gen_rc < 0) {
    return -1;
  }

  if (get_timestamp_s(&client->last_ping_req_received) < 0) {
    return -1;
  }

  int32_t send_rc = send_to_gateway(client, msg_data, gen_rc);
  if (send_rc < 0 || send_rc != gen_rc) {
    // TODO log etc
    return -1;
  }

  // TODO should never happen
  // TODO log

  return 0;

}
int32_t parse_and_handle_disconnect(MqttSnClient *client, MqttSnMessageData *msg) {
  int32_t await_fd;
  if ((await_fd = is_await_msg(client, DISCONNECT, 0)) < 0) {
    // out of order disconnect
    client->connected = false;
    // TODO log
    return 0;
  }
  // awaited disconnect
  client->connected = false;
  if (free_await_msg(client, await_fd) < 0) {
    return -1;
  }
  return 0;
}
int32_t parse_and_handle_advertise(MqttSnClient *client, MqttSnMessageData *msg) {
  MqttSnAdvertise mqtt_sn_advertise = {0};
  if (parse_advertise(&mqtt_sn_advertise, msg->data, msg->data_length) < 0) {
    return 0;
  }
  MqttSnClientReceivedAdvertise received_advertise = {
      .received_advertise = mqtt_sn_advertise,
      .from = msg->from,
      .signal_strength = msg->signal_strength
  };

  int32_t await_fd;
  if ((await_fd = is_await_msg(client, ADVERTISE, 0)) < 0) {
    if (client->adv_cb(client, &received_advertise, NULL) < 0) {
      return -1;
    }
  } else {
    if (client->adv_cb(client, &received_advertise, client->adv_cb_context) < 0) {
      return -1;
    }
    if (free_await_msg(client, await_fd) < 0) {
      return -1;
    }
  }
}
int32_t parse_and_handle_gwinfo(MqttSnClient *client, MqttSnMessageData *msg) {

  MqttSnGwInfo mqtt_sn_gwinfo = {0};
  if (parse_gwinfo(&mqtt_sn_gwinfo, msg->data, msg->data_length) < 0) {
    return 0;
  }
  MqttSnClientReceivedGwInfo received_gw_info = {
      .received_gw_info = mqtt_sn_gwinfo,
      .from = msg->from,
      .signal_strength = msg->signal_strength
  };

  int32_t await_fd;
  if ((await_fd = is_await_msg(client, GWINFO, 0)) < 0) {
    if (client->gwinfo_cb(client, &received_gw_info, NULL) < 0) {
      return -1;
    }
  } else {
    if (client->gwinfo_cb(client, &received_gw_info, client->gwinfo_cb_context) < 0) {
      return -1;
    }
    if (free_await_msg(client, await_fd) < 0) {
      return -1;
    }
  }
  return 0;
}

int32_t MqttSnClientSubscribe(MqttSnClient *client,
                              const char *topic_name,
                              int32_t (*publish_cb)(MqttSnClient, const char *, uint16_t, uint8_t *, uint16_t)) {
  return -1;
}
int32_t MqttSnClientPublishPredefinedM1(MqttSnClient *client,
                                        uint16_t predefined_topic_id,
                                        uint8_t retain,
                                        uint8_t *data,
                                        uint16_t data_len) {
  uint8_t msg_data[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
  memset(&msg_data, 0, sizeof(msg_data));

  int32_t gen_rc = generate_publish(msg_data,
                                    sizeof(msg_data),
                                    0,
                                    -1,
                                    retain,
                                    MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID,
                                    predefined_topic_id,
                                    client->msg_counter++,
                                    data,
                                    data_len);
  if (gen_rc < 0) {
    return -1;
  }

  int32_t send_rc = send_to_gateway(client, msg_data, gen_rc);
  if (send_rc < 0 || send_rc != gen_rc) {
    // TODO log etc
    return -1;
  }

  return send_rc;
}

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnect(MqttSnClient *client,
                                                     device_address *mqtt_sn_gateway_address,
                                                     int32_t connect_timeout_ms,
                                                     uint8_t clean_session,
                                                     const char *client_id,
                                                     uint16_t connect_duration) {
  client->mqtt_sn_gateway_address = (*mqtt_sn_gateway_address);
  client->gatewayNetwork.mqtt_sn_gateway_address = &client->mqtt_sn_gateway_address;

  int32_t client_id_len;
  if ((client_id_len = MqttSnClientSafeStrlen(client_id, MQTT_SN_MAX_CLIENT_ID_LENGTH)) < 0) {
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  strncpy(client->client_id_str, client_id, client_id_len);

  /*
  if (memcmp(mqtt_sn_gateway_address, client->gatewayNetwork.mqtt_sn_gateway_address, sizeof(device_address)) != 0) {
    client->gatewayNetwork.mqtt_sn_gateway_address = mqtt_sn_gateway_address;
  }
  */
  client->connect_duration = connect_duration;

  uint8_t msg_data[MQTT_SN_MESSAGE_CONNECT_MAX_LENGTH] = {0};
  // TODO protocol id as constant
  int32_t gen_rc = generate_connect(msg_data,
                                    sizeof(msg_data),
                                    false,
                                    clean_session,
                                    client->client_protocol_id,
                                    client->connect_duration,
                                    client_id);
  if (gen_rc < 0) {
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }

  int32_t await_fd = -1;
  while ((await_fd = new_await_msg(client, CONNACK, 0)) < 0) {
    if (MqttSnClientLoop(client) < 0) {
      return MQTT_SN_CLIENT_RETURN_ERROR;
    }
  }
  if (client->connected) {
    client->connected = false;
  }
  // TODO default_signal_strength shall be the signal strengh received by the advertisement layer or something
  // TODO so the strength of the signal adapts to the gateway distance
  uint64_t connect_send_time = 0;
  if (get_timestamp_s(&connect_send_time) < 0) {
    return -1;
  }

  int32_t send_rc = send_to_gateway(client, msg_data, gen_rc);
  if (send_rc < 0 || send_rc != gen_rc) {
    // TODO log etc
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }

  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc;
  while ((await_rc = get_await_status(client, await_fd)) == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
    uint64_t current_time = 0;
    if (get_timestamp_s(&current_time) < 0) {
      return -1;
    }
    if (MqttSnClientLoop(client) < 0) {
      return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (connect_timeout_ms > 0) {
      uint64_t elapsed_time = current_time - connect_send_time;
      if (elapsed_time > (uint64_t) (connect_timeout_ms / 1000)) {
        // timeout
        break;
      }
    } else if (connect_timeout_ms == 0) {
      break;
    }
  }
  if (await_rc != MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
    if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
      return MQTT_SN_CLIENT_RETURN_TIMEOUT;
    }
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  switch (client->connect_return_code) {
    case RETURN_CODE_ACCEPTED: {
      client->connected = true;
#if WITH_LOGGING
      log_mqtt_sn_client_connection_status(&client->logger,
                                           client->connected,
                                           client->gatewayNetwork.mqtt_sn_gateway_address);
#endif
      return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    case RETURN_CODE_REJECTED_CONGESTION:return MQTT_SN_CLIENT_RETURN_CONJESTION;
    case RETURN_CODE_REJCETED_NOT_SUPPORTED:return MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED;
    default:break;
  }
  return MQTT_SN_CLIENT_RETURN_ERROR;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAwaitAdvertise(MqttSnClient *client,
                                                      int32_t timeout_s,
                                                      MqttSnClientReceivedAdvertise *dst_advertise) {
  int32_t await_fd = -1;
  while ((await_fd = new_await_msg(client, ADVERTISE, 0)) < 0) {
    if (MqttSnClientLoop(client) < 0) {
      return MQTT_SN_CLIENT_RETURN_ERROR;
    }
  }

  client->adv_cb_context = dst_advertise;
  uint64_t start_ts = 0;
  if (get_timestamp_s(&start_ts) < 0) {
    client->adv_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  int32_t random_ts = 5000; // TODO get random

  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms(client, await_fd, start_ts, random_ts);

  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
    client->adv_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_TIMEOUT;
  }
  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR) {
    client->adv_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  client->adv_cb_context = NULL;
  return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAwaitAdvertiseCallback(MqttSnClient *client,
                                                              int32_t timeout_s,
                                                              void *context,
                                                              int32_t (*adv_cb)(const MqttSnClient *,
                                                                                const MqttSnClientReceivedAdvertise *,
                                                                                void *)) {
  // TODO implement me
  return MQTT_SN_CLIENT_RETURN_ERROR;
}
int32_t global_mqtt_sn_client_gwinfo_cb(MqttSnClient *client,
                                        MqttSnClientReceivedGwInfo *rec_gw_info,
                                        void *context) {
  if (context == NULL) {
    // TODO MqttSnCLientUpdateGateway(client,rec_gw_info);
    return 0;
  }
  MqttSnClientReceivedGwInfo *dst_gwinfo = (MqttSnClientReceivedGwInfo *) context;
  (*dst_gwinfo) = (*rec_gw_info);
  return 0;
}
int32_t global_mqtt_sn_client_adv_cb(MqttSnClient *client,
                                     MqttSnClientReceivedAdvertise *rec_advertise,
                                     void *context) {
  if (context == NULL) {
    // TODO MqttSnCLientUpdateGateway(client,rec_gw_info);
    return 0;
  }
  MqttSnClientReceivedAdvertise *dst_advertise = (MqttSnClientReceivedAdvertise *) context;
  (*dst_advertise) = (*rec_advertise);
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSearchGw(MqttSnClient *client,
                                                int32_t timeout_ms,
                                                uint8_t radius,
                                                MqttSnClientReceivedGwInfo *dst_gwinfo) {
  // wait random time - if now gw info received send searchgw - then wait until timeout_ms
  int32_t await_fd = -1;
  while ((await_fd = new_await_msg(client, GWINFO, 0)) < 0) {
    if (MqttSnClientLoop(client) < 0) {
      return MQTT_SN_CLIENT_RETURN_ERROR;
    }
  }

  client->gwinfo_cb_context = dst_gwinfo;
  uint64_t start_ts = 0;
  if (get_timestamp_s(&start_ts) < 0) {
    return -1;
  }
  int32_t random_ts = 5000; // TODO get random

  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms(client, await_fd, start_ts, random_ts);
  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
    client->gwinfo_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
  }
  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR) {
    client->gwinfo_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }

  while ((await_fd = new_await_msg(client, GWINFO, 0)) < 0) {
    if (MqttSnClientLoop(client) < 0) {
      return MQTT_SN_CLIENT_RETURN_ERROR;
    }
  }
  uint8_t msg_data[MQTT_SN_MESSAGE_SEARCHGW_LENGTH] = {0};
  int32_t gen_rc = generate_searchgw_message(msg_data, sizeof(msg_data), radius);
  if (gen_rc < 0) {
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  client->gwinfo_cb_context = dst_gwinfo;
  uint64_t send_time = 0;
  if (get_timestamp_s(&send_time) < 0) {
    return -1;
  }
  int32_t send_rc = send_to_gateway(client, msg_data, gen_rc);
  if (send_rc < 0 || send_rc != gen_rc) {
    client->gwinfo_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }

  await_rc = get_await_status_timeout_ms(client, await_fd, start_ts, timeout_ms);

  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
    client->gwinfo_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_TIMEOUT;
  }
  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR) {
    client->gwinfo_cb_context = NULL;
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  client->gwinfo_cb_context = NULL;
  return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
int32_t MqttSnClientSafeStrlen(const char *str, int32_t max_len) {
  const char *end = (const char *) memchr(str, '\0', max_len);
  if (end == NULL)
    return -1;
  else
    return end - str;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDisconnect(MqttSnClient *client) {
  int32_t await_fd = -1;
  while ((await_fd = new_await_msg(client, DISCONNECT, 0)) < 0) {
    if (MqttSnClientLoop(client) < 0) {
      return MQTT_SN_CLIENT_RETURN_ERROR;
    }
  }

  uint8_t msg_data[MQTT_SN_MESSAGE_DISCONNECT_MIN_LENGTH] = {0};
  int32_t gen_rc = generate_disconnect_message(msg_data, sizeof(msg_data));
  if (gen_rc < 0) {
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  uint64_t start_ts = 0;
  if (get_timestamp_s(&start_ts) < 0) {
    return -1;
  }
  int32_t send_rc = send_to_gateway(client, msg_data, gen_rc);
  if (send_rc < 0 || send_rc != gen_rc) {
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }

  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms(client,
                                                                             await_fd,
                                                                             start_ts,
                                                                             client->default_timeout);
  client->connected = false;
  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
    return MQTT_SN_CLIENT_RETURN_TIMEOUT;
  }
  if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR) {
    return MQTT_SN_CLIENT_RETURN_ERROR;
  }
  return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicName(MqttSnClient *client,
                                                        const char *topic_name,
                                                        uint16_t topic_name_length,
                                                        uint8_t retain,
                                                        int8_t qos,
                                                        uint8_t *data,
                                                        uint16_t data_len) {
  // TODO implement me
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishPredefined(MqttSnClient *client,
                                                         uint16_t predefined_topic_id,
                                                         uint8_t retain,
                                                         int8_t qos,
                                                         uint8_t *data,
                                                         uint16_t data_len) {
  // TODO implement me
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicId(MqttSnClient *client,
                                                      uint16_t short_topic_id,
                                                      uint8_t retain,
                                                      int8_t qos,
                                                      uint8_t *data,
                                                      uint16_t data_len) {
  // TODO implement me
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRegister(MqttSnClient *client, const char *topic_name) {
  // TODO implement me
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribePredefined(MqttSnClient *client, int8_t qos) {
  // TODO implement me
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeShort(MqttSnClient *client, int8_t qos) {
  // TODO implement me
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeTopicName(MqttSnClient *client, const char *topic_name) {
  // TODO implement me
  return 0;
}

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSleep(MqttSnClient *client, uint16_t sleep_duration) {
  // TODO implement me
  return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientWakeup(MqttSnClient *client) {
  // TODO implement me
  return 0;
}



