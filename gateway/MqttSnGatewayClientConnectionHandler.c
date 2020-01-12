//
// Created by SomeDude on 30.06.2019.
//

#include "MqttSnGatewayClientConnectionHandler.h"
#include <parser/MqttSnPingReqMessage.h>
#include <gateway/database/db_handler.h>
#include <platform/MqttSnMessageData.h>
#include <parser/MqttSnConnectMessage.h>
#include <parser/MqttSnForwarderEncapsulationMessage.h>
#include <parser/MqttSnConnackMessage.h>

#include <platform/platform_compatibility.h>
#include <parser/MqttSnPingRespMessage.h>
#include <ringbuffer/logging/MqttSnFixedSizeRingBufferLogging.h>
#include <gateway/logging/MqttSnGatewayClientConnectionLogging.h>
#include <parser/MqttSnDisconnectMessage.h>
#include <parser/logging/MqttSnPingReqMessageLogging.h>
#include <parser/logging/MqttSnMessageParserLogging.h>

int32_t  check_active_client_connection_timeout(MqttSnGatewayClientConnectionHandler *handler, uint64_t current_time);
uint64_t get_connection_timeout_offset_duration(uint16_t duration, int32_t offset);
uint64_t get_client_tolerance_timeout(uint16_t timeout);
int32_t  send_client_connection_ping_req(MqttSnGatewayClientConnectionHandler *handler, MqttSnGatewayForwarder *forwarders);
uint64_t get_active_last_timeout_reset(uint64_t ping_resp_received, uint64_t ping_req_received);

int32_t init_client_connection_handler(MqttSnGatewayClientConnectionHandler *handler, MqttSnClientNetworkInterface *clientNetwork,
                                       MqttSnFixedSizeRingBuffer *clientNetworkSendBuffer, db_handler *db_handler_,
                                       const gateway_client_connection_config *cfg, MqttSnLogger *logger) {
    assert(handler != NULL);
    assert(clientNetwork != NULL);
    assert(clientNetworkSendBuffer != NULL);
    assert(db_handler_ != NULL);
    assert(cfg != NULL);

    memset(handler, 0, sizeof(*handler));

    handler->db_handler_                             = db_handler_;
    handler->clientNetworkSendBuffer                 = clientNetworkSendBuffer;
    handler->clientNetwork                           = clientNetwork;
    handler->logger                                  = logger;
    handler->client_connection_timeout_check_enabled = cfg->client_connection_timeout_check_enabled;
    handler->client_connection_timeout_check_period  = cfg->client_connection_timeout_check_period;
    handler->client_connection_timeout_offset        = cfg->client_connection_timeout_offset;

    if (get_timestamp_s(&handler->last_client_connection_timeout_check) < 0) {
        return -1;
    }
    return 0;
}

int32_t client_connection_handler_check_timeouts(MqttSnGatewayClientConnectionHandler *handler) {
    uint64_t current_time;
    if (get_timestamp_s(&current_time) < 0) {
#if WITH_LOGGING
        // TODO error get timestamp error
#endif
        return -1;
    }
    uint64_t elapsed_time = current_time - handler->last_client_connection_timeout_check;
    if (elapsed_time < handler->client_connection_timeout_check_period) {
        return 0;
    }

    uint64_t client_count = 0;
    if (db_get_client_count(handler->db_handler_, &client_count) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#ifdef WITH_LOGGING
        log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                  get_db_handler_result(handler->db_handler_));
#endif
        return -1;
    }
#if WITH_DEBUG_LOGGING
    log_client_connection_checking_client_timeout(handler->logger, client_count);
#endif
    for (uint64_t client_number = 0; client_number < client_count; client_number++) {
        if (db_start_client_transaction_by_client_count(handler->db_handler_, client_number) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#ifdef WITH_LOGGING
            log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                      get_db_handler_result(handler->db_handler_));
#endif
            return -1;
        }
        DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status;
        if (db_get_client_status(handler->db_handler_, &client_status) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#ifdef WITH_LOGGING
            log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                      get_db_handler_result(handler->db_handler_));
#endif
            return -1;
        }

        int32_t check_rc = 0;
        switch (client_status) {
        case DB_ENTRY_MQTT_SN_CLIENT_STATUS_EMPTY:
            break;
        case DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE:
            check_rc = check_active_client_connection_timeout(handler, current_time);
            break;
        case DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP:  // TODO implement me: check_asleep_client_timeout();
            break;
        case DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE:  // TODO implement me: check_awake_client_timeout();
            break;
        case DB_ENTRY_MQTT_SN_CLIENT_STATUS_DISCONNECTED:  // TODO implement me: check_disconnected_client_timeout();
            break;
        case DB_ENTRY_MQTT_SN_CLIENT_STATUS_LOST:  // TODO implement me: check_lost_client_timeout();
            break;
        default:
            break;
        }

        assert(db_is_client_transaction_started(handler->db_handler_));
        if (db_end_client_transaction(handler->db_handler_) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#ifdef WITH_LOGGING
            log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                      get_db_handler_result(handler->db_handler_));
#endif
            return -1;
        }
        if (check_rc < 0) {
#ifdef WITH_LOGGING
            log_fatal_client_connection_check_error(handler->logger, __FILE__, __func__, __LINE__);
#endif
            return -1;
        }
    }
    handler->last_client_connection_timeout_check = current_time;
    return 0;
}

int32_t check_active_client_connection_timeout(MqttSnGatewayClientConnectionHandler *handler, uint64_t current_time) {
    assert(db_is_client_transaction_started(handler->db_handler_));
    // TODO when we have a lot of client
    // TODO it may happen that we need to send so much PingReq that the SendBuffer is full
    // TODO some client may starve from the PingReq
    // TODO das ist ok weil dann kann die gateway halt nicht mehr clients unterhalten

    uint16_t               connect_duration;
    uint64_t               ping_req_received;
    uint64_t               ping_resp_received;
    MQTT_SN_MESSAGE_TYPE   ping_req_await_message_type;
    MqttSnGatewayForwarder forwarders = { 0 };
    MqttSnGatewayForwarderInit(&forwarders);
    // TODO könnte man auch mit einem call machen
    DB_HANDLER_CK_CLIENT_RC(db_get_client_connect_duration(handler->db_handler_, &connect_duration))
    DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_req_received(handler->db_handler_, &ping_req_received))
    DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_resp_received(handler->db_handler_, &ping_resp_received))
    DB_HANDLER_CK_CLIENT_RC(db_get_ping_req_await_msg_type(handler->db_handler_, &ping_req_await_message_type))
    DB_HANDLER_CK_CLIENT_RC(db_get_client_address(handler->db_handler_, &forwarders.wireless_node_id, forwarders.forwarder_addresses,
                                                  &forwarders.forwarder_len, forwarders.forwarder_max_len))

    uint64_t last_timeout_reset = get_active_last_timeout_reset(ping_resp_received, ping_req_received);
    uint64_t elapsed_time       = current_time - last_timeout_reset;

    uint64_t connection_timeout_offset_duration =
      get_connection_timeout_offset_duration(connect_duration, handler->client_connection_timeout_offset);

    if (elapsed_time >= connection_timeout_offset_duration && ping_req_await_message_type == ANY_MESSAGE_TYPE) {
        // timeout of the connect duration without tolerance
        // no ping request in flight for this client
        // send ping request from the gateway to check client status
        if (send_client_connection_ping_req(handler, &forwarders) < 0) {
            return -1;
        }
        return 0;
    }

    uint64_t tolerance_timeout = get_client_tolerance_timeout(connect_duration);
    if (elapsed_time > tolerance_timeout) {
        // tolerance timeout exceeded
        // client is timed out, set client lost
        DB_HANDLER_CK_CLIENT_RC(db_set_client_status(handler->db_handler_, DB_ENTRY_MQTT_SN_CLIENT_STATUS_LOST))
#if WITH_LOGGING
        char client_id[MQTT_SN_CLIENTID_MAX_LENGTH] = { 0 };
        DB_HANDLER_CK_CLIENT_RC(db_get_client_id(handler->db_handler_, client_id))
        log_client_connection_client_lost(handler->logger, &forwarders.wireless_node_id, client_id, connect_duration);
#endif
    }
    return 0;
}
uint64_t get_active_last_timeout_reset(uint64_t ping_resp_received, uint64_t ping_req_received) {
    return ping_resp_received > ping_req_received ? ping_resp_received : ping_req_received;
}
uint64_t get_connection_timeout_offset_duration(uint16_t duration, int32_t offset) {
    if (offset < 0 && duration <= offset) {
        return duration;
    }
    return (((int32_t)duration) + offset);
}
uint64_t get_client_tolerance_timeout(uint16_t duration) {
    uint64_t tolerance_part;
    if (duration <= 60) {
        tolerance_part = (uint64_t)((double)duration * (double)0.5);
    } else {
        tolerance_part = (uint64_t)((double)duration * (double)0.1);
    }
    return tolerance_part + (uint64_t)duration;
}
int32_t send_client_connection_ping_req(MqttSnGatewayClientConnectionHandler *handler, MqttSnGatewayForwarder *forwarders) {
    if (isFull(handler->clientNetworkSendBuffer)) {
        return 0;
    }
    MqttSnMessageData msg = { 0 };
    if (get_timestamp_s(&msg.create_time_s)) {
        return -1;
    }

    msg.to         = forwarders->wireless_node_id;
    int32_t gen_rc = generate_ping_req(msg.data, sizeof(msg.data));
    if (gen_rc < 0) {
#if WITH_LOGGING
        log_pingreq_gen_error(handler->logger, __FILE__, __func__, __LINE__);
#endif
        return -1;
    }
    msg.data_length = gateway_add_forwarder_encapsulation_frames(&msg, gen_rc, forwarders, handler->logger);

    if (put(handler->clientNetworkSendBuffer, &msg) < 0) {
#if WITH_LOGGING
        log_fatal_queue_error(handler->logger, handler->clientNetworkSendBuffer, __FILE__, __func__, __LINE__, PSTR("put"));
#endif
        return -1;
    }
    DB_HANDLER_CK_CLIENT_RC(db_set_ping_req_await_msg_type(handler->db_handler_, PINGRESP))
#if WITH_DEBUG_LOGGING
    // ping req to <wireless_node_id> <client_id> <duration> <last_ping_resp_received> <last_ping_req_received>
    uint16_t connect_duration;
    uint64_t ping_req_received;
    uint64_t ping_resp_received;
    char     client_id[MQTT_SN_CLIENTID_MAX_LENGTH] = { 0 };
    DB_HANDLER_CK_CLIENT_RC(db_get_client_connect_duration(handler->db_handler_, &connect_duration))
    DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_req_received(handler->db_handler_, &ping_req_received))
    DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_resp_received(handler->db_handler_, &ping_resp_received))
    DB_HANDLER_CK_CLIENT_RC(db_get_client_id(handler->db_handler_, client_id))
    log_client_connection_client_send_ping_req(handler->logger, &forwarders->wireless_node_id, client_id, connect_duration,
                                               ping_req_received, ping_resp_received);
#endif
    return 0;
}
int32_t client_connection_handler_parse_and_handle_ping_resp(MqttSnGatewayClientConnectionHandler *handler, MqttSnMessageData *msg,
                                                             int32_t parsed_bytes, MqttSnGatewayForwarder *forwarders) {
    assert(!isFull(handler->clientNetworkSendBuffer));

    if (parse_ping_resp_byte(msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
#ifdef WITH_LOGGING
        log_malformed_message(handler->logger, msg);
#endif
        return 0;
    }
#ifdef WITH_DEBUG_LOGGING
    // ping resp from <wireless_node_id> <client_id> <duration> <last_ping_resp_received> <last_ping_req_received>
    DB_ENTRY_MQTT_SN_CLIENT_RESULT start_rc = db_start_client_transaction(handler->db_handler_, &msg->from, NULL);

    if (start_rc == DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_NOT_FOUND) {
        log_client_connection_ping_resp_received(handler->logger, &forwarders->wireless_node_id);
    } else {
        uint16_t connect_duration;
        uint64_t ping_req_received;
        uint64_t ping_resp_received;
        char     client_id[MQTT_SN_CLIENTID_MAX_LENGTH] = { 0 };
        DB_HANDLER_CK_CLIENT_RC(db_get_client_connect_duration(handler->db_handler_, &connect_duration))
        DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_req_received(handler->db_handler_, &ping_req_received))
        DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_resp_received(handler->db_handler_, &ping_resp_received))
        DB_HANDLER_CK_CLIENT_RC(db_get_client_id(handler->db_handler_, client_id))
        log_client_connection_client_ping_resp_received(handler->logger, &forwarders->wireless_node_id, client_id, connect_duration,
                                                        ping_req_received, ping_resp_received);
    }
#else
    db_start_client_transaction(handler->db_handler_, &msg->from, NULL);
#endif

    db_set_client_last_ping_resp_received(handler->db_handler_, msg->create_time_s);
    db_set_ping_req_await_msg_type(handler->db_handler_, ANY_MESSAGE_TYPE);

    if (db_end_client_transaction(handler->db_handler_) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS &&
        get_db_handler_result(handler->db_handler_) == DB_HANDLER_RESULT_ERROR) {
#ifdef WITH_LOGGING
        log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                  get_db_handler_result(handler->db_handler_));
#endif
        return -1;
    }
    return 0;
}
int32_t client_connection_handler_parse_and_handle_ping_req(MqttSnGatewayClientConnectionHandler *handler, MqttSnMessageData *msg,
                                                            int32_t parsed_bytes, MqttSnGatewayForwarder *forwarders) {
    assert(!isFull(handler->clientNetworkSendBuffer));

    if (parse_ping_req_byte(msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
#ifdef WITH_LOGGING
        log_malformed_message(handler->logger, msg);
#endif
        return 0;
    }

#ifdef WITH_DEBUG_LOGGING
    DB_ENTRY_MQTT_SN_CLIENT_RESULT start_rc = db_start_client_transaction(handler->db_handler_, &msg->from, NULL);
    if (start_rc == DB_ENTRY_MQTT_SN_CLIENT_RESULT_CLIENT_NOT_FOUND) {
        log_client_connection_ping_req_received(handler->logger, &forwarders->wireless_node_id);
    } else {
        uint16_t connect_duration;
        uint64_t ping_req_received;
        uint64_t ping_resp_received;
        char     client_id[MQTT_SN_CLIENTID_MAX_LENGTH] = { 0 };
        DB_HANDLER_CK_CLIENT_RC(db_get_client_connect_duration(handler->db_handler_, &connect_duration))
        DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_req_received(handler->db_handler_, &ping_req_received))
        DB_HANDLER_CK_CLIENT_RC(db_get_client_last_ping_resp_received(handler->db_handler_, &ping_resp_received))
        DB_HANDLER_CK_CLIENT_RC(db_get_client_id(handler->db_handler_, client_id))
        log_client_connection_client_ping_req_received(handler->logger, &forwarders->wireless_node_id, client_id, connect_duration,
                                                       ping_req_received, ping_resp_received);
    }
#else
    db_start_client_transaction(handler->db_handler_, &msg->from, NULL);
#endif

    db_set_client_last_ping_req_received(handler->db_handler_, msg->create_time_s);

    if (db_end_client_transaction(handler->db_handler_) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS &&
        get_db_handler_result(handler->db_handler_) == DB_HANDLER_RESULT_ERROR) {
#ifdef WITH_LOGGING
        log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                  get_db_handler_result(handler->db_handler_));
#endif
        return -1;
    }

    // reuse msg to save memory
    if (reuse_mqtt_sn_message_data(msg) < 0) {
        return -1;
    }
    int32_t gen_bytes = generate_ping_resp(msg->data, sizeof(msg->data));
    if (gen_bytes < 0) {
#ifdef WITH_LOGGING
        log_msg_gen_error(handler->logger, PINGREQ, __FILE__, __func__, __LINE__);
#endif
        return -1;
    }

    msg->data_length = gateway_add_forwarder_encapsulation_frames(msg, gen_bytes, forwarders, handler->logger);
    if (put(handler->clientNetworkSendBuffer, msg) < 0) {
#ifdef WITH_LOGGING
        log_fatal_queue_error(handler->logger, handler->clientNetworkSendBuffer, __FILE__, __func__, __LINE__, PSTR("put"));
#endif
        return -1;
    }
    return 0;
}
int32_t client_connection_handler_parse_and_handle_connect(MqttSnGatewayClientConnectionHandler *handler, MqttSnMessageData *msg,
                                                           int32_t parsed_bytes, MqttSnGatewayForwarder *forwarders) {
    if (isFull(handler->clientNetworkSendBuffer)) {
        return 0;
    }

    // parse
    ParsedMqttSnConnect connect = { 0 };
    if (parse_connect(&connect, msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
        return 0;
    }

    db_start_client_transaction(handler->db_handler_, &forwarders->wireless_node_id, NULL);
    if (client_exist(handler->db_handler_) == DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
        if (!connect.clean_session) {
            db_reset_client(handler->db_handler_, connect.client_id, &forwarders->wireless_node_id, forwarders->forwarder_addresses,
                            forwarders->forwarder_len, connect.duration, msg->create_time_s);
        } else {
            // before we can remove the client, we must unsubscribe from all subscriptions
            client_connection_handler_remove_client_subscriptions(handler, connect.client_id, &forwarders->wireless_node_id);
            delete_client(handler->db_handler_);
            add_client(handler->db_handler_, connect.client_id, &forwarders->wireless_node_id, forwarders->forwarder_addresses,
                       forwarders->forwarder_len, connect.duration, msg->create_time_s);
        }
    } else {
        add_client(handler->db_handler_, connect.client_id, &forwarders->wireless_node_id, forwarders->forwarder_addresses,
                   forwarders->forwarder_len, connect.duration, msg->create_time_s);
    }

    if (db_end_client_transaction(handler->db_handler_) != DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS) {
#if WITH_LOGGING
        log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                  get_db_handler_result(handler->db_handler_));
#endif
        return -1;
    }

    if (connect.will) {
        // TODO send will topic request
    } else {
        if (reuse_mqtt_sn_message_data(msg) < 0) {
            return -1;
        }

        int32_t gen_bytes = 0;
        if ((gen_bytes = generate_connack(msg->data + gen_bytes, sizeof(msg->data) - gen_bytes, RETURN_CODE_ACCEPTED)) < 0) {
#if WITH_LOGGING
            log_msg_gen_error(handler->logger, CONNACK, __FILE__, __func__, __LINE__);
#endif
            return -1;
        }

        msg->data_length = gateway_add_forwarder_encapsulation_frames(msg, gen_bytes, forwarders, handler->logger);

        if (put(handler->clientNetworkSendBuffer, msg) < 0) {
            // should never happen
#if WITH_LOGGING
            log_fatal_queue_error(handler->logger, handler->clientNetworkSendBuffer, __FILE__, __func__, __LINE__, PSTR("put"));
#endif
            return -1;
        }
    }

#if WITH_LOGGING
    log_client_connection_client_connected(handler->logger, &forwarders->wireless_node_id, connect.client_id, connect.duration,
                                           connect.clean_session);
#endif
    return 0;
}

int32_t client_connection_handler_remove_client_subscriptions(MqttSnGatewayClientConnectionHandler *gateway, const char *handler,
                                                              device_address *client_address) {
    // TODO implement me
    return 0;
}
int32_t client_connection_handler_parse_and_handle_disconnect(MqttSnGatewayClientConnectionHandler *handler, MqttSnMessageData *msg,
                                                              int32_t parsed_bytes, MqttSnGatewayForwarder *forwarders) {
    if (isFull(handler->clientNetworkSendBuffer)) {
        return 0;
    }

    if (msg->data_length - parsed_bytes > MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH) {
        return client_connection_handler_parse_and_handle_disconnect_duration(handler, msg, parsed_bytes, forwarders);
    }

    // normal disconnect

    // parse
    if (parse_disconnect_bytes(msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
        return 0;
    }
    db_start_client_transaction(handler->db_handler_, &forwarders->wireless_node_id, NULL);

    // TODO check state first only possibel from sleep, awake, connected
    DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status;
    db_get_client_status(handler->db_handler_, &client_status);
    if (client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE || client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP ||
        client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE) {
        db_set_client_status(handler->db_handler_, DB_ENTRY_MQTT_SN_CLIENT_STATUS_DISCONNECTED);
    }

#if WITH_LOGGING
    if (client_exist(handler->db_handler_) == DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS &&
        (client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE || client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP ||
         client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE)) {
        char     client_id[MQTT_SN_MAX_CLIENT_ID_LENGTH] = { 0 };
        uint16_t connect_duration                        = 0;
        db_get_client_id(handler->db_handler_, client_id);
        db_get_client_connect_duration(handler->db_handler_, &connect_duration);
        log_client_connection_client_disconnected(handler->logger, &forwarders->wireless_node_id, client_id, connect_duration);
    }
#endif

    if (db_end_client_transaction(handler->db_handler_) == DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR) {
#if WITH_LOGGING
        log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                  get_db_handler_result(handler->db_handler_));
#endif
        return -1;
    }

    if (!(client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE || client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP ||
          client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE)) {
        // client has wrong status - send no disconnect
        return 0;
    }

    if (reuse_mqtt_sn_message_data(msg) < 0) {
        return -1;
    }
    int32_t gen_bytes = 0;
    if ((gen_bytes = generate_disconnect_message(msg->data + gen_bytes, sizeof(msg->data) - gen_bytes)) < 0) {
#if WITH_LOGGING
        log_msg_gen_error(handler->logger, CONNACK, __FILE__, __func__, __LINE__);
#endif
        return -1;
    }
    msg->data_length = gateway_add_forwarder_encapsulation_frames(msg, gen_bytes, forwarders, handler->logger);
    if (put(handler->clientNetworkSendBuffer, msg) < 0) {
        // should never happen
#if WITH_LOGGING
        log_fatal_queue_error(handler->logger, handler->clientNetworkSendBuffer, __FILE__, __func__, __LINE__, PSTR("put"));
#endif
        return -1;
    }

    return 0;
}
int32_t client_connection_handler_parse_and_handle_disconnect_duration(MqttSnGatewayClientConnectionHandler *handler,
                                                                       MqttSnMessageData *msg, int32_t parsed_bytes,
                                                                       MqttSnGatewayForwarder *forwarders) {
    // parse
    uint16_t sleep_duration = 0;
    if (parse_disconnect_duration(&sleep_duration, msg->data + parsed_bytes, msg->data_length - parsed_bytes) < 0) {
        return 0;
    }
    db_start_client_transaction(handler->db_handler_, &forwarders->wireless_node_id, NULL);

    DB_ENTRY_MQTT_SN_CLIENT_STATUS client_status;
    db_get_client_status(handler->db_handler_, &client_status);
    if (client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE || client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP ||
        client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE) {
        db_set_client_connect_duration(handler->db_handler_, sleep_duration);
        db_set_client_status(handler->db_handler_, DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP);
    }

#if WITH_LOGGING
    if (client_exist(handler->db_handler_) == DB_ENTRY_MQTT_SN_CLIENT_RESULT_SUCCESS &&
        (client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ACTIVE || client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_ASLEEP ||
         client_status == DB_ENTRY_MQTT_SN_CLIENT_STATUS_AWAKE)) {
        char     client_id[MQTT_SN_MAX_CLIENT_ID_LENGTH] = { 0 };
        uint16_t connect_duration                        = 0;
        db_get_client_id(handler->db_handler_, client_id);
        db_get_client_connect_duration(handler->db_handler_, &connect_duration);
        log_client_connection_client_asleep(handler->logger, &forwarders->wireless_node_id, client_id, connect_duration);
    }
#endif

    if (db_end_client_transaction(handler->db_handler_) == DB_ENTRY_MQTT_SN_CLIENT_RESULT_ERROR) {
#if WITH_LOGGING
        log_fatal_client_db_error(handler->logger, __FILE__, __func__, __LINE__, db_get_client_transaction_result(handler->db_handler_),
                                  get_db_handler_result(handler->db_handler_));
#endif
        return -1;
    }

    // TODO maybe we should send a disconnect with duration back as acknowledgement - or a config parameter
    return 0;
}
