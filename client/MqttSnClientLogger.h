//
// Created by SomeDude on 27.06.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTLOGGER_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTLOGGER_H_

#include <stdint.h>
#include <logging/MqttSnLogging.h>
#include <platform/platform_compatibility.h>

#include <network/MqttSnGatewayNetworkInterface.h>
#include "MqttSnClientReturnCode.h"
#include "MqttSnClientAwaitMessage.h"
#include "MqttSnClientMsgHandlerReturnValue.h"

#ifdef __cplusplus
extern "C" {
#endif
int32_t log_mqtt_sn_client_status_asleep(const MqttSnLogger *logger);
int32_t log_mqtt_sn_client_deinitialization_success(const MqttSnLogger *logger);
int32_t log_mqtt_sn_client_deinitialization_failed(const MqttSnLogger *logger);

int32_t log_mqtt_sn_client_initialization_failed(const MqttSnLogger *logger);
int32_t log_mqtt_sn_client_initialization_success(const MqttSnLogger *logger);

int32_t log_mqtt_sn_client_reconnect_success(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);
int32_t log_mqtt_sn_client_reconnect_failed(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);
int32_t log_mqtt_sn_client_reconnect_congestion(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);

int32_t log_mqtt_sn_client_status_gateway_lost(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);
int32_t log_mqtt_sn_client_connection_maintain_connection(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);
int32_t log_mqtt_sn_client_connection_maintain_wakeup(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);

int32_t log_mqtt_sn_client_reconnected_to(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);
int32_t log_mqtt_sn_client_connected_to(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);
int32_t log_mqtt_sn_client_connect_failed(const MqttSnLogger *logger);

int32_t log_mqtt_sn_client_return_code(const MqttSnLogger *logger, MQTT_SN_CLIENT_RETURN_CODE mqtt_sn_client_return_code);
int32_t log_mqtt_sn_client_connection_status(const MqttSnLogger *logger, uint8_t connection_status, const device_address *gateway_address);
int32_t log_mqtt_sn_client_registrations_full(const MqttSnLogger *logger, uint16_t registrations_len, uint16_t max_len);
int32_t log_mqtt_sn_client_registrations_topic_name_too_long(const MqttSnLogger *logger, uint16_t topic_name_len, uint16_t max_len);
int32_t log_mqtt_sn_client_registrations_index_empty(const MqttSnLogger *logger, uint16_t registrations_len, uint16_t registration_index);

int32_t log_mqtt_sn_client_mqtt_sn_message_malformed(const MqttSnLogger *logger, const MqttSnMessageData *msg);

int32_t log_mqtt_sn_client_unhandled_message(const MqttSnLogger *logger, const MqttSnMessageData *msg);

int32_t print_fatal_mqtt_sn_client_error_in_file(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                                 int32_t line_number);
int32_t log_fatal_mqtt_sn_client_loop_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                            int32_t line_number);
int32_t log_fatal_mqtt_sn_client_send_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                            int32_t line_number, int32_t send_rc, int32_t to_send);

int32_t log_mqtt_sn_client_any_loop_message(const MqttSnLogger *logger, MQTT_SN_CLIENT_STATUS status, MQTT_SN_CLIENT_LOOP_RETURN_CODE rc,
                                            const MqttSnMessageData *msg);
int32_t log_mqtt_sn_client_not_await_message(const MqttSnLogger *logger, const MqttSnMessageData *msg);
int32_t log_mqtt_sn_client_not_await_message(const MqttSnLogger *logger, const MqttSnMessageData *msg);

int32_t log_fatal_mqtt_sn_client_await_fd_conversion_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                                           int32_t line_number, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status);
int32_t log_fatal_mqtt_sn_client_await_context_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                                     int32_t line_number);
int32_t log_mqtt_sn_client_await_fd_status(const MqttSnLogger *logger, MqttSnClient *client);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTLOGGER_H_
