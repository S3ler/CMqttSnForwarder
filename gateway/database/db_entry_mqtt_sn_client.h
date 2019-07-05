//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_H_

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnMessageParser.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DB_ENTRY_MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT
#define DB_ENTRY_MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT 5
#endif

#ifndef DB_ENTRY_MQTT_SN_CLIENT_MAX_FORWARDER_COUNT
#define DB_ENTRY_MQTT_SN_CLIENT_MAX_FORWARDER_COUNT 5
#endif

typedef enum DB_ENTRY_MQTT_SN_CLIENT_RESULT_ {
  DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS = 0,
  DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_SHORT,
  DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_LONG,
  DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_NOT_FOUND,
  DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR
} DB_ENTRY_MQTT_SN_CLIENT_RESULT;

typedef enum DB_ENTRY_MQTT_SN_CLIENT_STATUS_ {
  DB_ENTRY_MQTT_SN_CLIENT_STATUS_EMPTY = 0,
  DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE = 1,
  DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP = 2,
  DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE = 3,
  DB_ENTRY_MQTT_SN_CLIENT_STATUS_DISCONNECTED = 4,
  DB_ENTRY_MQTT_SN_CLIENT_STATUS_LOST = 5,
} DB_ENTRY_MQTT_SN_CLIENT_STATUS;

typedef enum DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ {
  DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS = 0,
  DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT,
  DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR
} DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS;

typedef struct DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_ {
  MQTT_SN_MESSAGE_TYPE type;
  uint16_t msg_id;
  DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status;
} DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE;

typedef struct DB_ENTRY_MQTT_SN_CLIENT_ {
  char client_id[24];
  device_address client_address;
  uint16_t connect_duration;
  uint64_t last_ping_req_received;
  uint64_t last_ping_resp_received;
  char file_number[9];
  int32_t position;
  DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status;

  MQTT_SN_MESSAGE_TYPE ping_req_await_message_type;
  DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE await_messages[DB_ENTRY_MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT];

  device_address forwarder_addresses[DB_ENTRY_MQTT_SN_CLIENT_MAX_FORWARDER_COUNT];
  uint16_t forwarder_address_len;
} DB_ENTRY_MQTT_SN_CLIENT;

void init_db_entry_mqtt_sn_client_await_message(DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE *await_message);
void init_db_entry_mqtt_sn_client(DB_ENTRY_MQTT_SN_CLIENT *client,
                                  const char *client_id,
                                  device_address *client_address,
                                  device_address *forwarder_addresses,
                                  uint16_t forwarder_address_len,
                                  uint16_t connect_duration,
                                  uint64_t connect_time,
                                  int32_t position);

void reset_db_entry_mqtt_sn_client(DB_ENTRY_MQTT_SN_CLIENT *client,
                                   const char *client_id,
                                   device_address *client_address,
                                   device_address *forwarder_addresses,
                                   uint16_t forwarder_address_len,
                                   uint16_t connect_duration,
                                   uint64_t connect_time);
void set_db_entry_mqtt_sn_client_forwarder_addresses(DB_ENTRY_MQTT_SN_CLIENT *client,
                                                     device_address *forwarder_addresses,
                                                     uint16_t forwarder_address_len);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_H_
