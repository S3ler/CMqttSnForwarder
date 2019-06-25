//
// Created by SomeDude on 21.06.2019.
//

#include "ftfs_db.h"
#include "db_entry_mqtt_sn_client.h"
#include "db_handler.h"
#include <string.h>
#include <external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.h>
#include <assert.h>

int32_t f_read_at(const char *path, UINT pos, void *buf, UINT btr) {
  FIL f;
  UINT read_bytes = 0;
  int32_t result = 0;
  if (f_open(&f, path, FA_READ) < 0) {
#if WITH_DEBUG_LOGGING
    // TODO log reopen error
#endif
    return -1;
  }
  if (f_lseek(&f, pos * btr) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log seek error
#endif
    return -1;
  }
  if (f_read(&f, buf, btr, &read_bytes) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log read error
#endif
  }
  if (read_bytes != btr) {
#if WITH_DEBUG_LOGGING
    // TODO log read bytes miss match error
#endif
    return -1;
  }
  if (f_close(&f) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log close error
#endif
    return -1;
  }
  return result;
}
int32_t f_write_at(const char *path, UINT pos, void *buf, UINT btr) {
  FIL f;
  UINT written_bytes = 0;
  int32_t result = 0;
  if (f_open(&f, path, FA_READ) < 0) {
#if WITH_DEBUG_LOGGING
    // TODO log reopen error
#endif
    return -1;
  }
  if (f_lseek(&f, pos * btr) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log seek error
#endif
    return -1;
  }
  if (f_write(&f, buf, btr, &written_bytes) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log read error
#endif
  }
  if (written_bytes != btr) {
#if WITH_DEBUG_LOGGING
    // TODO log read bytes miss match error
#endif
    return -1;
  }
  if (f_close(&f) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log close error
#endif
    return -1;
  }
  return result;
}
int32_t write_db_entry_client(FIL *f, DB_ENTRY_MQTT_SN_CLIENT *client_entry) {
  assert(client_entry->position >= 0);

  if (f_go_to_start(f, MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME, FA_READ | FA_WRITE) < 0) {
#if WITH_DEBUG_LOGGING
    // TODO log reopen error
#endif
    return -1;
  }
  if (f_lseek(f, client_entry->position * sizeof(DB_ENTRY_MQTT_SN_CLIENT_STATUS)) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log seek error
#endif
    return -1;
  }

  UINT written_bytes = 0;
  if (f_write(f, client_entry, sizeof(DB_ENTRY_MQTT_SN_CLIENT), &written_bytes) != FR_OK) {
#if WITH_DEBUG_LOGGING
    // TODO log write error
#endif
    return -1;
  }

  if (written_bytes != sizeof(DB_ENTRY_MQTT_SN_CLIENT)) {
#if WITH_DEBUG_LOGGING
    // TODO log written bytes miss match
#endif
    return -1;
  }
  return 0;
}

int32_t delete_db_entry_client(FIL *f, DB_ENTRY_MQTT_SN_CLIENT *client_entry) {
  memset(client_entry, 0, sizeof(DB_ENTRY_MQTT_SN_CLIENT));
  return write_db_entry_client(f, client_entry);
}

int32_t f_delete(const char *path) {
  if (f_unlink(path) != FR_OK) {
    return -1;
  }
  return 0;
}

int32_t f_go_to_start(FIL *f, const char *path, BYTE mode) {
  if (f_sync(f) != FR_OK) {
    return -1;
  }
  if (f_close(f) != FR_OK) {
    return -1;
  }
  if (f_open(f, path, mode) != FR_OK) {
    return -1;
  }
  return 0;
}
int32_t find_empty_entry_space_in_registry(DB_ENTRY_MQTT_SN_CLIENT *entry_client, FIL *fil, const char *file_name) {
  // TODO if full?
  if (f_go_to_start(fil, file_name, FA_READ) < 0) {
#if WITH_DEBUG_LOGGING
    // TODO log error
#endif
    return -1;
  }
  // if the file does not exist, it will be created
  int32_t line_number = 0;
  UINT readChars = 0;
  do {
    if (f_eof(fil)) {
      break;
    }
    memset(entry_client, 0, sizeof(DB_ENTRY_MQTT_SN_CLIENT));
    size_t buffer_size = sizeof(DB_ENTRY_MQTT_SN_CLIENT);
    if (f_read(fil, entry_client, buffer_size, &readChars) != FR_OK) {
      line_number = -1;
      break;
    }
    // assert(readChars == buffer_size);
    if (readChars != buffer_size) {
      line_number = -1;
      break;
    }
    if (readChars == buffer_size && (strlen(entry_client->client_id) == 0)) {
      break;
    }
    line_number++;
  } while (readChars > 0);
#if WITH_DEBUG_LOGGING
  if (line_number < 0) {
    // TODO log error
  } else {
    // TODO log success
  }
#endif
  if (f_close(fil) != FR_OK) {
    line_number = -1;
  }
  return line_number;
}
int32_t get_registration_file_name(const char *file_number, char *dst, uint16_t dst_len) {
  assert(strlen(file_number) + sizeof(MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING) > dst_len);
  memset(dst, 0, dst_len);
  memcpy(dst, file_number, strlen(file_number));
  memcpy(&dst[strlen(file_number)], MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING));
  return 0;
}
int32_t get_subscribe_file_name(const char *file_number, char *dst, uint16_t dst_len) {
  assert(strlen(file_number) + sizeof(MQTT_SN_GATEWAY_DB_SUBSCRIBE_FILE_ENDING) > dst_len);
  memset(dst, 0, dst_len);
  memcpy(dst, file_number, strlen(file_number));
  memcpy(&dst[strlen(file_number)], MQTT_SN_GATEWAY_DB_SUBSCRIBE_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_SUBSCRIBE_FILE_ENDING));
  return 0;
}
int32_t get_will_file_name(const char *file_number, char *dst, uint16_t dst_len) {
  assert(strlen(file_number) + sizeof(MQTT_SN_GATEWAY_DB_WILL_FILE_ENDING) > dst_len);
  memset(dst, 0, dst_len);
  memcpy(dst, file_number, strlen(file_number));
  memcpy(&dst[strlen(file_number)], MQTT_SN_GATEWAY_DB_WILL_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_WILL_FILE_ENDING));
  return 0;
}
int32_t get_publish_file_name(const char *file_number, char *dst, uint16_t dst_len) {
  assert(strlen(file_number) + sizeof(MQTT_SN_GATEWAY_DB_PUBLISH_FILE_ENDING) > dst_len);
  memset(dst, 0, dst_len);
  memcpy(dst, file_number, strlen(file_number));
  memcpy(&dst[strlen(file_number)], MQTT_SN_GATEWAY_DB_PUBLISH_FILE_ENDING,
         strlen(MQTT_SN_GATEWAY_DB_PUBLISH_FILE_ENDING));
  return 0;
}
