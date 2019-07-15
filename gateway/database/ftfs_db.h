//
// Created by SomeDude on 21.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_LOGGING_FTFS_DB_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_LOGGING_FTFS_DB_H_

#include <stdint.h>
#include <external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/integer.h>
#include <external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.h>
#include "db_entry_mqtt_sn_client.h"
#include "db_handler.h"
#ifdef __cplusplus
extern "C" {
#endif

int32_t f_read_at(const char *path, UINT pos, UINT pos_offset, void *buf, UINT btr);
int32_t f_write_at(const char *path, UINT pos, UINT pos_offset, void *buf, UINT btr);
int32_t f_delete(const char *path);

int32_t read_db_entry_client(FIL *f, uint64_t position, DB_ENTRY_MQTT_SN_CLIENT *client_entry);
int32_t write_db_entry_client(FIL *f, DB_ENTRY_MQTT_SN_CLIENT *client_entry);
int32_t delete_db_entry_client(FIL *f, DB_ENTRY_MQTT_SN_CLIENT *client_entry);

int32_t f_go_to_start(FIL *f, const char *path, BYTE mode);

int32_t find_empty_entry_space_in_registry(DB_ENTRY_MQTT_SN_CLIENT *entry_client, FIL *fil, const char *file_name);
int32_t get_registration_file_name(const char *file_number, char *dst, uint16_t dst_len);
int32_t get_subscribe_file_name(const char *file_number, char *dst, uint16_t dst_len);
int32_t get_will_file_name(const char *file_number, char *dst, uint16_t dst_len);
int32_t get_publish_file_name(const char *file_number, char *dst, uint16_t dst_len);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_LOGGING_FTFS_DB_H_
