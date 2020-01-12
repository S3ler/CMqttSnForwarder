//
// Created by SomeDude on 02.08.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTTSNCMDBUFFER_H_
#define CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTTSNCMDBUFFER_H_

#include <stdint.h>
#include <client/interactive/config/interactive_client_config.h>
#include <client/MqttSnClientMethod.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include "mqtt_sn_client_interactive_action_type.h"
#include "mqtt_sn_client_interactive_action_result_type.h"
#include "mqtt_sn_client_interactive_cmd_result.h"
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
#include <network/linux/gateway/ip/tcp/MqttSnLinuxGatewayTcpNetworkContainer.h>
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#include <network/linux/gateway/ip/udp/MqttSnLinuxGatewayUdpNetworkContainer.h>
#endif
#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
#include <network/arduino/gateway/ip/udp/MqttSnArduinoGatewayUdpNetworkContainer.hpp>
#endif
#ifdef __cplusplus
extern "C" {
#endif

int32_t interactive_client_publish_callback(const struct MqttSnClient_ *client, const struct MqttSnPublish_ *publish);

typedef struct mqtt_sn_interactive_client_ {
    uint64_t       action_number;
    MqttSnClient   client;
    device_address mqtt_sn_gateway_address;

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    MqttSnLinuxGatewayTcpNetworkContainer tcpGatewayNetworkContainer;
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    MqttSnLinuxGatewayUdpNetworkContainer udpGatewayNetworkContainer;
#endif
    FileLoggingContext       file_log_context;
    FileStdoutLoggingContext fileconsole_log_context;

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
    MqttSnArduinoGatewayUdpNetworkContainer udpGatewayNetworkContainer;
#endif
} mqtt_sn_interactive_client;

int32_t mqtt_sn_client_interactive_start_logger(mqtt_sn_interactive_client *client, const mqtt_sn_logger_config *cfg, MqttSnLogger *logger);

void mqtt_sn_interactive_client_init(mqtt_sn_interactive_client *interactive_client);

void mqtt_sn_client_interactive_print_usage(const MqttSnLogger *logger);
void mqtt_sn_client_interactive_print_usage_short(const MqttSnLogger *logger);
void mqtt_sn_client_interactive_print_usage_long(const MqttSnLogger *logger);

uint8_t mqtt_sn_client_interactive_is_exit(const char *str);
uint8_t mqtt_sn_client_interactive_is_help(const char *str);

MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE mqtt_sn_client_interactive_parse_action(const char *buffer, int32_t buffer_length);

mqtt_sn_client_interactive_cmd_result mqtt_sn_cmd_buffer_execute_command(mqtt_sn_interactive_client *            client,
                                                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE  tk,
                                                                         const interactive_client_config_container *cfg, MqttSnLogger *logger);

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveReinitLogger(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg, MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveInit(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg, const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveDeinit(mqtt_sn_interactive_client *client);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveAutoConnect(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg,
                                   const MQTT_SN_CLIENT_FIND_GW_PATTERN find_gateway_pattern, const int32_t find_gateway_timeout_ms,
                                   const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSearchGwAndConnect(mqtt_sn_interactive_client *            client,
                                                                                        const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDirectConnect(mqtt_sn_interactive_client *               client,
                                                                                   const interactive_client_config_container *cfg,
                                                                                   const MqttSnLogger *                       logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveAdvertiseAndDirectConnect(mqtt_sn_interactive_client *client,
                                                                                               const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDisconnect(mqtt_sn_interactive_client *client);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSubscribe(mqtt_sn_interactive_client *            client,
                                                                               const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveUnsubscribe(mqtt_sn_interactive_client *            client,
                                                                                 const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveRegister(mqtt_sn_interactive_client *            client,
                                                                              const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractivePublish(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractivePublishReceivePublish(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveSleep(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveWakeup(mqtt_sn_interactive_client *client);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveReconnect(mqtt_sn_interactive_client *client);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractivePing(mqtt_sn_interactive_client *client);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE
get_single_publish_mqtt_sn_client_interactive_action_type(const single_client_publish_config *publish, const MqttSnClient *client);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE
get_single_subscribe_mqtt_sn_client_interactive_action_type(const client_subscription_topic *subscription);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE
get_single_publish_receive_publish_mqtt_sn_client_interactive_action_type(const single_client_publish_config *publish);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTTSNCMDBUFFER_H_
