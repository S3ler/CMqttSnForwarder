//
// Created by SomeDude on 21.06.2019.
//
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <gateway/database/logging/db_logging.h>
#include "db_handler.h"
#include "ftfs_db.h"

bool db_is_client_transaction_started(db_handler *h) {
  return h->client_transaction_started;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_transaction_result(db_handler *h) {
  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_start_client_transaction(db_handler *h,
                                                           const device_address *address,
                                                           const char *client_id) {
  assert(address != NULL || client_id != NULL);
  assert(!h->client_transaction_started);

  h->transaction_result = DB_HANDLER_RESULT_SUCCESS;
  if (h->database_error) {
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
#if WITH_DB_DEBUG_LOGGING
    log_database_error(h->logger, NULL);
#endif
    return h->client_transaction_result;
  }

  h->client_transaction_started = true;

  if (f_open(&h->_client_registry_file, MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME, FA_READ) != FR_OK) {
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
#if WITH_DB_DEBUG_LOGGING
    log_database_error(h->logger, NULL);
#endif
    return h->client_transaction_result;
  }

  uint32_t line_number = 0;
  UINT readChars = 0;
  do {
    memset(&h->_entry_client, 0, sizeof(DB_ENTRY_MQTT_SN_CLIENT));
    uint16_t buffer_size = sizeof(DB_ENTRY_MQTT_SN_CLIENT);

    if (f_read(&h->_client_registry_file, &h->_entry_client, buffer_size, &readChars) != FR_OK) {
      h->transaction_result = DB_HANDLER_RESULT_ERROR;
      h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
#if WITH_DB_DEBUG_LOGGING
      log_database_error(h->logger, NULL);
#endif
      return h->client_transaction_result;
    }

    bool client_found = false;
    if (address) {
      if (readChars == buffer_size &&
          (strlen(h->_entry_client.client_id) < MQTT_SN_CLIENTID_MAX_LENGTH) &&
          (memcmp(&h->_entry_client.client_address, address, sizeof(device_address)) == 0)) {
        client_found = true;
      }
    } else {
      if (readChars == buffer_size &&
          (strlen(h->_entry_client.client_id) < MQTT_SN_CLIENTID_MAX_LENGTH) &&
          (strcmp(h->_entry_client.client_id, client_id) == 0)) {
        client_found = true;
      }
    }
    if (client_found) {
#if WITH_DB_DEBUG_LOGGING
      log_start_client_transaction(h->logger,
                                   h->transaction_result,
                                   address,
                                   h->client_transaction_result,
                                   h->_entry_client.client_id,
                                   h->_entry_client.file_number);
#endif
      return h->client_transaction_result;
    }
    line_number++;
  } while (readChars > 0);
  h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_NOT_FOUND;
#if WITH_DB_DEBUG_LOGGING
  log_start_client_transaction(h->logger,
                               h->transaction_result,
                               address,
                               h->client_transaction_result,
                               h->_entry_client.client_id,
                               h->_entry_client.file_number);
#endif
  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_end_client_transaction(db_handler *h) {
  assert(h->client_transaction_started);

  if (write_db_entry_client(&h->_client_registry_file, &h->_entry_client) < 0) {
#if WITH_DB_DEBUG_LOGGING
    log_database_error(h->logger, NULL);
    // TODO log write db entry client error
#endif
    h->client_transaction_started = false;
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->database_error = true;
    return h->client_transaction_result;
  }

  if (f_close(&h->_client_registry_file) != FR_OK) {
#if WITH_DB_DEBUG_LOGGING
    log_database_error(h->logger, NULL);
    // TODO log db error
#endif
    h->client_transaction_started = false;
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->database_error = true;
    return h->client_transaction_result;
  }
#if WITH_DB_DEBUG_LOGGING
  // TODO uncommend later
  log_apply_client_transaction(h->logger, h->transaction_result, h->client_transaction_result, h->database_error);
#endif
  // TODO close all files
  // TODO check what happens if we close file twice
  h->client_transaction_started = false;
  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT add_client(db_handler *h,
                                          const char *client_id,
                                          device_address *client_address,
                                          device_address *forwarder_addresses,
                                          uint16_t forwarder_address_len,
                                          uint16_t connect_duration,
                                          uint64_t connect_time) {
  assert(h->client_transaction_started);

  if (h->database_error) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->client_transaction_result;
  }
  if (h->client_transaction_result != DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_NOT_FOUND) {
    if (h->client_transaction_result != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS)
      return h->client_transaction_result;
  }

  if (strlen(client_id) < MQTT_SN_CLIENTID_MIN_LENGTH) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_SHORT;
    return h->client_transaction_result;
  }
  if (strlen(client_id) >= MQTT_SN_CLIENTID_MAX_LENGTH) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_LONG;
    return h->client_transaction_result;
  }

  int32_t empty_space = find_empty_entry_space_in_registry(&h->_entry_client,
                                                           &h->_client_registry_file,
                                                           MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME);
  if (empty_space < 0) {
#if WITH_DB_DEBUG_LOGGING
    // TODO log error
#endif
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->database_error = true;
    return h->client_transaction_result;
  }

  init_db_entry_mqtt_sn_client(&h->_entry_client,
                               client_id,
                               client_address,
                               forwarder_addresses,
                               forwarder_address_len,
                               connect_duration,
                               connect_time,
                               empty_space);

  if (h->client_count < (uint64_t) empty_space + 1) {
    h->client_count = (uint64_t) empty_space + 1;
  }

  /* // TODO files will be created if needed
  char filename_with_extension[
      sizeof(h->_entry_client.file_number.file_number) + sizeof(MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING)];

  // registration file
  memset(&filename_with_extension,
         0,
         sizeof(h->_entry_client.file_number) + sizeof(MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING));
  memcpy(&filename_with_extension, &h->_entry_client.file_number, strlen(h->_entry_client.file_number));
  memcpy(&filename_with_extension[strlen(h->_entry_client.file_number)], MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING));
  create_file(filename_with_extension);

  // subscription file
  memset(&filename_with_extension,
         0,
         sizeof(h->_entry_client.file_number) + sizeof(MQTT_SN_GATEWAY_DB_SUBSCRIBE_FILE_ENDING));
  memcpy(&filename_with_extension, &h->_entry_client.file_number, strlen(h->_entry_client.file_number));
  memcpy(&filename_with_extension[strlen(h->_entry_client.file_number)], MQTT_SN_GATEWAY_DB_SUBSCRIBE_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_SUBSCRIBE_FILE_ENDING));
  create_file(filename_with_extension);

  // will topic and message file
  memset(&filename_with_extension,
         0,
         sizeof(h->_entry_client.file_number) + sizeof(MQTT_SN_GATEWAY_DB_WILL_FILE_ENDING));
  memcpy(&filename_with_extension, &h->_entry_client.file_number, strlen(h->_entry_client.file_number));
  memcpy(&filename_with_extension[strlen(h->_entry_client.file_number)],
         MQTT_SN_GATEWAY_DB_WILL_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_WILL_FILE_ENDING));
  create_file(filename_with_extension);

  // publish messages file
  memset(&filename_with_extension,
         0,
         sizeof(h->_entry_client.file_number) + sizeof(MQTT_SN_GATEWAY_DB_PUBLISH_FILE_ENDING));
  memcpy(&filename_with_extension, &h->_entry_client.file_number, strlen(h->_entry_client.file_number));
  memcpy(&filename_with_extension[strlen(h->_entry_client.file_number)], MQTT_SN_GATEWAY_DB_PUBLISH_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_PUBLISH_FILE_ENDING));
  create_file(filename_with_extension);
  */

  if (h->client_transaction_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_NOT_FOUND) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS;
  }

#if WITH_DB_DEBUG_LOGGING
  // TODO log success
#endif
  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_reset_client(db_handler *h,
                                               const char *client_id,
                                               device_address *client_address,
                                               device_address *forwarder_addresses,
                                               uint16_t forwarder_address_len,
                                               uint16_t connect_duration,
                                               uint64_t connect_time) {
  assert(h->client_transaction_started);
  if (h->database_error) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->client_transaction_result;
  }
  if (h->client_transaction_result != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
    return h->client_transaction_result;
  }

  if (strlen(client_id) < MQTT_SN_CLIENTID_MIN_LENGTH) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_SHORT;
    return h->client_transaction_result;
  }
  if (strlen(client_id) >= MQTT_SN_CLIENTID_MAX_LENGTH) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_ID_TOO_LONG;
    return h->client_transaction_result;
  }

  reset_db_entry_mqtt_sn_client(&h->_entry_client,
                                client_id,
                                client_address,
                                forwarder_addresses,
                                forwarder_address_len,
                                connect_duration,
                                connect_time);

#if WITH_DB_DEBUG_LOGGING
  // TODO log success
#endif
  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT delete_client(db_handler *h) {
  assert(h->client_transaction_started);

  if (h->database_error) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->client_transaction_result;
  }
  if (h->client_transaction_result != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
    return h->client_transaction_result;
  }

  char filename_with_extension[sizeof(h->_entry_client.file_number)
      + sizeof(MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING)];

  get_registration_file_name(h->_entry_client.file_number, filename_with_extension, sizeof(filename_with_extension));
  f_delete(filename_with_extension);

  get_subscribe_file_name(h->_entry_client.file_number, filename_with_extension, sizeof(filename_with_extension));
  f_delete(filename_with_extension);

  get_will_file_name(h->_entry_client.file_number, filename_with_extension, sizeof(filename_with_extension));
  f_delete(filename_with_extension);

  get_publish_file_name(h->_entry_client.file_number, filename_with_extension, sizeof(filename_with_extension));
  f_delete(filename_with_extension);

  if (delete_db_entry_client(&h->_client_registry_file, &h->_entry_client) < 0) {
#if WITH_DB_DEBUG_LOGGING
    // TODO log write db entry client error
#endif
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->database_error = true;
    return h->client_transaction_result;
  }

#if WITH_DB_DEBUG_LOGGING
  // TODO log success
#endif
  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT client_exist(db_handler *h) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_last_ping_req_received(db_handler *h, uint64_t *last_ping_req_received) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  (*last_ping_req_received) = h->_entry_client.last_ping_req_received;

  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_client_last_ping_req_received(db_handler *h, uint64_t last_ping_req_received) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  h->_entry_client.last_ping_req_received = last_ping_req_received;

  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_last_ping_resp_received(db_handler *h, uint64_t *last_ping_resp_awaited) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  (*last_ping_resp_awaited) = h->_entry_client.last_ping_resp_received;

  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_client_last_ping_resp_received(db_handler *h, uint64_t last_ping_resp_awaited) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  h->_entry_client.last_ping_resp_received = last_ping_resp_awaited;

  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_address(db_handler *h,
                                                     device_address *client_address,
                                                     device_address *forwarder_addresses,
                                                     uint16_t *forwarder_addresses_len,
                                                     uint16_t forwarder_addresses_max_len) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  (*client_address) = h->_entry_client.client_address;
  get_db_entry_mqtt_sn_client_forwarder_addresses(&h->_entry_client,
                                                  forwarder_addresses,
                                                  forwarder_addresses_len,
                                                  forwarder_addresses_max_len);

  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_id(db_handler *h, char *client_id) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  memcpy(client_id, h->_entry_client.client_id, strlen(h->_entry_client.client_id));

  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_connect_duration(db_handler *h, uint16_t *connect_duration) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  (*connect_duration) = h->_entry_client.connect_duration;

  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_status(db_handler *h, DB_ENTRY_MQTT_SN_CLIENT_STATUS *client_status) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  (*client_status) = h->_entry_client.client_status;

  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_client_status(db_handler *h, DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  h->_entry_client.client_status = client_status;

  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_ping_req_await_msg_type(db_handler *h, MQTT_SN_MESSAGE_TYPE *await_message_type) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  (*await_message_type) = h->_entry_client.ping_req_await_message_type;

  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_ping_req_await_msg_type(db_handler *h, MQTT_SN_MESSAGE_TYPE await_message_type) {
  assert(h->client_transaction_started);
  DB_HANDLER_CK__CLIENT_DB_ERROR(h)

  h->_entry_client.ping_req_await_message_type = await_message_type;

  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_count(db_handler *h, uint64_t *client_count) {
  if (h->database_error) {
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
#if WITH_DB_DEBUG_LOGGING
    log_database_error(h->logger, NULL);
#endif
    return h->client_transaction_result;
  }

  (*client_count) = h->client_count;

  return DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_start_client_transaction_by_client_count(db_handler *h, uint64_t client_count) {
  assert(h->client_count > client_count);
  assert(!h->client_transaction_started);

  h->transaction_result = DB_HANDLER_RESULT_SUCCESS;
  if (h->database_error) {
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
#if WITH_DB_DEBUG_LOGGING
    log_database_error(h->logger, NULL);
#endif
    return h->client_transaction_result;
  }
  h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS;
  h->client_transaction_started = true;

  if (read_db_entry_client(&h->_client_registry_file, client_count, &h->_entry_client) < 0) {
    // TODO read error
    DB_HANDLER_SET_CLIENT_TRANSACTION_ERROR(h)
  }
#if WITH_DB_DEBUG_LOGGING
  // TODO log
  log_start_client_transaction(h->logger,
                               h->transaction_result,
                               NULL,
                               h->client_transaction_result,
                               h->_entry_client.client_id,
                               h->_entry_client.file_number);
#endif
  return h->client_transaction_result;
}

