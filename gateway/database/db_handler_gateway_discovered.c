//
// Created by SomeDude on 21.06.2019.
//

#include "db_handler.h"
#include "ftfs_db.h"

DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT mqtt_sn_discovered_gateway_init(db_handler *h) {
  if (h->gateway_discovered_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS) {
    return DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS;
  }
  if (h->database_error) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

  for (uint8_t pos = 0; pos < UINT8_MAX; pos++) {
    h->discovered_gw_list.gw[pos].gw_id = pos;
    h->discovered_gw_list.gw[pos].status = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_NEVER;
  }

  for (uint8_t pos = 0; pos < UINT8_MAX; pos++) {
    DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED entry_gw = {0};
    entry_gw.gw_id = pos;
    entry_gw.status = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_NEVER;
    if (f_write_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, pos, 0, &entry_gw, sizeof(entry_gw))
        < 0) {
      return DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    }
  }

#if WITH_DEBUG_LOGGING
  // TODO log success
#endif
  h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS;
  h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_SUCCESS;
  return h->gateway_discovered_result;
}

DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT mqtt_sn_discovered_gateway_update_address(db_handler *h,
                                                                                     uint8_t gw_id,
                                                                                     const device_address *address,
                                                                                     uint64_t received_time,
                                                                                     uint8_t signal_strength,
                                                                                     device_address *forwarder_addresses,
                                                                                     uint16_t forwarder_address_len) {
  if (h->gateway_discovered_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS) {
    return DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS;
  }
  if (h->database_error) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED entry_gw;

  if (f_read_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, 0, &entry_gw, sizeof(entry_gw)) < 0) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

  assert(forwarder_address_len <= DB_ENTRY_MQTT_SN_CLIENT_MAX_FORWARDER_COUNT);

  entry_gw.gw_id = gw_id;
  entry_gw.last_message_time = received_time;
  entry_gw.signal_strength = signal_strength;
  entry_gw.network_address = *address;
  entry_gw.forwarder_address_len = forwarder_address_len;
  for (uint16_t i = 0; i < forwarder_address_len; i++) {
    entry_gw.forwarder_addresses[i] = forwarder_addresses[i];
  }

  if (f_write_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, 0, &entry_gw, sizeof(entry_gw)) < 0) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

#if WITH_DEBUG_LOGGING
  // TODO log success
#endif
  h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS;
  h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_SUCCESS;
  return h->gateway_discovered_result;
}
DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT mqtt_sn_discovered_gateway_update(db_handler *h,
                                                                             uint8_t gw_id,
                                                                             uint16_t duration,
                                                                             const device_address *address,
                                                                             uint8_t signal_strength,
                                                                             uint64_t received_time,
                                                                             device_address *forwarder_addresses,
                                                                             uint16_t forwarder_address_len) {
  if (h->gateway_discovered_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS) {
    return DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS;
  }
  if (h->database_error) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED entry_gw;

  if (f_read_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, 0, &entry_gw, sizeof(entry_gw)) < 0) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

  assert(forwarder_address_len <= DB_ENTRY_MQTT_SN_CLIENT_MAX_FORWARDER_COUNT);
  entry_gw.gw_id = gw_id;
  entry_gw.duration = duration;
  entry_gw.duration_timeout = duration;
  entry_gw.last_message_time = received_time;
  entry_gw.signal_strength = signal_strength;
  entry_gw.network_address = *address;
  entry_gw.status = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_ACTIVE;
  entry_gw.forwarder_address_len = forwarder_address_len;
  for (uint16_t i = 0; i < forwarder_address_len; i++) {
    entry_gw.forwarder_addresses[i] = forwarder_addresses[i];
  }

  h->discovered_gw_list.gw[gw_id].status = entry_gw.status;

  if (f_write_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, 0, &entry_gw, sizeof(entry_gw)) < 0) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

#if WITH_DEBUG_LOGGING
  // TODO log success
#endif
  h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS;
  h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_SUCCESS;
  return h->gateway_discovered_result;
}

DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT get_gateway_discovered_result(db_handler *h) {
  return h->gateway_discovered_result;
}

DB_HANDLER_RESULT get_gateway_discovered_transaction_result(db_handler *h) {
  return h->gateway_discovered_transaction_result;
}

DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_STATUS get_gateway_status(db_handler *handler, uint8_t gw_id) {
  return handler->discovered_gw_list.gw[gw_id].status;
}
