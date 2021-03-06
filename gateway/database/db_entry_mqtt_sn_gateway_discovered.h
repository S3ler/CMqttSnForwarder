//
// Created by SomeDude on 20.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_H_
#include <stdint.h>
#include <platform/device_address.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_MAX_FORWARDER_COUNT
#define DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_MAX_FORWARDER_COUNT 5
#endif

typedef enum DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ {
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_SUCCESS = 0,
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_IN_PROGRESS,
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT_ERROR
} DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT;

typedef enum DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_STATUS_ {
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_NEVER = 0,
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_INACTIVE = 1,
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_ACTIVE = 2
} DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_STATUS;

typedef struct DB_ENTY_MQTT_SN_GATEWAY_DISCOVERED_LIST_ENTRY_ {
  uint8_t gw_id;  // gateway id
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_STATUS status;
} DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_LIST_ENTRY;

typedef struct DB_ENTY_MQTT_SN_GATEWAY_DISCOVERED_LIST_ {
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_LIST_ENTRY gw[UINT8_MAX];
} DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_LIST;

typedef struct DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_ {
  uint8_t gw_id;  // gateway id
  uint16_t duration; // duration like in the mqtt sn message
  uint16_t duration_timeout; // count down timeout
  uint64_t last_message_time; // when was last message received
  uint8_t signal_strength; // signal strength of message received from the gateway
  device_address network_address; // network address of gateway
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_STATUS status;

  device_address forwarder_addresses[DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_MAX_FORWARDER_COUNT];
  uint16_t forwarder_address_len;
} DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_H_
