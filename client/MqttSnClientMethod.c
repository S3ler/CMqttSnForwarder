//
// Created by SomeDude on 31.05.2019.
//

#include "MqttSnClientMethod.h"
#include "MqttSnClientLogger.h"
#include "MqttSnClient.h"
#include "MqttSnClientMsgHandlerReturnValue.h"
#include "common/timeout/MqttSnClientTimeout.h"
#include <string.h>
#include <stdbool.h>
#include <logging/MqttSnLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <parser/MqttSnPublishMessage.h>
#include <parser/MqttSnConnectMessage.h>
#include <parser/MqttSnConnackMessage.h>
#include <parser/MqttSnPingReqMessage.h>
#include <parser/MqttSnPingRespMessage.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/MqttSnDisconnectMessage.h>
#include <parser/MqttSnWillMsgMessageLogging.h>
#include <parser/MqttSnWillTopicMessage.h>
#include <parser/MqttSnWillTopicReq.h>
#include <parser/MqttSnWillMsgReqMessage.h>
#include <parser/MqttSnRegisterMessage.h>
#include <parser/MqttSnRegAckMessage.h>
#include <parser/MqttSnPubrecPubrelPubcompMessage.h>
#include <parser/MqttSnPubackMessage.h>
#include <parser/MqttSnSubackMessage.h>
#include <parser/MqttSnSubscribeMessage.h>
#include <parser/MqttSnUnsubackMessage.h>
#include <parser/MqttSnUnsubscribeMessage.h>
#include <parser/logging/MqttSnConnectMessageLogging.h>
#include <parser/logging/MqttSnWillTopicMessageLogging.h>
#include <parser/logging/MqttSnWillMsgMessageLogging.h>
#include <parser/logging/MqttSnPingReqMessageLogging.h>
#include <parser/logging/MqttSnPubackMessageLogging.h>
#include <parser/logging/MqttSnPubrecMessageLogging.h>
#include <parser/logging/MqttSnPubCompMessageLogging.h>
#include <parser/logging/MqttSnPublishMessageLogging.h>
#include <parser/logging/MqttSnSearchGwMessageLogging.h>
#include <parser/logging/MqttSnDisconnectMessageLogging.h>
#include <parser/logging/MqttSnRegisterMessageLogging.h>
#include <parser/logging/MqttSnSubscribeMessageLogging.h>
#include <parser/logging/MqttSnUnsubscribeMessageLogging.h>
#include <parser/logging/MqttSnPubrelMessageLogging.h>
#include <parser/logging/MqttSnRegAckMessageLogging.h>
#include <parser/logging/MqttSnGwInfoMessageLogging.h>
#include <parser/logging/MqttSnMessageParserLogging.h>

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectClientCredentialInit(MqttSnClient *client, device_address *mqtt_sn_gateway_address,
                                                                         uint8_t clean_session, const char *client_id,
                                                                         uint16_t connect_duration);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectWillCredentialInit(MqttSnClient *client, const char *will_topic, int8_t will_qos,
                                                                       uint8_t will_retain, const uint8_t *will_msg,
                                                                       uint16_t will_msg_length);

MQTT_SN_CLIENT_RETURN_CODE await_status_to_client_return_code(MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status, const MqttSnLogger *logger);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishQosMessageHandler(MqttSnClient *client, int8_t qos, uint16_t msg_id, uint8_t *publish_msg,
                                                                uint16_t publish_msg_length);

int32_t set_await_msg_status(MqttSnClient *client, int32_t await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status);
int32_t set_await_msg_return_code(MqttSnClient *client, int32_t await_fd, MQTT_SN_RETURN_CODE return_code);
int32_t set_await_msg_granted_qos(MqttSnClient *client, int32_t await_fd, int8_t granted_qos);
int32_t set_await_msg_topic_id(MqttSnClient *client, int32_t await_fd, uint16_t topic_id);
int32_t is_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id);

int32_t                         MqttSnClientSafeStrlen(const char *str, int32_t max_len);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_advertise(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_searchgw(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_gwinfo(MqttSnClient *client, MqttSnMessageData *msg);

uint64_t get_last_timeout_reset(MqttSnClient *client);

int32_t                    send_to_gateway_without_retry(MqttSnClient *client, const uint8_t *msg_data, int32_t gen_rc);
MQTT_SN_CLIENT_RETURN_CODE send_to_gateway_with_retry(MqttSnClient *client, int32_t await_fd, uint8_t *msg_data, int32_t msg_len);

int32_t send_to_broadcast(MqttSnClient *client, const uint8_t *msg_data, int32_t gen_rc);

MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_client_asleep_connection(MqttSnClient *client);
MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_client_connection(MqttSnClient *client);
MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_client_awake_connection(MqttSnClient *client);

MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_disconnect(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_connack(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE client_parse_and_handle_ping_resp(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE client_parse_and_handle_ping_req(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_will_topic_request(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_will_message_request(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_regack(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_puback(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_pubrec(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_pubcomp(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_publish(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_register(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_suback(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_unsuback(MqttSnClient *client, MqttSnMessageData *msg);
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_pubrel(MqttSnClient *client, MqttSnMessageData *msg);

MQTT_SN_CLIENT_STATUS MqttSnClientInit(MqttSnClient *client, const MqttSnLogger *logger) {
    client->retry_timeout_ms        = MQTT_SN_CLIENT_DEFAULT_RETRY_TIMEOUT_MS;
    client->retry_count             = MQTT_SN_CLIENT_DEFAULT_RETRY_COUNT;
    client->default_signal_strength = MQTT_SN_CLIENT_DEFAULT_SIGNAL_STRENGTH;

    client->low_timeout_threshold_s             = MQTT_SN_CLIENT_DEFAULT_LOW_TIMEOUT_THRESHOLD;
    client->low_threshold_tolerance_percentage  = MQTT_SN_CLIENT_DEFAULT_LOW_TIMEOUT_THRESHOLD_PERCENTAGE;
    client->high_threshold_tolerance_percentage = MQTT_SN_CLIENT_DEFAULT_HIGHT_TIMEOUT_THRESHOLD_PERCENTAGE;

    client->status                 = MQTT_SN_CLIENT_CONNECTION_STATUS_UNINITALIZED;
    client->msg_counter            = 0;
    client->connect_timeout_offset = 0;

    client->ping_req_await_msg.msg_type = ANY_MESSAGE_TYPE;
    client->ping_req_await_msg.msg_id   = 0;

#ifdef WITH_LOGGING
    client->logger                = (*logger);
    client->gatewayNetwork.logger = &client->logger;
#endif

    for (uint16_t i = 0; i < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
        client->await_msg[i].msg_type = ANY_MESSAGE_TYPE;
        client->await_msg[i].msg_id   = 0;
    }

    if (GatewayNetworkConnect(&client->gatewayNetwork, client->gatewayNetwork.context) != 0) {
        client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_UNINITALIZED;
#ifdef WITH_LOGGING
        log_mqtt_sn_client_initialization_failed(&client->logger);
#endif
        return client->status;
    }

    client->searchgw_reply  = MQTT_SN_CLIENT_SEARCHGW_REPLY_DEFAULT_ENABLED;
    client->gwinfo_delay_ms = MQTT_SN_CLIENT_GWINFO_DELAY_MS;

#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    MqttSnDiscoveredGatewayListInit(&client->discovered_gw_list);
#endif

#if defined(WITH_MQTT_SN_CLIENT_AUTO_CONNECT) || defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
    client->find_gw_pattern = MQTT_SN_CLIENT_DEFAULT_FIND_GW_PATTERN;
#endif
#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
    client->auto_reconnect_enabled = WITH_MQTT_SN_CLIENT_AUTO_RECONNECT_DEFAULT_ENABLED;
#endif
#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
    MqttSnClientClearWhitelistGwId(client);
    MqttSnClientClearWhitelistNetworkAddress(client);
#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
    MqttSnClientClearBlacklistGwId(client);
    MqttSnClientClearBlacklistNetworkAddress(client);
#endif
    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED;
#ifdef WITH_LOGGING
    log_mqtt_sn_client_initialization_success(&client->logger);
#endif
    return client->status;
}
MQTT_SN_CLIENT_STATUS MqttSnClientDeinit(MqttSnClient *client) {
    if (client->status < MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED) {
        return client->status;
    }
    if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED) {
        if (GatewayNetworkDisconnect(&client->gatewayNetwork, client->gatewayNetwork.context) < 0) {
#ifdef WITH_LOGGING
            log_mqtt_sn_client_deinitialization_failed(&client->logger);
#endif
            return client->status;
        }
    }
    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_UNINITALIZED;
#ifdef WITH_LOGGING
    log_mqtt_sn_client_deinitialization_success(&client->logger);
#endif
    return client->status;
}

MQTT_SN_CLIENT_LOOP_RETURN_CODE MqttSnClientLoop(MqttSnClient *client) {
    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        if (handle_client_connection(client) != MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS) {
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
    } else if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP) {
        MQTT_SN_CLIENT_LOOP_RETURN_CODE rc = handle_client_asleep_connection(client);
        if (rc != MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS) {
            return rc;
        }
    } else if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE) {
        if (handle_client_awake_connection(client) != MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS) {
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
    }
#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
    else if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST) {
        // 0 = not tried, 1 retry failed,
        if (MqttSnClientIsAutoReconnectEnabled(client)) {
            if (client->auto_reconnect_enabled && client->auto_reconnect_congestion) {
                // update congestion timer
                uint64_t current_time_s;
                if (get_timestamp_s(&current_time_s) < 0) {
                    return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                }
                if (current_time_s - client->auto_reconnect_congestion_wait_s < client->congestion_delay_s) {
                    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
                }
                client->auto_reconnect_congestion = 0;
            }
            if (client->auto_reconnect_tried == 0) {
                client->auto_reconnect_tried  = 1;
                MQTT_SN_CLIENT_RETURN_CODE rc = MqttSnClientReconnect(client);
                if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
#ifdef WITH_LOGGING
                    log_mqtt_sn_client_reconnect_failed(&client->logger, &client->mqtt_sn_gateway_address);
#endif
                    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
                }
                if (rc == MQTT_SN_CLIENT_RETURN_ERROR) {
                    return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                } else if (rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                    // connect failed - use MqttSnClientAutoConnect
                } else if (rc == MQTT_SN_CLIENT_RETURN_CONGESTION) {
                    if (get_timestamp_s(&client->auto_reconnect_congestion_wait_s) < 0) {
                        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                    }
#ifdef WITH_LOGGING
                    log_mqtt_sn_client_reconnect_congestion(&client->logger, &client->mqtt_sn_gateway_address);
#endif
                    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
                }
#ifdef WITH_LOGGING
                log_mqtt_sn_client_reconnect_success(&client->logger, &client->mqtt_sn_gateway_address);
#endif
            }
            if (client->auto_reconnect_tried) {
                MQTT_SN_CLIENT_RETURN_CODE rc =
                  MqttSnClientAutoConnect(client, client->find_gw_pattern, client->find_gateway_wait_timeout_ms,
                                          &client->mqtt_sn_gateway_address, client->clean_session, client->client_id,
                                          client->connect_duration, client->will_topic, client->will_qos, client->will_retain,
                                          client->will_msg, client->will_msg_len);
                if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
                    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
                }
                if (rc == MQTT_SN_CLIENT_RETURN_SUCCESS) {
                    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
                } else if (rc == MQTT_SN_CLIENT_RETURN_ERROR) {
                    return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                } else if (rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                    return MQTT_SN_CLIENT_LOOP_RETURN_TIMEOUT;
                } else if (rc == MQTT_SN_CLIENT_RETURN_CONGESTION) {
                    if (get_timestamp_s(&client->auto_reconnect_congestion_wait_s) < 0) {
                        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                    }
                    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
                }
            }
        }
    }
#endif

#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    if (MqttSnClientIsSearchGwReplyEnabled(client) && client->searchgw_reply_await_msg.msg_type == GWINFO) {
        uint64_t current_time_s;
        if (get_timestamp_s(&current_time_s) < 0) {
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
        if (current_time_s - client->searchgw_reply_await_last_s > (client->gwinfo_delay_ms / (uint64_t)1000)) {
            if (MqttSnDiscoveredGatewayListUpdate(&client->discovered_gw_list, NULL, 0, NULL, NULL, current_time_s) < 0) {
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
            if (client->discovered_gw_list.current_len > 0) {
                // we simply take the first one
                MqttSnDiscoveredGateway *discovered_gateway = &client->discovered_gw_list.gateways[0];

                uint8_t msg_data[MQTT_SN_MESSAGE_GWINFO_W_GWADD_MAX_LENGTH] = { 0 };
                int32_t gen_rc;
                if ((gen_rc = generate_gwinfo_message(msg_data, sizeof(msg_data), discovered_gateway->gw_id,
                                                      &discovered_gateway->network_address, sizeof(device_address))) < 0) {
#ifdef WITH_DEBUG_LOGGING
                    log_gwinfo_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                    return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                }
                int32_t send_rc = send_to_broadcast(client, msg_data, gen_rc);
                if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
                    log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
                    return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                }
            }
        }
    }
#endif

    MqttSnMessageData msg     = { 0 };
    int32_t           msg_len = 0;
    if ((msg_len = GatewayNetworkReceiveFom(&client->gatewayNetwork, &msg.from, &msg.to, msg.data, sizeof(msg.data), &msg.signal_strength,
                                            client->gatewayNetworkReceiveTimeout, client->gatewayNetwork.context)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (msg_len == 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    }

#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
    if (client->gw_addr_whitelist_len != 0) {
        if (!MqttSnClientWhitelistContainsNetworkAddress(client, msg.from)) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }
#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
    if (client->gw_addr_blacklist_len != 0) {
        if (MqttSnClientBlacklistContainsNetworkAddress(client, msg.from)) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }
#endif

    msg.data_length           = msg_len;
    ParsedMqttSnHeader header = { 0 };

    int32_t parsed_bytes = 0;
    if (parse_header(&header, ANY_MESSAGE_TYPE, msg.data, msg.data_length, &parsed_bytes) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, &msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED &&
        memcmp(&msg.from, &client->mqtt_sn_gateway_address, sizeof(device_address)) == 0) {
        MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_rc;
        switch (header.msg_type) {
        case ADVERTISE:
            handle_rc = parse_and_handle_advertise(client, &msg);
            break;
        case GWINFO:
            handle_rc = parse_and_handle_gwinfo(client, &msg);
            break;
        case WILLTOPICREQ:
            handle_rc = parse_and_handle_will_topic_request(client, &msg);
            break;
        case WILLMSGREQ:
            handle_rc = parse_and_handle_will_message_request(client, &msg);
            break;
        case CONNACK:
            handle_rc = parse_and_handle_connack(client, &msg);
            break;
        default:
            handle_rc = MQTT_SN_CLIENT_LOOP_RETURN_UNHANDLEDMSG;
#ifdef WITH_DEBUG_LOGGING
            log_mqtt_sn_client_unhandled_message(&client->logger, &msg);
#endif
            break;
        }
#ifdef WITH_LOGGING
        log_mqtt_sn_client_any_loop_message(&client->logger, client->status, handle_rc, &msg);
#endif
        return handle_rc;
    }

    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED &&
        memcmp(&msg.from, &client->mqtt_sn_gateway_address, sizeof(device_address)) == 0) {
        MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_rc;
        switch (header.msg_type) {
        case ADVERTISE:
            // should be a broadcast
            handle_rc = parse_and_handle_advertise(client, &msg);
            break;
        case GWINFO:
            // should be a broadcast
            handle_rc = parse_and_handle_gwinfo(client, &msg);
            break;
        case PINGRESP:
            handle_rc = client_parse_and_handle_ping_resp(client, &msg);
            break;
        case PINGREQ:
            handle_rc = client_parse_and_handle_ping_req(client, &msg);
            break;
        case CONNACK:
            handle_rc = parse_and_handle_connack(client, &msg);
            break;
        case DISCONNECT:
            handle_rc = parse_and_handle_disconnect(client, &msg);
            break;
        case REGACK:
            handle_rc = parse_and_handle_regack(client, &msg);
            break;
        case PUBACK:
            handle_rc = parse_and_handle_puback(client, &msg);
            break;
        case PUBREC:
            handle_rc = parse_and_handle_pubrec(client, &msg);
            break;
        case PUBCOMP:
            handle_rc = parse_and_handle_pubcomp(client, &msg);
            break;
        case PUBREL:
            handle_rc = parse_and_handle_pubrel(client, &msg);
            break;
        case SUBACK:
            handle_rc = parse_and_handle_suback(client, &msg);
            break;
        case UNSUBACK:
            handle_rc = parse_and_handle_unsuback(client, &msg);
            break;
        case PUBLISH:
            handle_rc = parse_and_handle_publish(client, &msg);
            break;
        case REGISTER:
            handle_rc = parse_and_handle_register(client, &msg);
            break;
        default:
            handle_rc = MQTT_SN_CLIENT_LOOP_RETURN_UNHANDLEDMSG;
#ifdef WITH_DEBUG_LOGGING
            log_mqtt_sn_client_unhandled_message(&client->logger, &msg);
#endif
            break;
        }
#ifdef WITH_LOGGING
        log_mqtt_sn_client_any_loop_message(&client->logger, client->status, handle_rc, &msg);
#endif
        return handle_rc;
    }
#ifdef WITH_PAHO_WAKEUP_WORKAROUND
    if ((client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP || client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE) &&
#else
    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE &&
#endif
        memcmp(&msg.from, &client->mqtt_sn_gateway_address, sizeof(device_address)) == 0) {
        MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_rc;
        switch (header.msg_type) {
        case ADVERTISE:
            // should be a broadcast
            handle_rc = parse_and_handle_advertise(client, &msg);
            break;
        case GWINFO:
            // should be a broadcast
            handle_rc = parse_and_handle_gwinfo(client, &msg);
            break;
        case PINGRESP:
            handle_rc = client_parse_and_handle_ping_resp(client, &msg);
            break;
        case PINGREQ:
            handle_rc = client_parse_and_handle_ping_req(client, &msg);
            break;
        case DISCONNECT:
            handle_rc = parse_and_handle_disconnect(client, &msg);
            break;
        case REGACK:
            handle_rc = parse_and_handle_regack(client, &msg);
            break;
        case PUBACK:
            handle_rc = parse_and_handle_puback(client, &msg);
            break;
        case PUBREC:
            handle_rc = parse_and_handle_pubrec(client, &msg);
            break;
        case PUBCOMP:
            handle_rc = parse_and_handle_pubcomp(client, &msg);
            break;
        case PUBREL:
            handle_rc = parse_and_handle_pubrel(client, &msg);
            break;
        case SUBACK:
            handle_rc = parse_and_handle_suback(client, &msg);
            break;
        case UNSUBACK:
            handle_rc = parse_and_handle_unsuback(client, &msg);
            break;
        case PUBLISH:
            handle_rc = parse_and_handle_publish(client, &msg);
            break;
        default:
            handle_rc = MQTT_SN_CLIENT_LOOP_RETURN_UNHANDLEDMSG;
#ifdef WITH_DEBUG_LOGGING
            log_mqtt_sn_client_unhandled_message(&client->logger, &msg);
#endif
            break;
        }
#ifdef WITH_PAHO_WAKEUP_WORKAROUND
        if (handle_rc == MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS && !client->awake_any_msg_received) {
            client->awake_any_msg_received = true;
        }
#endif
#ifdef WITH_LOGGING
        log_mqtt_sn_client_any_loop_message(&client->logger, client->status, handle_rc, &msg);
#endif
        if (handle_rc == MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS) {
            if (get_timestamp_s(&client->awake_ping_request_send_time_s) < 0) {
                handle_rc = MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
        }
        return handle_rc;
    }

    if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED &&
        memcmp(&msg.to, &client->mqtt_sn_broadcast_address, sizeof(device_address)) == 0) {
        MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_rc;
        switch (header.msg_type) {
        case ADVERTISE:
            handle_rc = parse_and_handle_advertise(client, &msg);
            break;
        case SEARCHGW:
            handle_rc = parse_and_handle_searchgw(client, &msg);
            break;
        case GWINFO:
            handle_rc = parse_and_handle_gwinfo(client, &msg);
            break;
        default:
            handle_rc = MQTT_SN_CLIENT_LOOP_RETURN_UNHANDLEDMSG;
#ifdef WITH_DEBUG_LOGGING
            log_mqtt_sn_client_unhandled_message(&client->logger, &msg);
#endif
            break;
        }
#ifdef WITH_LOGGING
        log_mqtt_sn_client_any_loop_message(&client->logger, client->status, handle_rc, &msg);
#endif
        return handle_rc;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_UNHANDLEDMSG;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_pubrel(MqttSnClient *client, MqttSnMessageData *msg) {
    uint16_t msg_id;
    if (parse_pubrel_bytes(&msg_id, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, PUBREL, msg_id)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }

    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_unsuback(MqttSnClient *client, MqttSnMessageData *msg) {
    uint16_t msg_id;
    if (parse_unsuback_byte(&msg_id, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, UNSUBACK, msg_id)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }

    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_suback(MqttSnClient *client, MqttSnMessageData *msg) {
    MqttSnSuback suback = { 0 };
    if (parse_suback(&suback, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, SUBACK, suback.msg_id)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }
    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    if (set_await_msg_return_code(client, await_fd, suback.return_code) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (set_await_msg_granted_qos(client, await_fd, suback.granted_qos) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (set_await_msg_topic_id(client, await_fd, suback.topic_id) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_publish(MqttSnClient *client, MqttSnMessageData *msg) {
    MqttSnPublish publish                                      = { 0 };
    uint8_t       payload[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH] = { 0 };
    if (parse_publish_byte(&publish.dup, &publish.qos, &publish.retain, &publish.topic_id_type, &publish.topic_id, &publish.msg_id, payload,
                           &publish.payload_length, sizeof(payload), msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }
    publish.payload = payload;

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, PUBLISH, publish.msg_id)) >= 0) {
        if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
    }

    if (publish.qos == 0) {
        // send nothing
        if (client->publish_cb) {
            if (client->publish_cb(client, &publish)) {
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    } else if (publish.qos == 1) {
        // send PUBACK
        uint8_t msg_data[MQTT_SN_MESSAGE_PUBACK_LENGTH];
        int32_t gen_rc = generate_puback(msg_data, sizeof(msg_data), publish.topic_id, publish.msg_id, RETURN_CODE_ACCEPTED);
        if (gen_rc < 0) {
#ifdef WITH_LOGGING
            log_puback_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }

        MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, -1, msg_data, gen_rc);
        free_await_fd(client, await_fd);
        if (rc == MQTT_SN_CLIENT_RETURN_ERROR) {
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
        /*
        int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
        if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
        */
        if (client->publish_cb) {
            if (client->publish_cb(client, &publish) < 0) {
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
        }
        if (rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_LOOP_RETURN_TIMEOUT;
        }
    } else if (publish.qos == 2) {
        {  // send PUBREC
            uint8_t msg_data[MQTT_SN_MESSAGE_PUBREC_LENGTH] = { 0 };

            int32_t gen_rc = generate_pubrec(msg_data, sizeof(msg_data), publish.msg_id);
            if (gen_rc < 0) {
#ifdef WITH_LOGGING
                log_pubrec_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }

            int32_t pubrel_await_fd;
            while ((pubrel_await_fd = acquire_await_fd(client, PUBREL, publish.msg_id)) < 0) {
                if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
                    log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                    return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                }
            }

#ifndef WITH_PAHO_AWAKE_QOS_WORKAROUND
            MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, pubrel_await_fd, msg_data, gen_rc);
            free_await_fd(client, await_fd);
            if (rc == MQTT_SN_CLIENT_RETURN_ERROR) {
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
#endif
            int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
            if (send_rc < 0 || send_rc != gen_rc) {
                if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
                    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
#ifdef WITH_LOGGING
                    log_mqtt_sn_client_status_gateway_lost(&client->logger, &client->mqtt_sn_gateway_address);
#endif
                }
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
#ifdef WITH_PAHO_AWAKE_QOS_WORKAROUND
            if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE) {
                // with paho we do not receive an answer to a PubRec (missing PubRel) we assume we got it
                free_await_fd(client, pubrel_await_fd);
                if (client->publish_cb) {
                    if (client->publish_cb(client, &publish) < 0) {
                        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                    }
                }
                return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
            }
#endif
            // receive PUBREL
            MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc =
              get_await_status_timeout_ms_easy(client, pubrel_await_fd, client->retry_timeout_ms);
            MQTT_SN_CLIENT_RETURN_CODE client_rc = await_status_to_client_return_code(await_rc, &client->logger);
            if (client_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                free_await_fd(client, pubrel_await_fd);
                return MQTT_SN_CLIENT_LOOP_RETURN_QOS2RECEIVETIMOUT;
            }
            free_await_fd(client, pubrel_await_fd);
        }
        {  // send PUBCOMP
            uint8_t msg_data[MQTT_SN_MESSAGE_PUBCOMP_LENGTH] = { 0 };

            int32_t gen_rc = generate_pubcomp(msg_data, sizeof(msg_data), publish.msg_id);
            if (gen_rc < 0) {
#ifdef WITH_LOGGING
                log_pubcomp_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }

            int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
            if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
        }
        if (client->publish_cb) {
            if (client->publish_cb(client, &publish) < 0) {
                return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
            }
        }
    } else {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_pubcomp(MqttSnClient *client, MqttSnMessageData *msg) {
    uint16_t msg_id;
    if (parse_pubcomp_bytes(&msg_id, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }
    int32_t await_fd;
    if ((await_fd = is_await_msg(client, PUBCOMP, msg_id)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }
    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_pubrec(MqttSnClient *client, MqttSnMessageData *msg) {
    uint16_t msg_id;
    if (parse_pubrec_bytes(&msg_id, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, PUBREC, msg_id)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }
    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_puback(MqttSnClient *client, MqttSnMessageData *msg) {
    MqttSnPuback puback;
    if (parse_puback(&puback, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, PUBACK, puback.msgId)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }
    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (set_await_msg_return_code(client, await_fd, puback.returnCode) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_regack(MqttSnClient *client, MqttSnMessageData *msg) {
    MqttSnRegack regack = { 0 };
    if (parse_regack(&regack, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, REGACK, regack.msg_id)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }
    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    if (set_await_msg_return_code(client, await_fd, regack.return_code) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (set_await_msg_topic_id(client, await_fd, regack.topic_id) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_will_message_request(MqttSnClient *client, MqttSnMessageData *msg) {
    if (parse_will_msg_req_byte(msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, WILLMSGREQ, 0)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }
    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_will_topic_request(MqttSnClient *client, MqttSnMessageData *msg) {
    if (parse_will_topic_req_byte(msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, WILLTOPICREQ, 0)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }

    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE client_parse_and_handle_ping_resp(MqttSnClient *client, MqttSnMessageData *msg) {
    if (parse_ping_resp_byte(msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    if (get_timestamp_s(&client->last_ping_resp_received_s) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    if (client->ping_req_await_msg.msg_type == PINGRESP) {
        // answer to a PingRequest from automatic active, wakeup, sleep connection handling.
        client->ping_req_await_msg.msg_type = ANY_MESSAGE_TYPE;
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    }

    int32_t await_fd = 0;
    if ((await_fd = is_await_msg(client, PINGRESP, 0)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }

    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
int32_t send_to_gateway_without_retry(MqttSnClient *client, const uint8_t *msg_data, int32_t gen_rc) {
    int32_t send_rc =
      GatewayNetworkSendTo(&client->gatewayNetwork, NULL, &client->mqtt_sn_gateway_address, msg_data, gen_rc,
                           client->default_signal_strength, client->gatewayNetworkSendTimeout, client->gatewayNetwork.context);
    return send_rc;
}
MQTT_SN_CLIENT_RETURN_CODE send_to_gateway_with_retry(MqttSnClient *client, int32_t await_fd, uint8_t *msg_data, int32_t msg_len) {
    for (uint16_t retry = 0; retry < client->retry_count; ++retry) {
        if (retry == 1) {
            if (add_dup_flag_to_mqtt_sn_message(msg_data, msg_len) < 0) {
                if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
                    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
#ifdef WITH_LOGGING
                    log_mqtt_sn_client_status_gateway_lost(&client->logger, &client->mqtt_sn_gateway_address);
#endif
                }
#ifdef WITH_LOGGING
                log_fatal_cannot_add_dup_flag(&client->logger, __FILE__, __func__, __LINE__, msg_data, msg_len);
#endif
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }
        }
        int32_t send_rc = send_to_gateway_without_retry(client, msg_data, msg_len);
        if (send_rc < 0 || send_rc != msg_len) {
            if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
                client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
#ifdef WITH_LOGGING
                log_mqtt_sn_client_status_gateway_lost(&client->logger, &client->mqtt_sn_gateway_address);
#endif
            }
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, msg_len);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        if (await_fd >= 0) {
            MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
            MQTT_SN_CLIENT_RETURN_CODE          rc       = await_status_to_client_return_code(await_rc, &client->logger);
            if (rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                continue;
            }
            return rc;
        } else {
            // not reply => success
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }

    if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
#ifdef WITH_LOGGING
        log_mqtt_sn_client_status_gateway_lost(&client->logger, &client->mqtt_sn_gateway_address);
#endif
    }

#ifdef WITH_MQTT_SN_CLIENT_AUTO_RECONNECT

#endif
    return MQTT_SN_CLIENT_RETURN_TIMEOUT;
}
int32_t send_to_broadcast(MqttSnClient *client, const uint8_t *msg_data, int32_t gen_rc) {
    int32_t send_rc =
      GatewayNetworkSendTo(&client->gatewayNetwork, NULL, &client->mqtt_sn_broadcast_address, msg_data, gen_rc,
                           client->default_signal_strength, client->gatewayNetworkSendTimeout, client->gatewayNetwork.context);
    return send_rc;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_client_connection(MqttSnClient *client) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    uint64_t current_time = 0;
    if (get_timestamp_s(&current_time) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    uint64_t elapsed_time = current_time - get_last_timeout_reset(client);

    if (client->ping_req_await_msg.msg_type == PINGRESP) {
        // ping request already in flight

        // check if ping request timed out
        if (elapsed_time > get_tolerance_timeout(client->connect_duration, client->low_timeout_threshold_s,
                                                 client->low_threshold_tolerance_percentage, client->high_threshold_tolerance_percentage)) {
            client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
#ifdef WITH_LOGGING
            log_mqtt_sn_client_status_gateway_lost(&client->logger, &client->mqtt_sn_gateway_address);
#endif
        }
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    }

    if (client->connect_duration <= MQTT_SN_CLIENT_PING_REQ_ADVANCE_TIME_S) {
        // if the connect_duration is lower than the ping request advance time than we use the connect duration
        if (elapsed_time < client->connect_duration) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    } else {
        // use
        if (elapsed_time < (uint64_t)client->connect_duration + client->connect_timeout_offset) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_PINGREQ_LENGTH] = { 0 };
    int32_t gen_rc                                   = generate_ping_req(msg_data, sizeof(msg_data));
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_pingreq_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    client->ping_req_await_msg.msg_type = PINGRESP;
#ifdef WITH_LOGGING
    log_mqtt_sn_client_connection_maintain_connection(&client->logger, &client->mqtt_sn_gateway_address);
#endif
    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}

uint64_t get_last_timeout_reset(MqttSnClient *client) {
    uint64_t result = client->last_ping_resp_received_s;
    if (client->last_ping_req_received > client->last_ping_resp_received_s) {
        result = client->last_ping_req_received;
    }
    return result;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_connack(MqttSnClient *client, MqttSnMessageData *msg) {
    ParsedMqttSnConnack mqtt_sn_connack;
    memset(&mqtt_sn_connack, 0, sizeof(mqtt_sn_connack));
    if (parse_connack(&mqtt_sn_connack, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, CONNACK, 0)) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG;
    }

    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (set_await_msg_return_code(client, await_fd, mqtt_sn_connack.return_code) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    uint64_t current_time_s;
    if (get_timestamp_s(&current_time_s) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    client->last_ping_req_received    = current_time_s;
    client->last_ping_resp_received_s = current_time_s;

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE client_parse_and_handle_ping_req(MqttSnClient *client, MqttSnMessageData *msg) {
    if (parse_ping_req_byte(msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    if (get_timestamp_s(&client->last_ping_req_received) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_PINGREQ_LENGTH] = { 0 };
    int32_t gen_rc                                   = generate_ping_resp(msg_data, sizeof(msg_data));
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_pingreq_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_disconnect(MqttSnClient *client, MqttSnMessageData *msg) {
    if (parse_disconnect_bytes(msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    int32_t await_fd;
    if ((await_fd = is_await_msg(client, DISCONNECT, 0)) < 0) {
        // out of order disconnect
        client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED;
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    }

    if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_advertise(MqttSnClient *client, MqttSnMessageData *msg) {
    MqttSnAdvertise mqtt_sn_advertise = { 0 };
    if (parse_advertise(&mqtt_sn_advertise, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
    if (client->gw_addr_whitelist_len != 0) {
        if (!MqttSnClientWhitelistContainsGwId(client, mqtt_sn_advertise.gwId)) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }

#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
    if (client->gw_addr_blacklist_len != 0) {
        if (MqttSnClientBlacklistContainsGwId(client, mqtt_sn_advertise.gwId)) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }
#endif

    MqttSnClientReceivedAdvertise received_advertise = { .received_advertise = mqtt_sn_advertise,
                                                         .from               = msg->from,
                                                         .signal_strength    = msg->signal_strength };

    MQTT_SN_CLIENT_LOOP_RETURN_CODE rc = MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    int32_t                         await_fd;
    if ((await_fd = is_await_msg(client, ADVERTISE, 0)) >= 0) {
        MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
        if (await_context) {
            if (await_context->dst_advertise) {
                await_context->dst_advertise->received_advertise = received_advertise.received_advertise;
                await_context->dst_advertise->from               = received_advertise.from;
                await_context->dst_advertise->signal_strength    = received_advertise.signal_strength;
            }
            if (await_context->adv_cb) {
                if (await_context->adv_cb(&received_advertise, await_context->context) < 0) {
                    rc = MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                }
            }
        }

        if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
            rc = MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
    }

#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST)
    uint64_t current_time_s;
    if (get_timestamp_s(&current_time_s) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (MqttSnDiscoveredGatewayListUpdate(&client->discovered_gw_list, &received_advertise.from, msg->signal_strength, &mqtt_sn_advertise,
                                          NULL, current_time_s) < 0) {
    }
#endif

    return rc;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_searchgw(MqttSnClient *client, MqttSnMessageData *msg) {
    MqttSnSearchGw mqtt_sn_searchgw = { 0 };
    if (parse_searchgw(&mqtt_sn_searchgw, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    if (MqttSnClientIsSearchGwReplyEnabled(client)) {
        if (client->searchgw_reply_await_msg.msg_type == ANY_MESSAGE_TYPE) {
            client->searchgw_reply_await_msg.msg_type = GWINFO;
        }
        if (get_timestamp_s(&client->searchgw_reply_await_last_s) < 0) {
            return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
    }
#endif
    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_gwinfo(MqttSnClient *client, MqttSnMessageData *msg) {
    MqttSnGwInfo mqtt_sn_gwinfo = { 0 };
    if (parse_gwinfo(&mqtt_sn_gwinfo, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
    if (client->gw_addr_whitelist_len != 0) {
        if (!MqttSnClientWhitelistContainsGwId(client, mqtt_sn_gwinfo.gwId)) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }

#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
    if (client->gw_addr_blacklist_len != 0) {
        if (MqttSnClientBlacklistContainsGwId(client, mqtt_sn_gwinfo.gwId)) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }
#endif

    MqttSnClientReceivedGwInfo received_gw_info = { .received_gw_info = mqtt_sn_gwinfo,
                                                    .from             = msg->from,
                                                    .signal_strength  = msg->signal_strength };

    MQTT_SN_CLIENT_LOOP_RETURN_CODE rc = MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    int32_t                         await_fd;
    if ((await_fd = is_await_msg(client, GWINFO, 0)) >= 0) {
        // search gateway
        MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
        if (await_context) {
            if (await_context->dst_gwinfo) {
                await_context->dst_gwinfo->received_gw_info = received_gw_info.received_gw_info;
                await_context->dst_gwinfo->signal_strength  = received_gw_info.signal_strength;
                await_context->dst_gwinfo->from             = received_gw_info.from;
            }
            if (await_context->gwinfo_cb) {
                if (await_context->gwinfo_cb(&received_gw_info, await_context->context) < 0) {
                    rc = MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
                }
            }
        }
        if (set_await_msg_status(client, await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) < 0) {
            rc = MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
        }
    }

#if defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    uint64_t current_time_s;
    if (get_timestamp_s(&current_time_s) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (MqttSnDiscoveredGatewayListUpdate(&client->discovered_gw_list, &msg->from, msg->signal_strength, NULL, &mqtt_sn_gwinfo,
                                          current_time_s) < 0) {
        rc = MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
#endif
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    if (MqttSnClientIsSearchGwReplyEnabled(client)) {
        if (client->searchgw_reply_await_msg.msg_type == GWINFO) {
            client->searchgw_reply_await_msg.msg_type = ANY_MESSAGE_TYPE;
        }
    }
#endif
    return rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishPredefinedM1(MqttSnClient *client, uint16_t predefined_topic_id, uint8_t retain,
                                                           uint8_t *data, uint16_t data_len) {
    int32_t publish_msg_length = get_publish_msg_len(data_len);
    if (publish_msg_length < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    uint8_t publish_msg[publish_msg_length];

    uint16_t msg_id = 0;
    int8_t   qos    = -1;
    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        msg_id = client->msg_counter++;
    }

    int32_t gen_rc = generate_publish(publish_msg, sizeof(publish_msg), 0, qos, retain, MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID,
                                      predefined_topic_id, msg_id, data, data_len);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_publish_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientPublishQosMessageHandler(client, qos, msg_id, publish_msg, sizeof(publish_msg));
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnect(MqttSnClient *client, device_address *mqtt_sn_gateway_address, uint8_t clean_session,
                                                     const char *client_id, uint16_t connect_duration, const char *will_topic,
                                                     int8_t will_qos, uint8_t will_retain, const uint8_t *will_msg,
                                                     uint16_t will_msg_length) {
    if (MqttSnClientDirectConnectClientCredentialInit(client, mqtt_sn_gateway_address, clean_session, client_id, connect_duration) !=
        MQTT_SN_CLIENT_RETURN_SUCCESS) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    bool will = false;
    if (will_topic != NULL && will_msg != NULL) {
        will = true;
    }

    if (will) {
        return MqttSnClientDirectConnectWithWill(client, mqtt_sn_gateway_address, clean_session, client_id, connect_duration, will_topic,
                                                 will_qos, will_retain, will_msg, will_msg_length);
    }
    return MqttSnClientDirectConnectWithoutWill(client, mqtt_sn_gateway_address, clean_session, client_id, connect_duration);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAwaitAdvertise(MqttSnClient *client, int32_t timeout_s,
                                                      MqttSnClientReceivedAdvertise *dst_advertise) {
    int32_t await_fd;
    while ((await_fd = acquire_await_fd(client, ADVERTISE, 0)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }
    MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
    if (await_context == NULL) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    await_context->dst_advertise = dst_advertise;

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, timeout_s * 1000);
    MQTT_SN_CLIENT_RETURN_CODE          rc       = await_status_to_client_return_code(await_rc, &client->logger);
    free_await_fd(client, await_fd);
    return rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSearchGw(MqttSnClient *client, int32_t random_timeout_ms, uint8_t radius,
                                                MqttSnClientReceivedGwInfo *dst_gwinfo) {
    // wait random time - if now gw info received send searchgw - then wait until timeout_ms
    int32_t await_fd;
    while ((await_fd = acquire_await_fd(client, GWINFO, 0)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
    if (await_context == NULL) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    await_context->dst_gwinfo = dst_gwinfo;

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, random_timeout_ms);
    if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
        free_await_fd(client, await_fd);
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR) {
        free_await_fd(client, await_fd);
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_SEARCHGW_LENGTH] = { 0 };
    int32_t gen_rc                                    = generate_searchgw_message(msg_data, sizeof(msg_data), radius);
    if (gen_rc < 0) {
        free_await_fd(client, await_fd);
#ifdef WITH_LOGGING
        log_searchgw_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t send_rc = send_to_broadcast(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
        free_await_fd(client, await_fd);
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    await_rc                      = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
    MQTT_SN_CLIENT_RETURN_CODE rc = await_status_to_client_return_code(await_rc, &client->logger);
    free_await_fd(client, await_fd);
    return rc;
}
int32_t MqttSnClientSafeStrlen(const char *str, int32_t max_len) {
    const char *end = (const char *)memchr(str, '\0', max_len);
    if (end == NULL)
        return -1;
    else
        return end - str;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDisconnect(MqttSnClient *client) {
    if (client->status <= MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_DISCONNECT_WO_DURATION_LENGTH] = { 0 };
    int32_t gen_rc                                                  = generate_disconnect_message(msg_data, sizeof(msg_data));
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_disconnect_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t await_fd;
    while ((await_fd = acquire_await_fd(client, DISCONNECT, 0)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
    free_await_fd(client, await_fd);
    return rc;
    /*
    int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_RETURN_NETWORK_ERROR;
    }

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
    client->status                               = MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED;
    free_await_fd(client, await_fd);
    return await_status_to_client_return_code(await_rc, &client->logger);
          */
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientGetRegisteredTopicId(MqttSnClient *client, const char *topic_name, uint16_t *dst_topic_id) {
    int32_t topic_name_length;
    if ((topic_name_length = MqttSnClientSafeStrlen(topic_name, MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH - 1)) < 0) {
#ifdef WITH_LOGGING
        log_mqtt_sn_client_registrations_topic_name_too_long(&client->logger, topic_name_length,
                                                             MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    for (uint16_t i = 0; i < client->registrations_len; i++) {
        if (strcmp(client->registrations[i].topic_name, topic_name) == 0) {
            (*dst_topic_id) = client->registrations[i].topic_id;
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    return MQTT_SN_CLIENT_RETURN_ERROR;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRegister(MqttSnClient *client, const char *topic_name, uint16_t topic_name_length) {
    if (client->status <= MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint16_t topic_id;
    if (MqttSnClientGetRegisteredTopicId(client, topic_name, &topic_id) == MQTT_SN_CLIENT_RETURN_SUCCESS) {
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    if (client->registrations_len >= MQTT_SN_CLIENT_MAX_REGISTRATIONS) {
#ifdef WITH_LOGGING
        log_mqtt_sn_client_registrations_full(&client->logger, client->registrations_len, MQTT_SN_CLIENT_MAX_REGISTRATIONS);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (topic_name_length > MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH) {
#ifdef WITH_LOGGING
        log_mqtt_sn_client_registrations_topic_name_too_long(&client->logger, topic_name_length,
                                                             MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint16_t msg_id = client->msg_counter++;

    int32_t await_fd;
    while ((await_fd = acquire_await_fd(client, REGACK, msg_id)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
    if (await_context == NULL) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_await_context_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t msg_data_length = get_register_msg_len(topic_name_length);
    if (msg_data_length < 0) {
#ifdef WITH_LOGGING
        log_register_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    uint8_t msg_data[msg_data_length];
    int32_t gen_rc = generate_register(msg_data, msg_data_length, 0, msg_id, topic_name, topic_name_length);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_register_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
    if (rc == MQTT_SN_CLIENT_RETURN_SUCCESS) {
        switch (await_context->return_code) {
        case RETURN_CODE_ACCEPTED:
            strncpy(client->registrations[client->registrations_len].topic_name, topic_name, topic_name_length);
            client->registrations[client->registrations_len].topic_id = await_context->topic_id;
            client->registrations_len += 1;
            rc = MQTT_SN_CLIENT_RETURN_SUCCESS;
            break;
        case RETURN_CODE_REJECTED_CONGESTION:
            rc = MQTT_SN_CLIENT_RETURN_CONGESTION;
            break;
        case RETURN_CODE_REJCETED_NOT_SUPPORTED:
            rc = MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED;
            break;
        default:
            rc = MQTT_SN_CLIENT_RETURN_ERROR;
            break;
        }
    }
    free_await_fd(client, await_fd);

    /*
    int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);

    MQTT_SN_CLIENT_RETURN_CODE rc;
    if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
        switch (await_context->return_code) {
        case RETURN_CODE_ACCEPTED:
            strncpy(client->registrations[client->registrations_len].topic_name, topic_name, topic_name_length);
            client->registrations[client->registrations_len].topic_id = await_context->topic_id;
            client->registrations_len += 1;
            rc = MQTT_SN_CLIENT_RETURN_SUCCESS;
            break;
        case RETURN_CODE_REJECTED_CONGESTION:
            rc = MQTT_SN_CLIENT_RETURN_CONGESTION;
            break;
        case RETURN_CODE_REJCETED_NOT_SUPPORTED:
            rc = MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED;
            break;
        default:
            rc = MQTT_SN_CLIENT_RETURN_ERROR;
            break;
        }
    } else if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
        rc = MQTT_SN_CLIENT_RETURN_TIMEOUT;
    } else {
        rc = MQTT_SN_CLIENT_RETURN_ERROR;
    }
    free_await_fd(client, await_fd);
    */
    return rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicName(MqttSnClient *client, const char *topic_name, uint16_t topic_name_length,
                                                        uint8_t retain, int8_t qos, uint8_t *data, uint16_t data_len) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    MQTT_SN_CLIENT_RETURN_CODE register_rc;
    if ((register_rc = MqttSnClientRegister(client, topic_name, topic_name_length)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        return register_rc;
    }
    uint16_t topic_id;
    if (MqttSnClientGetRegisteredTopicId(client, topic_name, &topic_id) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientPublishTopicId(client, topic_id, retain, qos, data, data_len);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishPredefined(MqttSnClient *client, uint16_t predefined_topic_id, uint8_t retain, int8_t qos,
                                                         uint8_t *data, uint16_t data_len) {
    if (client->status <= MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (qos >= 0 && client->status < MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    uint16_t msg_id = 0;
    if (qos >= 0 && client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        msg_id = client->msg_counter++;
    }
    int32_t publish_msg_length = get_publish_msg_len(data_len);
    if (publish_msg_length < 0) {
#ifdef WITH_LOGGING
        log_publish_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint8_t publish_msg[publish_msg_length];
    int32_t gen_rc = generate_publish(publish_msg, sizeof(publish_msg), 0, qos, retain, MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID,
                                      predefined_topic_id, msg_id, data, data_len);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_publish_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientPublishQosMessageHandler(client, qos, msg_id, publish_msg, sizeof(publish_msg));
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicId(MqttSnClient *client, uint16_t topic_id, uint8_t retain, int8_t qos, uint8_t *data,
                                                      uint16_t data_len) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t publish_msg_length = get_publish_msg_len(data_len);
    if (publish_msg_length < 0) {
#ifdef WITH_LOGGING
        log_publish_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    uint16_t msg_id = client->msg_counter++;
    uint8_t  publish_msg[publish_msg_length];
    int32_t  gen_rc = generate_publish(publish_msg, sizeof(publish_msg), 0, qos, retain, MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME, topic_id,
                                      msg_id, data, data_len);  // TODO eigentlich MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_publish_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    return MqttSnClientPublishQosMessageHandler(client, qos, msg_id, publish_msg, sizeof(publish_msg));
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishByRegistrationIndex(MqttSnClient *client, uint16_t registration_index, uint8_t retain,
                                                                  int8_t qos, uint8_t *data, uint16_t data_len) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (client->registrations_len <= registration_index) {
#ifdef WITH_LOGGING
        log_mqtt_sn_client_registrations_index_empty(&client->logger, client->registrations_len, MQTT_SN_CLIENT_MAX_REGISTRATIONS);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientPublishTopicId(client, client->registrations[registration_index].topic_id, retain, qos, data, data_len);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishQosMessageHandler(MqttSnClient *client, int8_t qos, uint16_t msg_id, uint8_t *publish_msg,
                                                                uint16_t publish_msg_length) {
    if (qos == 0 || qos == -1) {
        return send_to_gateway_with_retry(client, -1, publish_msg, publish_msg_length);
        /*
        int32_t send_rc = send_to_gateway_without_retry(client, publish_msg, publish_msg_length);
        if (send_rc < 0 || send_rc != publish_msg_length) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, publish_msg_length);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
         */
    } else if (qos == 1) {
        int32_t await_fd;
        while ((await_fd = acquire_await_fd(client, PUBACK, msg_id)) < 0) {
            if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }
        }

        MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
        if (await_context == NULL) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_await_context_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, publish_msg, publish_msg_length);
        free_await_fd(client, await_fd);
        return rc;
        /*
        int32_t send_rc = send_to_gateway_without_retry(client, publish_msg, publish_msg_length);
        if (send_rc < 0 || send_rc != publish_msg_length) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, publish_msg_length);
#endif
            free_await_fd(client, await_fd);
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
        if (await_rc != MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
            free_await_fd(client, await_fd);
            if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
                return MQTT_SN_CLIENT_RETURN_TIMEOUT;
            }
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        switch (await_context->return_code) {
        case RETURN_CODE_ACCEPTED:
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        case RETURN_CODE_REJECTED_CONGESTION:
            return MQTT_SN_CLIENT_RETURN_CONGESTION;
        case RETURN_CODE_REJCETED_NOT_SUPPORTED:
            return MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED;
        default:
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
        */
    } else if (qos == 2) {
        {
            int32_t await_fd;
            while ((await_fd = acquire_await_fd(client, PUBREC, msg_id)) < 0) {
                if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
                    log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                    return MQTT_SN_CLIENT_RETURN_ERROR;
                }
            }

            MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, publish_msg, publish_msg_length);
            free_await_fd(client, await_fd);
            if (rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                return rc;
            }
            /*
            uint64_t send_time = 0;
            if (get_timestamp_s(&send_time) < 0) {
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }

            int32_t send_rc = send_to_gateway_without_retry(client, publish_msg, publish_msg_length);
            if (send_rc < 0 || send_rc != publish_msg_length) {
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, publish_msg_length);
#endif
                free_await_fd(client, await_fd);
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }

            MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
            MQTT_SN_CLIENT_RETURN_CODE          rc       = await_status_to_client_return_code(await_rc, &client->logger);

            free_await_fd(client, await_fd);
            if (rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                return rc;
            }
             */
        }
        {
            int32_t await_fd;
            while ((await_fd = acquire_await_fd(client, PUBCOMP, msg_id)) < 0) {
                if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
                    log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                    return MQTT_SN_CLIENT_RETURN_ERROR;
                }
            }
            uint8_t pubrec_msg[MQTT_SN_MESSAGE_PUBREL_LENGTH];
            int32_t gen_rc = generate_pubrel(pubrec_msg, sizeof(pubrec_msg), msg_id);
            if (gen_rc < 0) {
#ifdef WITH_LOGGING
                log_pubrel_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }

            MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, pubrec_msg, gen_rc);
            free_await_fd(client, await_fd);
            if (rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                return rc;
            }
            /*
            int32_t send_rc = send_to_gateway_without_retry(client, pubrec_msg, gen_rc);
            if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, publish_msg_length);
#endif
                free_await_fd(client, await_fd);
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }

            MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
            free_await_fd(client, await_fd);
            MQTT_SN_CLIENT_RETURN_CODE rc = await_status_to_client_return_code(await_rc, &client->logger);
            */
            return rc;
        }
    }
    return MQTT_SN_CLIENT_RETURN_ERROR;
}
MQTT_SN_CLIENT_RETURN_CODE await_status_to_client_return_code(MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status, const MqttSnLogger *logger) {
    switch (status) {
    case MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS:
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    case MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT:
        return MQTT_SN_CLIENT_RETURN_TIMEOUT;
    case MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR:
        return MQTT_SN_CLIENT_RETURN_NETWORK_ERROR;
    case MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT:
        return MQTT_SN_CLIENT_RETURN_TIMEOUT;
    default:
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_await_fd_conversion_error(logger, __FILE__, __func__, __LINE__, status);
#endif
        break;
    }
    return MQTT_SN_CLIENT_RETURN_ERROR;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_client_asleep_connection(MqttSnClient *client) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP) {
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    }

    uint64_t current_time = 0;
    if (get_timestamp_s(&current_time) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    uint64_t elapsed_time = current_time - get_last_timeout_reset(client);

    if (client->sleep_duration <= MQTT_SN_CLIENT_PING_REQ_ADVANCE_TIME_S) {
        // if the connect_duration is lower than the ping request advance time than we use the connect duration
        if (elapsed_time < client->sleep_duration) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    } else {
        // use
        if (elapsed_time < (uint64_t)client->sleep_duration + client->connect_timeout_offset) {
            return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
        }
    }
#ifdef WITH_LOGGING
    log_mqtt_sn_client_connection_maintain_wakeup(&client->logger, &client->mqtt_sn_gateway_address);
#endif
    MQTT_SN_CLIENT_RETURN_CODE rc = MqttSnClientWakeup(client);
    if (rc == MQTT_SN_CLIENT_RETURN_ERROR) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    if (rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
        return MQTT_SN_CLIENT_LOOP_RETURN_TIMEOUT;
    }
    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE handle_client_awake_connection(MqttSnClient *client) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE) {
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    }
    // means we await a ping request
    uint64_t current_time = 0;
    if (get_timestamp_s(&current_time) < 0) {
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }
    uint64_t elapsed_time = current_time - get_last_timeout_reset(client);

#ifdef WITH_PAHO_WAKEUP_WORKAROUND
    uint64_t paho_elapsed_time = current_time - client->ping_req_send_time_s;
    if (paho_elapsed_time > (uint64_t)client->retry_timeout_ms / (uint64_t)1000) {
        // pingresp timeout - first check if any message received
        if (client->awake_any_msg_received) {
            // any message received - we take the send time of the ping request as ping response received time
            client->last_ping_resp_received_s = client->ping_req_send_time_s;
            if (client->ping_req_await_msg.msg_type == PINGRESP) {
                client->ping_req_await_msg.msg_type = ANY_MESSAGE_TYPE;
            }
        }
    }
#endif

    if (client->ping_req_await_msg.msg_type == PINGRESP) {
        // ping request already in flight

        // check if ping request timed out
        if (elapsed_time > get_tolerance_timeout(client->sleep_duration, client->low_timeout_threshold_s,
                                                 client->low_threshold_tolerance_percentage, client->high_threshold_tolerance_percentage)) {
            client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST;
#ifdef WITH_LOGGING
            log_mqtt_sn_client_status_gateway_lost(&client->logger, &client->mqtt_sn_gateway_address);
#endif
        }
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    } else {
        // if we manually call wakeup we cannot simply go back to asleep state
        if (is_await_msg(client, PINGRESP, 0) < 0) {
            client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP;
#ifdef WITH_LOGGING
            log_mqtt_sn_client_status_asleep(&client->logger);
#endif
        }
    }

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectClientCredentialInit(MqttSnClient *client, device_address *mqtt_sn_gateway_address,
                                                                         uint8_t clean_session, const char *client_id,
                                                                         uint16_t connect_duration) {
    if (client->status < MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    client->mqtt_sn_gateway_address = (*mqtt_sn_gateway_address);

    int32_t client_id_len;
    if ((client_id_len = MqttSnClientSafeStrlen(client_id, MQTT_SN_MAX_CLIENT_ID_LENGTH)) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (client_id_len == 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

#ifdef WITH_MQTT_SN_CLIENT_BUFFER
    strncpy(client->client_buffer.client_id, client_id, client_id_len);
    client->client_id = client->client_buffer.client_id;
#else
    client->client_id = client_id;
#endif
    if (clean_session) {
        client->registrations_len = 0;
    }

    client->ping_req_await_msg.msg_type = ANY_MESSAGE_TYPE;
    client->ping_req_await_msg.msg_id   = 0;
    client->connect_duration            = connect_duration;
    client->clean_session               = clean_session;

    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED;

    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectWillCredentialInit(MqttSnClient *client, const char *will_topic, int8_t will_qos,
                                                                       uint8_t will_retain, const uint8_t *will_msg,
                                                                       uint16_t will_msg_length) {
    int32_t will_topic_length;
#ifdef WITH_MQTT_SN_CLIENT_BUFFER
    if ((will_topic_length = MqttSnClientSafeStrlen(will_topic, MQTT_SN_CLIENT_MAX_WILL_TOPIC_LENGTH)) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
#else
    if ((will_topic_length = MqttSnClientSafeStrlen(will_topic, UINT16_MAX)) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
#endif

    if (will_topic_length <= 0 || get_will_topic_msg_len(will_topic_length) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (get_will_msg_msg_len(will_msg_length) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
#ifdef WITH_MQTT_SN_CLIENT_BUFFER
    if (will_msg_length >= MQTT_SN_CLIENT_MAX_WILL_MSG_LENGTH) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    strncpy(client->client_buffer.will_topic, will_topic, will_topic_length);
    client->will_topic        = client->client_buffer.will_topic;
    client->will_topic_length = will_topic_length;

    memcpy(client->client_buffer.will_msg, will_msg, will_msg_length);
    client->client_buffer.will_msg_len = will_topic_length;
    client->will_msg                   = client->client_buffer.will_msg;
#else
    client->client_id  = client_id;
    client->will_topic = will_topic;
    client->will_msg   = will_msg;
#endif
    client->will_msg_len                    = client->will_msg_len;
    client->client_buffer.will_topic_length = will_topic_length;
    client->will_msg_len                    = will_msg_length;
    client->will_qos                        = will_qos;
    client->will_retain                     = will_retain;
    return MQTT_SN_CLIENT_RETURN_NETWORK_ERROR;
}
MQTT_SN_CLIENT_LOOP_RETURN_CODE parse_and_handle_register(MqttSnClient *client, MqttSnMessageData *msg) {
    if (client->registrations_len >= MQTT_SN_CLIENT_MAX_REGISTRATIONS) {
#ifdef WITH_LOGGING
        log_mqtt_sn_client_registrations_full(&client->logger, client->registrations_len, MQTT_SN_CLIENT_MAX_REGISTRATIONS);
#endif
        // there is not appropriate MQTT-SN ReturnCode for this case - we ignore the message
        return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
    }

    uint16_t  msg_id;
    uint16_t *topic_id   = &client->registrations[client->registrations_len].topic_id;
    char *    topic_name = client->registrations[client->registrations_len].topic_name;
    uint16_t  topic_name_length;
    uint16_t  max_topic_name_length = MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH;

    if (parse_register_byte(topic_id, &msg_id, topic_name, &topic_name_length, max_topic_name_length, msg->data, msg->data_length) < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_mqtt_sn_client_mqtt_sn_message_malformed(&client->logger, msg);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR;
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_REGACK_LENGTH] = { 0 };
    int32_t gen_rc = generate_regack(msg_data, sizeof(msg_data), (*topic_id), msg_id, RETURN_CODE_ACCEPTED);
    if (gen_rc < 0) {
#ifdef WITH_DEBUG_LOGGING
        log_regack_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_LOOP_RETURN_ERROR;
    }

    client->registrations_len += 1;

    return MQTT_SN_CLIENT_LOOP_RETURN_SUCCESS;
}

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeMessageHandler(MqttSnClient *client, uint16_t msg_id, uint8_t *subscribe_msg,
                                                               uint16_t subscribe_msg_length) {
    int32_t await_fd;
    while ((await_fd = acquire_await_fd(client, SUBACK, msg_id)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
    if (await_context == NULL) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_await_context_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, subscribe_msg, subscribe_msg_length);
    if (rc == MQTT_SN_CLIENT_RETURN_SUCCESS) {
        switch (await_context->return_code) {
        case RETURN_CODE_ACCEPTED:
            client->registrations[client->registrations_len].topic_id = await_context->topic_id;
            client->registrations_len += 1;
            rc = MQTT_SN_CLIENT_RETURN_SUCCESS;
            break;
        case RETURN_CODE_REJECTED_CONGESTION:
            rc = MQTT_SN_CLIENT_RETURN_CONGESTION;
            break;
        case RETURN_CODE_REJCETED_NOT_SUPPORTED:
            rc = MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED;
            break;
        default:
            rc = MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }
    free_await_fd(client, await_fd);
    /*
    int32_t send_rc = send_to_gateway_without_retry(client, subscribe_msg, subscribe_msg_length);
    if (send_rc < 0 || send_rc != subscribe_msg_length) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, subscribe_msg_length);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);

    MQTT_SN_CLIENT_RETURN_CODE rc;
    if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
        switch (await_context->return_code) {
        case RETURN_CODE_ACCEPTED:
            client->registrations[client->registrations_len].topic_id = await_context->topic_id;
            client->registrations_len += 1;
            rc = MQTT_SN_CLIENT_RETURN_SUCCESS;
            break;
        case RETURN_CODE_REJECTED_CONGESTION:
            rc = MQTT_SN_CLIENT_RETURN_CONGESTION;
            break;
        case RETURN_CODE_REJCETED_NOT_SUPPORTED:
            rc = MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED;
            break;
        default:
            rc = MQTT_SN_CLIENT_RETURN_ERROR;
        }
    } else if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
        rc = MQTT_SN_CLIENT_RETURN_TIMEOUT;
    } else {
        rc = MQTT_SN_CLIENT_RETURN_ERROR;
    }
    free_await_fd(client, await_fd);
    */
    return rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribePredefined(MqttSnClient *client, int8_t qos, uint16_t topic_id) {
    if (client->status <= MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint16_t msg_id = client->msg_counter++;
    uint8_t  subscribe_msg[MQTT_SN_MESSAGE_SUBSCRIBE_W_PREDEFINED_TOPICID_LENGTH];
    int32_t  gen_rc = generate_subscribe(subscribe_msg, sizeof(subscribe_msg), 0, qos, MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID,
                                        msg_id, topic_id, NULL, 0);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_subscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    return MqttSnClientSubscribeMessageHandler(client, msg_id, subscribe_msg, gen_rc);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeShort(MqttSnClient *client, int8_t qos,
                                                      uint16_t topic_id) {  // TODO: this crashes the paho gateway
    if (client->status <= MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint16_t msg_id = client->msg_counter++;
    uint8_t  subscribe_msg[MQTT_SN_MESSAGE_SUBSCRIBE_W_TOPICID_LENGTH];
    int32_t  gen_rc = generate_subscribe(subscribe_msg, sizeof(subscribe_msg), 0, qos, MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME, msg_id,
                                        topic_id, NULL, 0);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_subscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientSubscribeMessageHandler(client, msg_id, subscribe_msg, gen_rc);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeTopicName(MqttSnClient *client, int8_t qos, const char *topic_name,
                                                          uint16_t topic_name_length) {
    if (client->status <= MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    if (client->registrations_len >= MQTT_SN_CLIENT_MAX_REGISTRATIONS) {
#ifdef WITH_LOGGING
        log_mqtt_sn_client_registrations_full(&client->logger, client->registrations_len, MQTT_SN_CLIENT_MAX_REGISTRATIONS);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (topic_name_length > MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH) {
#ifdef WITH_LOGGING
        log_mqtt_sn_client_registrations_topic_name_too_long(&client->logger, topic_name_length,
                                                             MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    strncpy(client->registrations[client->registrations_len].topic_name, topic_name, topic_name_length);

    int32_t subscribe_msg_length = get_subscribe_msg_len(topic_name_length);
    if (subscribe_msg_length < 0) {
#ifdef WITH_LOGGING
        log_subscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    uint16_t msg_id = client->msg_counter++;
    uint8_t  subscribe_msg[subscribe_msg_length];
    int32_t  gen_rc = generate_subscribe(subscribe_msg, sizeof(subscribe_msg), 0, qos, MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME, msg_id, 0,
                                        topic_name, topic_name_length);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_subscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientSubscribeMessageHandler(client, msg_id, subscribe_msg, gen_rc);
}

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSleep(MqttSnClient *client, uint16_t sleep_duration) {
    if (client->status <= MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    uint8_t disconnect_msg[MQTT_SN_MESSAGE_DISCONNECT_W_DURATION_LENGTH];
    int32_t gen_rc = generate_disconnect_duration(disconnect_msg, sizeof(disconnect_msg), sleep_duration);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_disconnect_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t await_fd;
    if ((await_fd = acquire_await_fd(client, DISCONNECT, 0)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    int32_t send_rc = send_to_gateway_without_retry(client, disconnect_msg, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_RETURN_NETWORK_ERROR;
    }

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
    free_await_fd(client, await_fd);

    MQTT_SN_CLIENT_RETURN_CODE rc = await_status_to_client_return_code(await_rc, &client->logger);
    if (rc == MQTT_SN_CLIENT_RETURN_SUCCESS) {
        client->sleep_duration = sleep_duration;
        client->status         = MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP;
    }
    return rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientWakeup(MqttSnClient *client) {
    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE) {
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_PINGREQ_W_CLIENTID_MAX_LENGTH] = { 0 };
    int32_t gen_rc = generate_ping_req_w_client_id(msg_data, sizeof(msg_data), client->client_id, strlen(client->client_id));
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_pingreq_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t await_fd;
    if ((await_fd = acquire_await_fd(client, PINGRESP, 0)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    if (get_timestamp_s(&client->awake_ping_request_send_time_s) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
#ifdef WITH_PAHO_WAKEUP_WORKAROUND
    uint64_t mqtt_sn_client_wakeup_time_s;
    if (get_timestamp_s(&mqtt_sn_client_wakeup_time_s) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    client->awake_any_msg_received = false;
#endif

    /*
    int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE;

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
    MQTT_SN_CLIENT_RETURN_CODE          rc       = await_status_to_client_return_code(await_rc, &client->logger);
    */
    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP) {
        client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE;
    }
    MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
    free_await_fd(client, await_fd);
#ifdef WITH_PAHO_WAKEUP_WORKAROUND
    if (rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
        if (client->awake_any_msg_received) {
            // TODO mention in standard
            // TODO standard nochmal durchlesen dazu (unteres drittel, seite 26)
            /*
             * t restarts timerTretrywhen itreceives a message other than a PINGRESP, and stops it when it receives a PINGRESP. The PINGREQ messageis retransmitted and timerTretryrestarted when timerTretrytimes out.
             */
            // no real time - paho does not send ping response but we received
            // two possible ways to fix this:
            // - send repeatedly PingRequests (and receive buffered Publishes) until we receive a PingResponse. But this blocks very long.
            // - assume the PingResponse is received because we received buffered Publishes.
            // But we detect the broken gateway with the next PingRequest.
            // We use the second approach (assume we received a PingResponse)
            if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_GATEWAY_LOST) {
                client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE;
            }
            client->last_ping_resp_received_s = mqtt_sn_client_wakeup_time_s;
            rc                                = MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
#endif
    if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE) {
        client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_ASLEEP;
    }
    return rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPing(MqttSnClient *client) {
    int32_t await_fd;
    if ((await_fd = acquire_await_fd(client, PINGRESP, 0)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_PINGREQ_LENGTH] = { 0 };

    int32_t gen_rc = generate_ping_req(msg_data, sizeof(msg_data));
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_pingreq_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
    free_await_fd(client, await_fd);
    return rc;
    /*
    uint64_t connect_send_time = 0;
    if (get_timestamp_s(&connect_send_time) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
    if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
    free_await_fd(client, await_fd);
    return await_status_to_client_return_code(await_rc, &client->logger);
    */
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientUnsubscribeMessageHandler(MqttSnClient *client, uint16_t msg_id, uint8_t *unsubscribe_msg,
                                                                 uint16_t unsubscribe_msg_length) {
    int32_t await_fd;
    while ((await_fd = acquire_await_fd(client, UNSUBACK, msg_id)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, unsubscribe_msg, unsubscribe_msg_length);
    free_await_fd(client, await_fd);
    return rc;
    /*
    int32_t send_rc = send_to_gateway_without_retry(client, unsubscribe_msg, unsubscribe_msg_length);
    if (send_rc < 0 || send_rc != unsubscribe_msg_length) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, unsubscribe_msg_length);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
    free_await_fd(client, await_fd);
    return await_status_to_client_return_code(await_rc, &client->logger);
     */
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientUnsubscribePredefined(MqttSnClient *client, uint16_t topic_id) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint16_t msg_id = client->msg_counter++;
    uint8_t  unsubscribe_msg[MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICID_LENGTH];
    int32_t  gen_rc = generate_unsubscribe(unsubscribe_msg, sizeof(unsubscribe_msg), 0, MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID,
                                          msg_id, topic_id, NULL, 0);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_unsubscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientUnsubscribeMessageHandler(client, msg_id, unsubscribe_msg, gen_rc);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientUnsubscribeShort(MqttSnClient *client, uint16_t topic_id) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint16_t msg_id = client->msg_counter++;
    uint8_t  unsubscribe_msg[MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICID_LENGTH];
    int32_t  gen_rc = generate_unsubscribe(unsubscribe_msg, sizeof(unsubscribe_msg), 0, MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME, msg_id,
                                          topic_id, NULL, 0);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_unsubscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientUnsubscribeMessageHandler(client, msg_id, unsubscribe_msg, gen_rc);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientUnsubscribeTopicName(MqttSnClient *client, const char *topic_name, uint16_t topic_name_length) {
    if (client->status != MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t unsubscribe_msg_length = get_unsubscribe_msg_len(topic_name_length);
    if (unsubscribe_msg_length < 0) {
#ifdef WITH_LOGGING
        log_unsubscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    uint16_t msg_id = client->msg_counter++;
    uint8_t  unsubscribe_msg[unsubscribe_msg_length];
    int32_t  gen_rc = generate_unsubscribe(unsubscribe_msg, sizeof(unsubscribe_msg), 0, MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME, msg_id, 0,
                                          topic_name, topic_name_length);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_unsubscribe_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    return MqttSnClientUnsubscribeMessageHandler(client, msg_id, unsubscribe_msg, gen_rc);
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientReconnect(MqttSnClient *client) {
    MQTT_SN_CLIENT_RETURN_CODE reconnect_rc =
      MqttSnClientDirectConnectWithoutWill(client, &client->mqtt_sn_gateway_address, false, client->client_id, client->connect_duration);
#ifdef WITH_LOGGING
    if (reconnect_rc == MQTT_SN_CLIENT_RETURN_SUCCESS) {
        log_mqtt_sn_client_reconnected_to(&client->logger, &client->mqtt_sn_gateway_address);
    }
#endif
    return reconnect_rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectWithoutWill(MqttSnClient *client, device_address *mqtt_sn_gateway_address,
                                                                uint8_t clean_session, const char *client_id, uint16_t connect_duration) {
    if (MqttSnClientDirectConnectClientCredentialInit(client, mqtt_sn_gateway_address, clean_session, client_id, connect_duration) !=
        MQTT_SN_CLIENT_RETURN_SUCCESS) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    uint8_t msg_data[MQTT_SN_MESSAGE_CONNECT_MAX_LENGTH] = { 0 };
    int32_t gen_rc = generate_connect(msg_data, sizeof(msg_data), MQTT_SN_FLAG_WILL_FALSE, clean_session, MQTT_SN_CLIENT_PROTOCOL_ID,
                                      client->connect_duration, client->client_id, strlen(client->client_id) + 1);
    if (gen_rc < 0) {
#ifdef WITH_LOGGING
        log_connect_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    int32_t await_fd;
    while ((await_fd = acquire_await_fd(client, CONNACK, 0)) < 0) {
        if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
    }

    MqttSnClientAwaitContext *await_context = get_await_context(client, await_fd);
    if (await_context == NULL) {
#ifdef WITH_LOGGING
        log_fatal_mqtt_sn_client_await_context_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
    if (rc == MQTT_SN_CLIENT_RETURN_SUCCESS && await_context->return_code == RETURN_CODE_ACCEPTED) {
        if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED) {
#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
            client->auto_reconnect_tried      = 0;
            client->auto_reconnect_congestion = 0;
#endif
            client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED;
        }
#ifdef WITH_LOGGING
        log_mqtt_sn_client_connected_to(&client->logger, &client->mqtt_sn_gateway_address);
#endif
    }
#ifdef WITH_LOGGING
    else {
        log_mqtt_sn_client_connect_failed(&client->logger);
    }
#endif
    free_await_fd(client, await_fd);

    return rc;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectWithWill(MqttSnClient *client, device_address *mqtt_sn_gateway_address,
                                                             uint8_t clean_session, const char *client_id, uint16_t connect_duration,
                                                             const char *will_topic, int8_t will_qos, uint8_t will_retain,
                                                             const uint8_t *will_msg, uint16_t will_msg_length) {
    bool will = false;
    if (will_topic != NULL && will_msg != NULL) {
        will = true;
    }
    if (!will) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    if (MqttSnClientDirectConnectClientCredentialInit(client, mqtt_sn_gateway_address, clean_session, client_id, connect_duration) !=
        MQTT_SN_CLIENT_RETURN_SUCCESS) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (MqttSnClientDirectConnectWillCredentialInit(client, will_topic, will_qos, will_retain, will_msg, will_msg_length) !=
        MQTT_SN_CLIENT_RETURN_SUCCESS) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }

    {
        uint8_t msg_data[MQTT_SN_MESSAGE_CONNECT_MAX_LENGTH] = { 0 };
        int32_t gen_rc;
        if ((gen_rc = generate_connect(msg_data, sizeof(msg_data), MQTT_SN_FLAG_WILL_TRUE, clean_session, MQTT_SN_CLIENT_PROTOCOL_ID,
                                       client->connect_duration, client->client_id, strlen(client->client_id))) < 0) {
#ifdef WITH_LOGGING
            log_connect_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        int32_t await_fd;
        while ((await_fd = acquire_await_fd(client, WILLTOPICREQ, 0)) < 0) {
            if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }
        }

        MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
        free_await_fd(client, await_fd);
        if (rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            return rc;
        }
        /*
        int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
        if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
        if ((rc = await_status_to_client_return_code(await_rc, &client->logger)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            free_await_fd(client, await_fd);
            return rc;
        }
        free_await_fd(client, await_fd);
        */
    }
    {
        int32_t msg_data_length = get_will_topic_msg_len(strlen(will_topic) + 1);
        if (msg_data_length < 0) {
#ifdef WITH_LOGGING
            log_will_topic_msg_length_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
        uint8_t msg_data[msg_data_length];

        int32_t gen_rc;
        if ((gen_rc = generate_will_topic(msg_data, sizeof(msg_data), will_qos, will_retain, will_topic, strlen(will_topic))) < 0) {
#ifdef WITH_LOGGING
            log_will_topic_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        int32_t await_fd;
        while ((await_fd = acquire_await_fd(client, WILLMSGREQ, 0)) < 0) {
            if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }
        }

        MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
        free_await_fd(client, await_fd);
        if (rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            return rc;
        }
        /*
        int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
        if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
        if ((rc = await_status_to_client_return_code(await_rc, &client->logger)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            free_await_fd(client, await_fd);
            return rc;
        }
        free_await_fd(client, await_fd);
         */
    }
    {
        int32_t msg_data_length = get_will_msg_msg_len(will_msg_length);
        if (msg_data_length < 0) {
#ifdef WITH_LOGGING
            log_will_msg_msg_length_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
        uint8_t msg_data[msg_data_length];

        int32_t gen_rc = generate_will_msg(msg_data, sizeof(msg_data), will_msg, will_msg_length);
        if (gen_rc < 0) {
#ifdef WITH_LOGGING
            log_will_msg_gen_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        int32_t await_fd;
        while ((await_fd = acquire_await_fd(client, CONNACK, 0)) < 0) {
            if (MqttSnClientLoop(client) < 0) {
#ifdef WITH_LOGGING
                log_fatal_mqtt_sn_client_loop_error(&client->logger, __FILE__, __func__, __LINE__);
#endif
                return MQTT_SN_CLIENT_RETURN_ERROR;
            }
        }

        MQTT_SN_CLIENT_RETURN_CODE rc = send_to_gateway_with_retry(client, await_fd, msg_data, gen_rc);
        free_await_fd(client, await_fd);
        if (rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            return rc;
        }
        /*
        int32_t send_rc = send_to_gateway_without_retry(client, msg_data, gen_rc);
        if (send_rc < 0 || send_rc != gen_rc) {
#ifdef WITH_LOGGING
            log_fatal_mqtt_sn_client_send_error(&client->logger, __FILE__, __func__, __LINE__, send_rc, gen_rc);
#endif
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }

        MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc = get_await_status_timeout_ms_easy(client, await_fd, client->retry_timeout_ms);
        if ((rc = await_status_to_client_return_code(await_rc, &client->logger)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            free_await_fd(client, await_fd);
            return rc;
        }
        free_await_fd(client, await_fd);
         */
    }
#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
    client->auto_reconnect_tried      = 0;
    client->auto_reconnect_congestion = 0;
#endif
    client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED;
#ifdef WITH_LOGGING
    log_mqtt_sn_client_connected_to(&client->logger, &client->mqtt_sn_gateway_address);
#endif
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAutoConnect(MqttSnClient *client, MQTT_SN_CLIENT_FIND_GW_PATTERN find_gateway_pattern,
                                                   int32_t find_gateway_wait_timeout_ms, device_address *mqtt_sn_gateway_address,
                                                   uint8_t clean_session, const char *client_id, uint16_t connect_duration,
                                                   const char *will_topic, int8_t will_qos, uint8_t will_retain, const uint8_t *will_msg,
                                                   uint16_t will_msg_length) {
    if (client->status < MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    if (client->status >= MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED) {
        client->status = MQTT_SN_CLIENT_CONNECTION_STATUS_DISCONNECTED;
    }
    client->find_gw_pattern              = find_gateway_pattern;
    client->find_gateway_wait_timeout_ms = find_gateway_wait_timeout_ms;
    if (client->find_gw_pattern == MQTT_SN_CLIENT_FIND_GW_PATTERN_DIRECT) {
        if (mqtt_sn_gateway_address == NULL) {
            return MQTT_SN_CLIENT_RETURN_ERROR;
        }
        return MqttSnClientDirectConnect(client, &client->mqtt_sn_gateway_address, clean_session, client_id, connect_duration, will_topic,
                                         will_qos, will_retain, will_msg, will_msg_length);
    }
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    uint64_t current_time_s;
    if (get_timestamp_s(&current_time_s) < 0) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    MqttSnDiscoveredGatewayListUpdate(&client->discovered_gw_list, NULL, 0, NULL, NULL, current_time_s);
    if (client->discovered_gw_list.current_len > 0) {
        MqttSnDiscoveredGateway *discovered_gateway = &client->discovered_gw_list.gateways[0];
        // get the discovery gateway with the least signal_strength and then smaller received Advertise or SearchGw (last_message_s)
        for (uint8_t i = 0; i < client->discovered_gw_list.current_len; i++) {
            if (client->discovered_gw_list.gateways[i].signal_strength == discovered_gateway->signal_strength &&
                client->discovered_gw_list.gateways[i].last_message_s > discovered_gateway->last_message_s) {
                discovered_gateway = &client->discovered_gw_list.gateways[i];
            } else if (client->discovered_gw_list.gateways[i].signal_strength < discovered_gateway->signal_strength) {
                discovered_gateway = &client->discovered_gw_list.gateways[i];
            }
        }
        client->default_signal_strength = discovered_gateway->signal_strength;
        return MqttSnClientDirectConnect(client, &discovered_gateway->network_address, clean_session, client_id, connect_duration,
                                         will_topic, will_qos, will_retain, will_msg, will_msg_length);
    }
#endif

    if (client->find_gw_pattern == MQTT_SN_CLIENT_FIND_GW_PATTERN_SEARCHGW) {
        MQTT_SN_CLIENT_RETURN_CODE gw_info_rc;
        MqttSnClientReceivedGwInfo received_gw_info = { { 0 } };
        if ((gw_info_rc = MqttSnClientSearchGw(client, find_gateway_wait_timeout_ms, client->default_signal_strength, &received_gw_info)) !=
            MQTT_SN_CLIENT_RETURN_SUCCESS) {
            return gw_info_rc;
        }
        device_address possible_mqtt_sn_gateway_address;
        if (received_gw_info.received_gw_info.gwAddLen > 0) {
            possible_mqtt_sn_gateway_address = received_gw_info.from;
            int32_t addr_diff_len            = sizeof(device_address) - received_gw_info.received_gw_info.gwAddLen;
            for (uint16_t i = 0; i < addr_diff_len; i++) {
                possible_mqtt_sn_gateway_address.bytes[i] = 0;
            }
            for (uint16_t i = addr_diff_len; i < sizeof(device_address); i++) {
                possible_mqtt_sn_gateway_address.bytes[i] = received_gw_info.received_gw_info.gwAdd.bytes[i - addr_diff_len];
            }
        } else {
            possible_mqtt_sn_gateway_address = received_gw_info.from;
        }
        client->default_signal_strength = received_gw_info.signal_strength;
        return MqttSnClientDirectConnect(client, &possible_mqtt_sn_gateway_address, clean_session, client_id, connect_duration, will_topic,
                                         will_qos, will_retain, will_msg, will_msg_length);
    } else if (client->find_gw_pattern == MQTT_SN_CLIENT_FIND_GW_PATTERN_ADVERTISE) {
        MqttSnClientReceivedAdvertise received_advertise = { { 0 } };
        MQTT_SN_CLIENT_RETURN_CODE    advertise_rc;
        if ((advertise_rc = MqttSnClientAwaitAdvertise(client, find_gateway_wait_timeout_ms / (int32_t)1000, &received_advertise)) !=
            MQTT_SN_CLIENT_RETURN_SUCCESS) {
            return advertise_rc;
        }
        device_address possible_mqtt_sn_gateway_address = received_advertise.from;
        client->default_signal_strength                 = received_advertise.signal_strength;
        return MqttSnClientDirectConnect(client, &possible_mqtt_sn_gateway_address, clean_session, client_id, connect_duration, will_topic,
                                         will_qos, will_retain, will_msg, will_msg_length);
    }
#ifdef WITH_LOGGING
    log_mqtt_sn_client_connect_failed(&client->logger);
#endif
    return MQTT_SN_CLIENT_RETURN_ERROR;
}
#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
void MqttSnClientEnableAutoReconnect(MqttSnClient *client) {
    client->auto_reconnect_enabled = 1;
}
void MqttSnClientDisableAutoReconnect(MqttSnClient *client) {
    client->auto_reconnect_enabled = 0;
}
uint8_t MqttSnClientIsAutoReconnectEnabled(MqttSnClient *client) {
    return client->auto_reconnect_enabled;
}

#endif
#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddWhitelistGwId(MqttSnClient *client, uint8_t gw_id) {
    if (client->gw_id_whitelist_len + 1 >= client->gw_id_whitelist_max_len) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    for (int32_t i = 0; i < client->gw_id_whitelist_len; i++) {
        if (client->gw_id_whitelist[i] == gw_id) {
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    client->gw_id_whitelist[client->gw_id_whitelist_len] = gw_id;
    client->gw_id_whitelist_len += 1;
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveWhitelistGwId(MqttSnClient *client, uint8_t gw_id) {
    if (client->gw_id_whitelist_len == 0) {
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    for (int32_t i = 0; i < client->gw_id_whitelist_len; i++) {
        if (client->gw_id_whitelist[i] == gw_id) {
            client->gw_id_whitelist[i] = client->gw_id_whitelist[client->gw_id_whitelist_len - 1];

            client->gw_id_whitelist[client->gw_id_whitelist_len - 1] = 0;
            client->gw_id_whitelist_len -= 1;
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
void MqttSnClientClearWhitelistGwId(MqttSnClient *client) {
    client->gw_id_whitelist_max_len = MQTT_SN_CLIENT_GW_ID_WHITELIST_LENGTH;
    client->gw_id_whitelist_len     = 0;
    for (int32_t i = 0; i < client->gw_id_whitelist_max_len; i++) {
        client->gw_id_whitelist[i] = 0;
    }
}
uint8_t MqttSnClientWhitelistContainsGwId(MqttSnClient *client, uint8_t gw_id) {
    for (int32_t i = 0; i < client->gw_id_whitelist_len; i++) {
        if (client->gw_id_whitelist[i] == gw_id) {
            return 1;
        }
    }
    return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddWhitelistNetworkAddress(MqttSnClient *client, device_address address) {
    if (client->gw_addr_whitelist_len + 1 >= client->gw_addr_whitelist_max_len) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    for (int32_t i = 0; i < client->gw_addr_whitelist_len; i++) {
        if (memcmp(&client->gw_addr_whitelist[i], &address, sizeof(device_address)) == 0) {
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    client->gw_addr_whitelist[client->gw_addr_whitelist_len] = address;
    client->gw_addr_whitelist_len += 1;
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveWhitelistNetworkAddress(MqttSnClient *client, device_address address) {
    if (client->gw_addr_whitelist_len == 0) {
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    for (int32_t i = 0; i < client->gw_addr_whitelist_len; i++) {
        if (memcmp(&client->gw_addr_whitelist[i], &address, sizeof(device_address)) == 0) {
            client->gw_addr_whitelist[i] = client->gw_addr_whitelist[client->gw_addr_whitelist_len - 1];
            memset(&client->gw_addr_whitelist[client->gw_addr_whitelist_len - 1], 0, sizeof(device_address));
            client->gw_addr_whitelist_len -= 1;
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
void MqttSnClientClearWhitelistNetworkAddress(MqttSnClient *client) {
    client->gw_addr_whitelist_max_len = MQTT_SN_CLIENT_GW_ADDRESS_BLACKLIST_LENGTH;
    client->gw_addr_whitelist_len     = 0;
    for (int32_t i = 0; i < client->gw_addr_whitelist_max_len; i++) {
        memset(&client->gw_addr_whitelist[i], 0, sizeof(device_address));
    }
}
uint8_t MqttSnClientWhitelistContainsNetworkAddress(MqttSnClient *client, device_address address) {
    for (int32_t i = 0; i < client->gw_addr_whitelist_len; i++) {
        if (memcmp(&client->gw_addr_whitelist[i], &address, sizeof(device_address)) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddBlacklistGwId(MqttSnClient *client, uint8_t gw_id) {
    if (client->gw_id_blacklist_len + 1 >= client->gw_id_blacklist_max_len) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    for (int32_t i = 0; i < client->gw_id_blacklist_len; i++) {
        if (client->gw_id_blacklist[i] == gw_id) {
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    client->gw_id_blacklist[client->gw_id_blacklist_len] = gw_id;
    client->gw_id_blacklist_len += 1;
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveBlacklistGwId(MqttSnClient *client, uint8_t gw_id) {
    if (client->gw_id_blacklist_len == 0) {
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    for (int32_t i = 0; i < client->gw_id_blacklist_len; i++) {
        if (client->gw_id_blacklist[i] == gw_id) {
            client->gw_id_blacklist[i] = client->gw_id_blacklist[client->gw_id_blacklist_len - 1];

            client->gw_id_blacklist[client->gw_id_blacklist_len - 1] = 0;
            client->gw_id_blacklist_len -= 1;
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
void MqttSnClientClearBlacklistGwId(MqttSnClient *client) {
    client->gw_id_blacklist_max_len = MQTT_SN_CLIENT_GW_ID_BLACKLIST_LENGTH;
    client->gw_id_blacklist_len     = 0;
    for (int32_t i = 0; i < client->gw_id_blacklist_max_len; i++) {
        client->gw_id_blacklist[i] = 0;
    }
}
uint8_t MqttSnClientBlacklistContainsGwId(MqttSnClient *client, uint8_t gw_id) {
    for (int32_t i = 0; i < client->gw_id_blacklist_len; ++i) {
        if (client->gw_id_blacklist[i] == gw_id) {
            return 1;
        }
    }
    return 0;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddBlacklistNetworkAddress(MqttSnClient *client, device_address address) {
    if (client->gw_addr_blacklist_len + 1 >= client->gw_addr_blacklist_max_len) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    for (int32_t i = 0; i < client->gw_addr_blacklist_len; i++) {
        if (memcmp(&client->gw_addr_blacklist[i], &address, sizeof(device_address)) == 0) {
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    client->gw_addr_blacklist[client->gw_addr_blacklist_len] = address;
    client->gw_addr_blacklist_len += 1;
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveBlacklistNetworkAddress(MqttSnClient *client, device_address address) {
    if (client->gw_addr_blacklist_len == 0) {
        return MQTT_SN_CLIENT_RETURN_SUCCESS;
    }
    for (int32_t i = 0; i < client->gw_addr_blacklist_len; ++i) {
        if (memcmp(&client->gw_addr_blacklist[i], &address, sizeof(device_address)) == 0) {
            client->gw_addr_blacklist[i] = client->gw_addr_blacklist[client->gw_addr_blacklist_len - 1];

            memset(&client->gw_addr_blacklist[client->gw_addr_blacklist_len - 1], 0, sizeof(device_address));
            client->gw_addr_blacklist_len -= 1;
            return MQTT_SN_CLIENT_RETURN_SUCCESS;
        }
    }
    return MQTT_SN_CLIENT_RETURN_CONNECTION_LOST;
}
void MqttSnClientClearBlacklistNetworkAddress(MqttSnClient *client) {
    client->gw_addr_blacklist_max_len = MQTT_SN_CLIENT_GW_ADDRESS_BLACKLIST_LENGTH;
    client->gw_addr_blacklist_len     = 0;
    for (int32_t i = 0; i < client->gw_addr_blacklist_max_len; i++) {
        memset(&client->gw_addr_blacklist[i], 0, sizeof(device_address));
    }
}
uint8_t MqttSnClientBlacklistContainsNetworkAddress(MqttSnClient *client, device_address address) {
    for (int32_t i = 0; i < client->gw_addr_blacklist_len; i++) {
        if (memcmp(&client->gw_addr_blacklist[i], &address, sizeof(device_address)) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif
void MqttSnClientSetTimeoutOffset(MqttSnClient *client, int32_t timeout_offset_s) {
    client->connect_timeout_offset = timeout_offset_s;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetRetryTimeout(MqttSnClient *client, int32_t retry_timeout_ms) {
    if (retry_timeout_ms < -1) {
        return MQTT_SN_CLIENT_RETURN_ERROR;
    }
    client->retry_timeout_ms = retry_timeout_ms;
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
void MqttSnClientSetRetryCount(MqttSnClient *client, uint16_t retry_count) {
    client->retry_count = retry_count;
}
void MqttSnClientSetCongestionDelay(MqttSnClient *client, uint64_t congestion_delay_s) {
    client->congestion_delay_s = congestion_delay_s;
}
void MqttSnClientSetDefaultSignalStrength(MqttSnClient *client, uint8_t default_signal_strength) {
    client->default_signal_strength = default_signal_strength;
}
void MqttSnClientEnableSearchGwReply(MqttSnClient *client) {
    client->searchgw_reply = 1;
}
void MqttSnClientDisableSearchGwReply(MqttSnClient *client) {
    client->searchgw_reply = 0;
}
void MqttSnClientSearchGwGwInfoReplyDelay(MqttSnClient *client, uint16_t gw_info_delay_ms) {
    client->gwinfo_delay_ms = gw_info_delay_ms;
}
uint8_t MqttSnClientIsSearchGwReplyEnabled(MqttSnClient *client) {
    return client->searchgw_reply;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetMqttSnGatewayAddress(MqttSnClient *client, device_address *mqtt_sn_gateway_address) {
    client->mqtt_sn_gateway_address = (*mqtt_sn_gateway_address);
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetMqttSnBroadcastAddress(MqttSnClient *client, device_address *mqtt_sn_broadcast_address) {
    client->mqtt_sn_broadcast_address = (*mqtt_sn_broadcast_address);
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetPublishCallback(MqttSnClient *client,
                                                          int32_t (*publish_cb)(const MqttSnClient *, const MqttSnPublish *)) {
    client->publish_cb = publish_cb;
    return MQTT_SN_CLIENT_RETURN_SUCCESS;
}
