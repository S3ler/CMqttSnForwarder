//
// Created by SomeDude on 21.06.2019.
//
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <gateway/database/logging/db_logging.h>
#include "db_handler.h"
#include "ftfs_db.h"

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_start_client_transaction(db_handler *h,
                                                           const device_address *address,
                                                           const char *client_id) {
  assert(address != NULL || client_id != NULL);
  assert(!h->client_transaction_started);

  h->transaction_result = DB_HANDLER_RESULT_SUCCESS;
  if (h->database_error) {
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
#if WITH_DEBUG_LOGGING
    log_database_error(h->logger, NULL);
#endif
    return h->client_transaction_result;
  }

  h->client_transaction_started = true;

  uint32_t line_number = 0;
  UINT readChars = 0;
  do {
    memset(&h->_entry_client, 0, sizeof(DB_ENTRY_MQTT_SN_CLIENT));
    uint16_t buffer_size = sizeof(DB_ENTRY_MQTT_SN_CLIENT);

    if (f_read(&h->_client_registry_file, &h->_entry_client, buffer_size, &readChars) != FR_OK) {
      h->transaction_result = DB_HANDLER_RESULT_ERROR;
      h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
#if WITH_DEBUG_LOGGING
      log_database_error(h->logger, NULL);
#endif
      return h->client_transaction_result;
    }

    if (address) {
      if (readChars == buffer_size &&
          (strlen(h->_entry_client.client_id) < MQTT_SN_CLIENTID_MAX_LENGTH) &&
          (memcmp(&h->_entry_client.client_address, address, sizeof(device_address)) == 0)) {
        h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS;
      }
    } else {
      if (readChars == buffer_size &&
          (strlen(h->_entry_client.client_id) < MQTT_SN_CLIENTID_MAX_LENGTH) &&
          (strcmp(h->_entry_client.client_id, client_id) == 0)) {
        h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS;
      }
    }
    if (h->client_transaction_result == DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#if WITH_DEBUG_LOGGING
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
#if WITH_DEBUG_LOGGING
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

#if WITH_DEBUG_LOGGING
  log_apply_client_transaction(h->logger, h->transaction_result, h->client_transaction_result, h->database_error);
#endif
  // TODO close all files
  // TODO check what happens if we close file twice
  return h->client_transaction_result;
}

DB_ENTRY_MQTT_SN_CLIENT_RESULT add_client(db_handler *h,
                                          const char *client_id,
                                          device_address *address,
                                          uint16_t duration,
                                          uint64_t current_time) {
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

  int32_t empty_space = find_empty_entry_space_in_registry(&h->_entry_client,
                                                           &h->_client_registry_file,
                                                           MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME);
  if (empty_space < 0) {
#if WITH_DEBUG_LOGGING
    // TODO log error
#endif
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->database_error = true;
    return h->client_transaction_result;
  }
  memset(&h->_entry_client, 0x0, sizeof(DB_ENTRY_MQTT_SN_CLIENT));
  strcpy(h->_entry_client.client_id, client_id);
  sprintf(h->_entry_client.file_number, "%08d", empty_space);
  memcpy(&h->_entry_client.client_address, address, sizeof(device_address));
  h->_entry_client.duration = duration;
  h->_entry_client.timeout = 0; // TODO set timeout properly
  h->_entry_client.last_message_time = current_time;
  h->_entry_client.client_status = DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE;
  h->_entry_client.await_message_id = 0;
  h->_entry_client.await_message = PINGREQ;
  h->_entry_client.position = empty_space;


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

  if (write_db_entry_client(&h->_client_registry_file, &h->_entry_client) < 0) {
#if WITH_DEBUG_LOGGING
    // TODO log write db entry client error
#endif
    return DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
  }

#if WITH_DEBUG_LOGGING
  // TODO log success
#endif
  return DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_reset_client(db_handler *h,
                                               const char *client_id,
                                               device_address *address,
                                               uint16_t duration,
                                               uint64_t current_time) {
  assert(h->client_transaction_started);
  if (h->database_error) {
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

  h->_entry_client.client_address = (*address);
  h->_entry_client.duration = duration;
  h->_entry_client.timeout = 0;// TODO set timeout properly
  h->_entry_client.last_message_time = current_time;
  h->_entry_client.client_status = DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE;
  h->_entry_client.await_message_id = 0;
  h->_entry_client.await_message = PINGREQ;

  if (write_db_entry_client(&h->_client_registry_file, &h->_entry_client) < 0) {
#if WITH_DEBUG_LOGGING
    // TODO log write db entry client error
#endif
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->database_error = true;
    return h->client_transaction_result;
  }

#if WITH_DEBUG_LOGGING
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
#if WITH_DEBUG_LOGGING
    // TODO log write db entry client error
#endif
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    h->database_error = true;
    return h->client_transaction_result;
  }

#if WITH_DEBUG_LOGGING
  // TODO log success
#endif
  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT client_exist(db_handler *h) {
  assert(h->client_transaction_started);

  if (h->database_error) {
    h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR;
    h->transaction_result = DB_HANDLER_RESULT_ERROR;
    return h->client_transaction_result;
  }
  return h->client_transaction_result;
}
DB_ENTRY_MQTT_SN_CLIENT_RESULT get_client_result(db_handler *h) {
  return h->client_transaction_result;
}
