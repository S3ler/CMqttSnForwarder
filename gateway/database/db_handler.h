//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_HANDLER_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_HANDLER_H_

#include <stdint.h>
#include <stdbool.h>
#include <external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.h>
#include <logging/MqttSnLoggingInterface.h>
#include "db_entry_mqtt_sn_client.h"
#include "db_entry_mqtt_sn_gateway_discovered.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_GATEWAY_DB_MAXIMUM_CLIENTS
#define MQTT_SN_GATEWAY_DB_MAXIMUM_CLIENTS -1
#endif

#ifndef MQTT_SN_GATEWAY_DB_MAXIMUM_PUBLISH_COUNT
#define MQTT_SN_GATEWAY_DB_MAXIMUM_PUBLISH_COUNT -1
#endif

#define MQTT_SN_GATEWAY_DB_SUBSCRIBE_FILE_ENDING      ".SUB"
#define MQTT_SN_GATEWAY_DB_REGISTRATION_FILE_ENDING   ".REG"
#define MQTT_SN_GATEWAY_DB_WILL_FILE_ENDING           ".WIL"
#define MQTT_SN_GATEWAY_DB_PUBLISH_FILE_ENDING        ".PUB"

#ifndef MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME
#define MQTT_SN_GATEWAY_DB_CLIENT_REGISTRY_FILE_NAME "CLIENTS"
#endif

#ifndef MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME
#define MQTT_SN_GATEWAY_DB_DISCOVERED_GATEWAYS_FILE_NAME "DGW"
#endif

#ifndef MQTT_SN_GATEWAY_DB_MQTT_SUBSCRIPTION_FILE_NAME
#define MQTT_SN_GATEWAY_DB_MQTT_SUBSCRIPTION_FILE_NAME "MQTT.SUB"
#endif

#ifndef MQTT_SN_GATEWAY_DB_DEFAULT_PREDEFINED_TOPIC_FILE_NAME
#define MQTT_SN_GATEWAY_DB_DEFAULT_PREDEFINED_TOPIC_FILE_NAME "TOPICS.PRE"
#endif

#ifndef MQTT_SN_GATEWAY_DB_DEFAULT_MQTT_CONFIGURATION_FILE_NAME
#define MQTT_SN_GATEWAY_DB_DEFAULT_MQTT_CONFIGURATION_FILE_NAME "MQTT.CON"
#endif

#ifndef MQTT_SN_GATEWAY_DB_DEFAULT_MQTT_SN_CONFIGURATION_FILE_NAME
#define MQTT_SN_GATEWAY_DB_DEFAULT_MQTT_SN_CONFIGURATION_FILE_NAME "MQTTSN.CON"
#endif

// TODO remove later and adept mount
#define MQTT_SN_GATEWAY_DB_LINUX_MOUNT_FAKE_SD
#ifndef MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME
#define MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME "db"
#endif
#ifndef MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME_ENDING
#define MQTT_SN_GATEWAY_DB_DEFAULT_DATABASE_FILE_NAME_ENDING ".dat"
#endif

#ifndef SECCOUNT
#define SECCOUNT 128
#endif

#define DB_HANDLER_CK_CLIENT_RC(cmd) if((cmd) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS){return -1;}

#define DB_HANDLER_CK__CLIENT_DB_ERROR(h)   if (h->database_error) { \
                                              h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR; \
                                              h->transaction_result = DB_HANDLER_RESULT_ERROR; \
                                              return h->client_transaction_result; \
                                            } \

#define DB_HANDLER_SET_CLIENT_TRANSACTION_ERROR(h)  h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR; \
                                                    h->transaction_result = DB_HANDLER_RESULT_ERROR; \
                                                    h->database_error = true; \
                                                    log_database_error(h->logger, NULL); \
                                                    return h->client_transaction_result; \

#if WITH_LOGGING
#define DB_HANDLER_SET_CLIENT_TRANSACTION_ERROR(h)  h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR; \
                                                    h->transaction_result = DB_HANDLER_RESULT_ERROR; \
                                                    h->database_error = true; \
                                                    log_database_error(h->logger, NULL); \
                                                    return h->client_transaction_result; \

#else
#define DB_HANDLER_SET_CLIENT_TRANSACTION_ERROR(h)  h->client_transaction_result = DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR; \
                                                    h->transaction_result = DB_HANDLER_RESULT_ERROR; \
                                                    h->database_error = true; \
                                                    return h->client_transaction_result; \

#endif

typedef enum DB_HANDLER_RESULT_ {
  DB_HANDLER_RESULT_SUCCESS = 0,
  DB_HANDLER_RESULT_NO_TRANSACTION_STARTED,
  DB_HANDLER_RESULT_ERROR
} DB_HANDLER_RESULT;

typedef struct db_handler_ {
  //char *db_directory_path;
  //bool _not_in_client_registry;
  //bool _error;
  //  uint32_t _panic;
  FATFS fs;
  FIL f;

  bool database_error;
  DB_HANDLER_RESULT transaction_result;

  bool client_transaction_started;
  DB_ENTRY_MQTT_SN_CLIENT_RESULT client_transaction_result;

  FIL _client_registry_file;
  DB_ENTRY_MQTT_SN_CLIENT _entry_client;

  FIL _gateway_registry_file;
  DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT gateway_discovered_result;
  DB_HANDLER_RESULT gateway_discovered_transaction_result;

  uint64_t client_count;

#if WITH_LOGGING
  MqttSnLogger *logger;
#endif
} db_handler;

// initialization
int32_t db_handler_initialize(db_handler *h, MqttSnLogger *logger);
DB_HANDLER_RESULT get_db_handler_result(db_handler *h);

// TODO



// client
// transaction

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_start_client_transaction(db_handler *h,
                                                           const device_address *address,
                                                           const char *client_id);

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_end_client_transaction(db_handler *h);

bool db_is_client_transaction_started(db_handler *h);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_transaction_result(db_handler *h);

// crud
DB_ENTRY_MQTT_SN_CLIENT_RESULT add_client(db_handler *h,
                                          const char *client_id,
                                          device_address *client_address,
                                          device_address *forwarder_addresses,
                                          uint16_t forwarder_address_len,
                                          uint16_t connect_duration,
                                          uint64_t connect_time);

DB_ENTRY_MQTT_SN_CLIENT_RESULT client_exist(db_handler *h);

DB_ENTRY_MQTT_SN_CLIENT_RESULT delete_client(db_handler *h);

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_reset_client(db_handler *h,
                                               const char *client_id,
                                               device_address *client_address,
                                               device_address *forwarder_addresses,
                                               uint16_t forwarder_address_len,
                                               uint16_t connect_duration,
                                               uint64_t connect_time);

// DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_count(db_handler *h, uint64_t *client_count);
// DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_address(db_handler *h, device_address *address, uint64_t client_count);

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_count(db_handler *h, uint64_t *client_count);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_start_client_transaction_by_client_count(db_handler *h, uint64_t client_count);

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_last_ping_req_received(db_handler *h, uint64_t *last_ping_req_received);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_client_last_ping_req_received(db_handler *h, uint64_t last_ping_req_received);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_last_ping_resp_received(db_handler *h, uint64_t *last_ping_resp_awaited);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_client_last_ping_resp_received(db_handler *h, uint64_t last_ping_resp_awaited);

DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_address(db_handler *h, device_address *client_address);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_connect_duration(db_handler *h, uint16_t *connect_duration);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_client_status(db_handler *h, DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_client_status(db_handler *h, DB_ENTRY_MQTT_SN_CLIENT_STATUS *client_status);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_get_ping_req_await_msg_type(db_handler *h, MQTT_SN_MESSAGE_TYPE *await_message_type);
DB_ENTRY_MQTT_SN_CLIENT_RESULT db_set_ping_req_await_msg_type(db_handler *h, MQTT_SN_MESSAGE_TYPE await_message_type);

void set_client_await_message(MQTT_SN_MESSAGE_TYPE msg_type);

MQTT_SN_MESSAGE_TYPE get_client_await_MQTT_SN_MESSAGE_TYPE();

void set_timeout(uint32_t timeout);

bool has_client_will();

void get_client_will(char *target_willtopic,
                     uint8_t *target_willmsg,
                     uint8_t *target_willmsg_length,
                     uint8_t *target_qos,
                     bool *target_retain);

void set_client_willtopic(char *willtopic, uint8_t qos, bool retain);

void set_client_willmessage(uint8_t *willmsg, uint8_t willmsg_length);

void delete_will();

void get_last_client_address(device_address *address);

void get_nth_client(uint64_t n,
                    char *client_id,
                    device_address
                    *target_address,
                    DB_ENTRY_MQTT_SN_CLIENT_STATUS *target_status,
                    uint32_t
                    *target_duration,
                    uint32_t *target_timeout);

void set_client_state(DB_ENTRY_MQTT_SN_CLIENT_STATUS status);

void set_client_duration(uint32_t duration);

uint16_t get_client_await_msg_id();

void set_client_await_msg_id(uint16_t msg_id);

bool has_client_publishes();

// topic
const char *get_topic_name(uint16_t topic_id);

uint16_t get_topic_id(char *topic_name);

bool is_topic_known(uint16_t topic_id);

bool set_topic_known(uint16_t topic_id, bool known);

void add_client_registration(char *topic_name, uint16_t *topic_id);

char *get_predefined_topic_name(uint16_t topic_id);

// subscription
bool is_subscribed(const char *topic_name);

void add_subscription(const char *topic_name, uint16_t topic_id, uint8_t qos);

void delete_subscription(uint16_t topic_id);

int8_t get_subscription_qos(const char *topic_name);

uint16_t get_subscription_topic_id(const char *topic_name);

uint16_t get_nth_subscribed_topic_id(uint16_t n);

uint16_t get_client_subscription_count();

bool decrement_global_subscription_count(const char *topic_name);

bool increment_global_subscription_count(const char *topic_name);

uint32_t get_global_topic_subscription_count(const char *topic_name);

// publish
void add_new_client_publish(uint8_t *data, uint8_t data_len, uint16_t topic_id, bool retain, uint8_t qos);
/*
void add_new_client_publish(uint8_t *data, uint8_t data_len, uint16_t topic_id, bool retain,
                       uint8_t qos) {
  this->add_client_publish(data, data_len, topic_id, retain, qos, false, 0);
}
*/

void add_client_publish(uint8_t *data, uint8_t data_len, uint16_t topic_id, bool retain,
                        uint8_t qos, bool dup, uint16_t msg_id);

void get_next_publish(uint8_t *data, uint8_t *data_len, uint16_t *topic_id, bool *retain,
                      uint8_t *qos,
                      bool *dup, uint16_t *publish_id);

void set_publish_msg_id(uint16_t publish_id, uint16_t msg_id);

void remove_publish_by_msg_id(uint16_t msg_id);

void remove_publish_by_publish_id(uint16_t publish_id);

// gateway configuration

uint16_t get_timeout_check_duration();

uint16_t get_advertise_duration();

bool get_gateway_id(uint8_t *gateway_id);

bool get_mqtt_config(uint8_t *server_ip, uint16_t *server_port, char *client_id);

bool get_mqtt_login_config(char *username, char *password);

bool get_mqtt_will(char *will_topic, char *will_msg, uint8_t *will_qos, bool *will_retain);


// gateway connection status

uint8_t set_mqttsn_disconnected();

uint8_t set_mqtt_disconnected();

uint8_t set_mqtt_connected();

uint8_t set_mqttsn_connected();

bool is_mqttsn_online();

bool is_mqtt_online();

// logging use only
void get_client_id(char *client_id);

// monitoring of gateway
DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT mqtt_sn_discovered_gateway_update_address(db_handler *h,
                                                                                     uint8_t gw_id,
                                                                                     const device_address *address,
                                                                                     uint64_t received_time);
DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT mqtt_sn_discovered_gateway_update(db_handler *h,
                                                                             uint8_t gw_id,
                                                                             uint16_t duration,
                                                                             const device_address *address,
                                                                             uint64_t received_time);
DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT get_gateway_discovered_result(db_handler *h);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_HANDLER_H_
