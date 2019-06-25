//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_H_

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnMessageParser.h>

#ifdef __cplusplus
extern "C" {
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

typedef struct DB_ENTRY_MQTT_SN_CLIENT_ {
  char client_id[24];
  char file_number[9]; // TODO better name like key
  device_address client_address;
  DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status;
  uint16_t duration; // changed
  uint32_t timeout;
  uint64_t last_message_time;
  uint16_t await_message_id;
  MQTT_SN_MESSAGE_TYPE await_message;
  int32_t position;
} DB_ENTRY_MQTT_SN_CLIENT;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_H_
