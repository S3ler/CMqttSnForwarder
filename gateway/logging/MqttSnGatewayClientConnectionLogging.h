//
// Created by SomeDude on 15.07.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYCLIENTCONNECTIONLOGGING_H_
#define CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYCLIENTCONNECTIONLOGGING_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <gateway/database/db_entry_mqtt_sn_client.h>
#include <gateway/database/db_handler_result.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_fatal_client_db_error(const MqttSnLogger *logger,
                                  const char *file_name,
                                  const char *function_name,
                                  int32_t line_number,
                                  DB_ENTRY_MQTT_SN_CLIENT_RESULT db_entry_result,
                                  DB_HANDLER_RESULT db_handler_result);
int32_t log_fatal_client_connection_check_error(const MqttSnLogger *logger,
                                                const char *file_name,
                                                const char *function_name,
                                                int32_t line_number);
int32_t log_client_connection_client_lost(const MqttSnLogger *logger,
                                          const device_address *wireless_node_id,
                                          const char *client_id,
                                          uint16_t connect_duration);
int32_t log_client_connection_client_connected(const MqttSnLogger *logger,
                                               const device_address *wireless_node_id,
                                               const char *client_id,
                                               uint16_t connect_duration,
                                               uint8_t clean_session);
int32_t log_client_connection_checking_client_timeout(const MqttSnLogger *logger, uint64_t client_count);
int32_t log_client_connection_client_send_ping_req(const MqttSnLogger *logger,
                                                   const device_address *wireless_node_id,
                                                   const char *client_id,
                                                   uint16_t connect_duration,
                                                   uint64_t ping_req_received,
                                                   uint64_t ping_resp_received);
int32_t log_client_connection_ping_resp_received(const MqttSnLogger *logger, const device_address *wireless_node_id);
int32_t log_client_connection_client_ping_resp_received(const MqttSnLogger *logger,
                                                        const device_address *wireless_node_id,
                                                        const char *client_id,
                                                        uint16_t connect_duration,
                                                        uint64_t ping_req_received,
                                                        uint64_t ping_resp_received);
int32_t log_client_connection_ping_req_received(const MqttSnLogger *logger, const device_address *wireless_node_id);
int32_t log_client_connection_client_ping_req_received(const MqttSnLogger *logger,
                                                       const device_address *wireless_node_id,
                                                       const char *client_id,
                                                       uint16_t connect_duration,
                                                       uint64_t ping_req_received,
                                                       uint64_t ping_resp_received);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYCLIENTCONNECTIONLOGGING_H_
