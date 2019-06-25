//
// Created by SomeDude on 21.06.2019.
//

#include "db_handler.h"
#include "ftfs_db.h"

DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT mqtt_sn_discovered_gateway_update_address(db_handler *h,
                                                                                     uint8_t gw_id,
                                                                                     const device_address *address,
                                                                                     uint64_t received_time) {
  if (h->gateway_discovered_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS) {
    return DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS;
  }
  if (h->database_error) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED entry_gw;

  if (f_read_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, &entry_gw, sizeof(entry_gw)) < 0) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;  }

  entry_gw.gw_id = gw_id;
  entry_gw.last_message_time = received_time;
  entry_gw.network_address = *address;

  if (f_write_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, &entry_gw, sizeof(entry_gw)) < 0) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;  }

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
                                                                             uint64_t received_time) {
  if (h->gateway_discovered_result == DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS) {
    return DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS;
  }
  if (h->database_error) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED entry_gw;

  if (f_read_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, &entry_gw, sizeof(entry_gw)) < 0) {
    h->gateway_discovered_result = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    h->gateway_discovered_transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->gateway_discovered_result;
  }

  entry_gw.gw_id = gw_id;
  entry_gw.duration = duration;
  entry_gw.duration_timeout = duration;
  entry_gw.last_message_time = received_time;
  entry_gw.network_address = *address;
  entry_gw.status = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_ACTIVE;

  if (f_write_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, gw_id, &entry_gw, sizeof(entry_gw)) < 0) {
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
