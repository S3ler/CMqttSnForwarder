//
// Created by SomeDude on 19.06.2019.
//

#include "db_handler.h"
#include "ftfs_db.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <gateway/database/logging/db_logging.h>
#include <stdio.h>

static int32_t remove_file_if_exists(const char *path) {
  FRESULT filres;
  if ((filres = f_stat(path, NULL)) == FR_OK) {
    if (filres == FR_OK) {
      if (f_unlink(path) != FR_OK) {
        return -1;
      }
    }
    FIL f;
    if (f_open(&f, path, FA_CREATE_ALWAYS | FA_READ | FA_WRITE) != FR_OK) {
      return -1;
    }
    //int w;
    //f_write(&f, "", 0, &w));
    if (f_sync(&f) != FR_OK) {
      f_close(&f);
      return -1;
    }
    if (f_close(&f) != FR_OK) {
      return -1;
    }
  } else if (filres != FR_NO_FILE) {
    return -1;
  }
  return 0;
}

int32_t db_handler_initialize(db_handler *h, MqttSnLogger *logger) {
  memset(h, 0, sizeof(*h));

  h->client_count = 0;
  h->logger = logger;
  FRESULT mount_result;
  char fname[sizeof(MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME)
      + sizeof(MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME_ENDING)] = {0};
  strcat(fname, MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME);
  strcat(fname, MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME_ENDING);

  if ((mount_result = f_mount(&h->fs, "", 1)) != FR_OK) {
    if (mount_result != FR_NO_FILESYSTEM) {
#if WITH_LOGGING
      // TODO log error no file system found
#endif
      return -1;
    }
    char b[4096];
    int fno = open("abc.dat", O_CREAT | O_SYNC | O_RDWR, S_IRUSR | S_IWUSR);
    printf("fno = %d\n", fno);
    fflush(stdout);
    int i = 0;
    char buf[512] = {0};
    while (i < SECCOUNT) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
      write(fno, buf, 512);
#pragma GCC diagnostic pop

      i++;
    }
    close(fno);
    fno = -1;
    FRESULT mkfs_res;
    if ((mkfs_res = f_mkfs("", FM_SFD | FM_FAT, 0, b, 4096)) != FR_OK) {
#if WITH_LOGGING
      // TODO log error no file system found
#endif
      return -1;
    }
#ifdef MQTT_SN_GATEWAY_DB_LINUX_MOUNT_FAKE_SD
    if ((mount_result = f_mount(&h->fs, "", 1)) != FR_OK) {
#if WITH_LOGGING
      // TODO log error no file system found
#endif
#if WITH_LOGGING
      // TODO log coult not mount
      // TODO log error no file system found
#endif
      return -1;
    }
#endif
  }
  if (remove_file_if_exists(MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME) < 0) {
    return -1;
  }
  if (remove_file_if_exists(MQTT_SN_GATEWAY_DB_MQTT_SUBSCRIPTION_FILE_NAME) < 0) {
    return -1;
  }
  if (remove_file_if_exists(MQTT_SN_GATEWAY_DB_DEFAULT_PREDEFINED_TOPIC_FILE_NAME) < 0) {
    return -1;
  }
  if (remove_file_if_exists(MQTT_SN_GATEWAY_DB_DEFAULT_MQTT_CONFIGURATION_FILE_NAME) < 0) {
    return -1;
  }
  if (remove_file_if_exists(MQTT_SN_GATEWAY_DB_DEFAULT_MQTT_SN_CONFIGURATION_FILE_NAME) < 0) {
    return -1;
  }

  if (f_open(&h->_client_registry_file,
             MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME,
             FA_CREATE_ALWAYS | FA_READ | FA_WRITE) != FR_OK) {
    return -1;
  }
  if (f_close(&h->_client_registry_file) != FR_OK) {
    return -1;
  }

  if (f_open(&h->_gateway_registry_file,
             MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME,
             FA_CREATE_ALWAYS | FA_READ | FA_WRITE) != FR_OK) {
    return -1;
  }
  if (f_close(&h->_gateway_registry_file) != FR_OK) {
    return -1;
  }
  // init discovered gateways file with empty
  for (uint8_t pos = 0; pos < UINT8_MAX; pos++) {
    DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED entry_gw = {0};
    entry_gw.gw_id = pos;
    entry_gw.status = DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_NEVER;
    if (f_write_at(MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME, pos, &entry_gw, sizeof(entry_gw)) < 0) {
      return DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR;
    }
  }

  return 0;
}
DB_HANDLER_RESULT get_db_handler_result(db_handler *h) {
  return h->transaction_result;
}



