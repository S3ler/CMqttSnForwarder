//
// Created by SomeDude on 25.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENT_H
#define CMQTTSNFORWARDER_MQTTSNCLIENT_H

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnMessageParser.h>
#include <parser/MqttSnPublishMessage.h>
#include <network/MqttSnGatewayNetworkInterface.h>
#include "MqttSnClientConnectionStatus.h"
#include "MqttSnClientConfig.h"
#include "MqttSnClientRegistration.h"
#include "MqttSnClientAwaitMessage.h"
#include "MqttSnClientFindGatewayPattern.h"
#include "common/discoveredgw/MqttSnClientDiscoveredGateway.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnClient_ {
    // network parameter
    int32_t  retry_timeout_ms;
    uint16_t retry_count;
    uint64_t  congestion_delay_s;
    uint8_t  default_signal_strength;
    // timeout tolerances
    uint16_t  low_timeout_threshold_s;
    int32_t low_threshold_tolerance_percentage;
    int32_t high_threshold_tolerance_percentage;

    MQTT_SN_CLIENT_STATUS status;
    uint16_t              msg_counter;

    uint8_t        clean_session;
    uint16_t       connect_duration;
    uint16_t       sleep_duration;
    device_address mqtt_sn_gateway_address;
    device_address mqtt_sn_broadcast_address;

    char *client_id;

    char *   will_topic;
    uint16_t will_topic_length;
    uint8_t *will_msg;
    uint16_t will_msg_len;
    int8_t   will_qos;
    uint8_t  will_retain;

    MqttSnClientRegistration registrations[MQTT_SN_CLIENT_MAX_REGISTRATIONS];
    uint16_t                 registrations_len;

    MqttSnGatewayNetworkInterface gatewayNetwork;
    int32_t                       gatewayNetworkSendTimeout;
    int32_t                       gatewayNetworkReceiveTimeout;

    MqttSnClientAwaitMessage await_msg[MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT];

    uint64_t                 last_ping_req_received;
    uint64_t                 last_ping_resp_received_s;
    MqttSnClientAwaitMessage ping_req_await_msg;
    int32_t                  connect_timeout_offset;

#ifdef WITH_PAHO_WAKEUP_WORKAROUND
    uint64_t ping_req_send_time_s;
    uint8_t  awake_any_msg_received;
#endif
    uint64_t awake_ping_request_send_time_s;

    int32_t (*publish_cb)(const struct MqttSnClient_ *client, const struct MqttSnPublish_ *publish);

    uint8_t                  searchgw_reply;
    MqttSnClientAwaitMessage searchgw_reply_await_msg;
    uint64_t                 searchgw_reply_await_last_s;
    uint16_t                 gwinfo_delay_ms;

#ifdef WITH_LOGGING
    MqttSnLogger logger;
#endif
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    MqttSnDiscoveredGatewayList discovered_gw_list;
#endif
#ifdef WITH_MQTT_SN_CLIENT_BUFFER
    MqttSnClientBuffer client_buffer;
#endif
#if defined(WITH_MQTT_SN_CLIENT_AUTO_CONNECT) || defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
    MQTT_SN_CLIENT_FIND_GW_PATTERN find_gw_pattern;
    int32_t                        find_gateway_wait_timeout_ms;
#endif
#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
    uint8_t  auto_reconnect_enabled;
    uint8_t  auto_reconnect_tried;
    uint8_t  auto_reconnect_congestion;
    uint64_t auto_reconnect_congestion_wait_s;
#endif
#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
    uint16_t gw_id_whitelist_max_len;
    uint8_t  gw_id_whitelist[MQTT_SN_CLIENT_GW_ID_WHITELIST_LENGTH];
    uint16_t gw_id_whitelist_len;

    uint16_t       gw_addr_whitelist_max_len;
    device_address gw_addr_whitelist[MQTT_SN_CLIENT_GW_ADDRESS_WHITELIST_LENGTH];
    uint16_t       gw_addr_whitelist_len;
#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
    uint16_t gw_id_blacklist_max_len;
    uint8_t  gw_id_blacklist[MQTT_SN_CLIENT_GW_ID_BLACKLIST_LENGTH];
    uint16_t gw_id_blacklist_len;

    uint16_t       gw_addr_blacklist_max_len;
    device_address gw_addr_blacklist[MQTT_SN_CLIENT_GW_ADDRESS_BLACKLIST_LENGTH];
    uint16_t       gw_addr_blacklist_len;
#endif
} MqttSnClient;

MqttSnClientAwaitContext *          get_await_context(MqttSnClient *client, int32_t await_fd);
MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status(MqttSnClient *client, int32_t await_fd);
int32_t                             free_await_fd(MqttSnClient *client, int32_t await_fd);
int32_t                             acquire_await_fd(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id);
MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status_timeout_ms_easy(MqttSnClient *client, int32_t await_fd, uint64_t timeout_ms);
MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status_timeout_ms(MqttSnClient *client, int32_t await_fd, uint64_t start_ts_ms,
                                                                uint64_t timeout_ms);


#ifdef __cplusplus
}
#endif

#endif  //CMQTTSNFORWARDER_MQTTSNCLIENT_H
