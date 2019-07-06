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

int32_t parse_and_handle_connack(MqttSnClient *client, MqttSnMessageData *msg);
int32_t handle_client_connection(MqttSnClient *client);
int32_t parse_and_handle_ping_resp(MqttSnClient *client, MqttSnMessageData *msg);
int32_t parse_and_handle_ping_req(MqttSnClient *client, MqttSnMessageData *msg);

int32_t send_to_gateway(MqttSnClient *client, const uint8_t *msg_data, int32_t gen_rc);
uint64_t get_last_timeout_reset(MqttSnClient *client);
uint64_t get_tolerance_timeout(uint16_t timeout);
int32_t MqttSnClientInit(MqttSnClient *client, log_level_t log_level, void *gatewayNetworkContext) {
  //memset(client, 0, sizeof(*client));
  client->client_protocol_id = 0x01; // TODO define

  client->default_timeout = MQTT_SN_CLIENT_DEFAULT_TIMEOUT_MS;
  client->default_signal_strength = MQTT_SN_CLIENT_DEFAULT_SIGNAL_STRENGTH;
  client->status = 0;
  client->msg_counter = 7;
  client->connect_duration = MQTT_SN_CLIENT_DEFAULT_CONNECT_DURATION;
  client->connect_timeout_offset = 0;

#ifdef WITH_LOGGING
#if defined(Arduino_h) || defined(WITH_PLATFORMIO)
  if (MqttSnLoggerInit(&mqttSnForwarder->logger, log_level, arduino_serial_log_init) != 0) {
#else
  if (MqttSnLoggerInit(&client->logger, log_level, stdout_log_init) != 0) {
#endif
    MqttSnLoggerDeinit(&client->logger);
    return -1;
  }
  client->gatewayNetwork.logger = &client->logger;
#endif

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

  return 0;
}
int32_t MqttSnClientDeinit(MqttSnClient *client) {
  GatewayNetworkDisconnect(&client->gatewayNetwork, client->gatewayNetworkContext);
  GatewayNetworkDeinitialize(&client->gatewayNetwork, client->gatewayNetworkContext);
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
  msg.data_length = msg_data_length;
  if (memcmp(&msg.from, client->gatewayNetwork.mqtt_sn_gateway_address, sizeof(device_address)) != 0) {
    // TODO parse only advertise and search gw and gw info
    // message is not from gateway
    return 0;
  }
  ParsedMqttSnHeader header = {0};
  int32_t parsed_bytes = 0;
  if (parse_header(&header, ANY_MESSAGE_TYPE, msg.data, msg.data_length, &parsed_bytes) < 0) {
    return 0;
  }
  switch (header.msg_type) {
    case PINGRESP: return parse_and_handle_ping_resp(client, &msg);
    case PINGREQ: return parse_and_handle_ping_req(client, &msg);
      break;
    case ADVERTISE:
      // TODO update observed gateway
      break;
    case CONNACK:return parse_and_handle_connack(client, &msg);
      break;
    default:
      // TODO log unknown message
      break;
  }
  return 0;
}
int32_t parse_and_handle_ping_resp(MqttSnClient *client, MqttSnMessageData *msg) {
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
                                         client->gatewayNetwork.mqtt_sn_gateway_address,
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
int32_t parse_and_handle_ping_req(MqttSnClient *client, MqttSnMessageData *msg) {

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

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientConnect(MqttSnClient *client,
                                               device_address *mqtt_sn_gateway_address,
                                               int32_t connect_timeout_ms,
                                               bool clean_session,
                                               const char *client_id,
                                               uint16_t connect_duration) {
  if (memcmp(mqtt_sn_gateway_address, client->gatewayNetwork.mqtt_sn_gateway_address, sizeof(device_address)) != 0) {
    client->gatewayNetwork.mqtt_sn_gateway_address = mqtt_sn_gateway_address;
  }
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

