//
// Created by SomeDude on 29.06.2019.
//
#include "db_entry_mqtt_sn_client.h"

void init_db_entry_mqtt_sn_client_await_message(DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE *await_message) {
  await_message->type = ANY_MESSAGE_TYPE;
  await_message->msg_id = 0;
  await_message->status = DB_ENTRY_MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS;
}

void init_db_entry_mqtt_sn_client(DB_ENTRY_MQTT_SN_CLIENT *client,
                                  const char *client_id,
                                  device_address *client_address,
                                  device_address *forwarder_addresses,
                                  uint16_t forwarder_address_len,
                                  uint16_t connect_duration,
                                  uint64_t connect_time,
                                  int32_t position) {
  assert(strlen(client_id) >= MQTT_SN_CLIENTID_MIN_LENGTH && strlen(client_id) <= MQTT_SN_CLIENTID_MAX_LENGTH);
  assert(position >= 0);
  memset(client, 0x0, sizeof(DB_ENTRY_MQTT_SN_CLIENT));

  strcpy(client->client_id, client_id);
  client->client_address = (*client_address);
  client->connect_duration = connect_duration;
  client->last_ping_req_received = connect_time;
  client->last_ping_resp_received = connect_time;
  sprintf(client->file_number, "%08d", position);
  client->position = position;
  client->ping_req_await_message_type = ANY_MESSAGE_TYPE;
  client->client_status = DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE;
  for (uint16_t i = 0; i < DB_ENTRY_MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
    init_db_entry_mqtt_sn_client_await_message(&client->await_messages[i]);
  }
  set_db_entry_mqtt_sn_client_forwarder_addresses(client, forwarder_addresses, forwarder_address_len);
}
void reset_db_entry_mqtt_sn_client(DB_ENTRY_MQTT_SN_CLIENT *client,
                                   const char *client_id,
                                   device_address *client_address,
                                   device_address *forwarder_addresses,
                                   uint16_t forwarder_address_len,
                                   uint16_t connect_duration,
                                   uint64_t connect_time) {
  assert(strlen(client_id) >= MQTT_SN_CLIENTID_MIN_LENGTH && strlen(client_id) <= MQTT_SN_CLIENTID_MAX_LENGTH - 1);

  strcpy(client->client_id, client_id);
  client->client_address = (*client_address);
  client->connect_duration = connect_duration;
  client->last_ping_req_received = connect_time;
  client->last_ping_resp_received = connect_time;
  client->ping_req_await_message_type = ANY_MESSAGE_TYPE;
  client->client_status = DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE;
  set_db_entry_mqtt_sn_client_forwarder_addresses(client, forwarder_addresses, forwarder_address_len);
}
void set_db_entry_mqtt_sn_client_forwarder_addresses(DB_ENTRY_MQTT_SN_CLIENT *client,
                                                     device_address *forwarder_addresses,
                                                     uint16_t forwarder_address_len) {
  assert(forwarder_address_len <= DB_ENTRY_MQTT_SN_CLIENT_MAX_FORWARDER_COUNT);

  client->forwarder_address_len = forwarder_address_len;
  for (uint16_t i = 0; i < forwarder_address_len; i++) {
    client->forwarder_addresses[i] = forwarder_addresses[i];
  }
}
void get_db_entry_mqtt_sn_client_forwarder_addresses(DB_ENTRY_MQTT_SN_CLIENT *client,
                                                     device_address *forwarder_addresses,
                                                     uint16_t *forwarder_address_len,
                                                     uint16_t forwarder_address_max_len) {
  assert(forwarder_address_max_len <= DB_ENTRY_MQTT_SN_CLIENT_MAX_FORWARDER_COUNT);

  (*forwarder_address_len) = client->forwarder_address_len;
  for (uint16_t i = i; i < client->forwarder_address_len; i++) {
    forwarder_addresses[i] = client->forwarder_addresses[i];
  }
}
