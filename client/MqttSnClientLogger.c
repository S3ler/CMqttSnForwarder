//
// Created by SomeDude on 27.06.2019.
//

#include <parser/logging/MqttSnMessageParserLogging.h>
#include "MqttSnClientLogger.h"
#include "MqttSnClientMsgHandlerReturnValue.h"

int32_t log_mqtt_sn_client_return_code(const MqttSnLogger *logger, MQTT_SN_CLIENT_RETURN_CODE mqtt_sn_client_return_code) {
    switch (mqtt_sn_client_return_code) {
    case MQTT_SN_CLIENT_RETURN_SUCCESS:
        log_str(logger, PSTR("MQTT_SN_CLIENT_RETURN_SUCCESS"));
        break;
    case MQTT_SN_CLIENT_RETURN_CONGESTION:
        log_str(logger, "MQTT_SN_CLIENT_RETURN_CONGESTION");
        break;
    case MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED:
        log_str(logger, "MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED");
        break;
    case MQTT_SN_CLIENT_RETURN_TIMEOUT:
        log_str(logger, "MQTT_SN_CLIENT_RETURN_TIMEOUT");
        break;
    case MQTT_SN_CLIENT_RETURN_NETWORK_ERROR:
        log_str(logger, "MQTT_SN_CLIENT_RETURN_NETWORK_ERROR");
        break;
    case MQTT_SN_CLIENT_RETURN_NOT_CONNECTED:
        log_str(logger, "MQTT_SN_CLIENT_RETURN_NOT_CONNECTED");
        break;
    case MQTT_SN_CLIENT_RETURN_CONNECTION_LOST:
        log_str(logger, "MQTT_SN_CLIENT_RETURN_CONNECTION_LOST");
        break;
    case MQTT_SN_CLIENT_RETURN_ERROR:
        log_str(logger, "MQTT_SN_CLIENT_RETURN_ERROR");
        break;
    default:
        break;
    }
    log_flush(logger);

    return log_status(logger);
}
int32_t log_mqtt_sn_client_connection_status(const MqttSnLogger *logger, uint8_t connection_status, const device_address *gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("timeout connected to gateway  "));
    log_device_address(logger, gateway_address);
    log_str(logger, PSTR("."));
    log_flush(logger);
    return log_status(logger);
}
int32_t print_fatal_mqtt_sn_client_error_in_file(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                                 int32_t line_number) {
    log_str(logger, PSTR("fatal error in file: "));
    log_str(logger, file_name);
    log_str(logger, PSTR(" function: "));
    log_str(logger, function_name);
    log_str(logger, PSTR(" line: "));
    log_int32(logger, line_number);
    return log_status(logger);
}
int32_t log_fatal_mqtt_sn_client_loop_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                            int32_t line_number) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    print_fatal_mqtt_sn_client_error_in_file(logger, file_name, function_name, line_number);

    log_str(logger, PSTR(" loop_error"));
    log_str(logger, PSTR("."));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_fatal_mqtt_sn_client_send_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                            int32_t line_number, int32_t send_rc, int32_t to_send) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    print_fatal_mqtt_sn_client_error_in_file(logger, file_name, function_name, line_number);

    log_str(logger, PSTR(" send_rc: "));
    log_int32(logger, send_rc);
    log_str(logger, PSTR(" to_send: "));
    log_int32(logger, to_send);
    log_str(logger, PSTR("."));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_fatal_mqtt_sn_client_await_fd_conversion_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                                           int32_t line_number, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    print_fatal_mqtt_sn_client_error_in_file(logger, file_name, function_name, line_number);

    log_str(logger, PSTR(" MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS: "));
    if (status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
        log_str(logger, PSTR("MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS"));
    } else if (status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
        log_str(logger, PSTR("MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT"));
    } else if (status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR) {
        log_str(logger, PSTR("MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR"));
    } else if (status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
        log_str(logger, PSTR("MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT"));
    } else {
        log_str(logger, PSTR("UNDEFINED"));
    }
    log_str(logger, PSTR(" status: "));
    log_int32(logger, status);
    log_str(logger, PSTR("."));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_unhandled_message(const MqttSnLogger *logger, const MqttSnMessageData *msg) {
    // TODO
    return log_any_message(logger, msg);
}
int32_t log_mqtt_sn_client_mqtt_sn_message_malformed(const MqttSnLogger *logger, const MqttSnMessageData *msg) {
    return log_malformed_message(logger, msg);
}
int32_t log_mqtt_sn_client_registrations_full(const MqttSnLogger *logger, uint16_t registrations_len, uint16_t max_len) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("Cannot save registration into registration - list full"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_registrations_topic_name_too_long(const MqttSnLogger *logger, uint16_t topic_name_len, uint16_t max_len) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Cannot save registration into registration - topic name too long."));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_registrations_index_empty(const MqttSnLogger *logger, uint16_t registrations_len, uint16_t registration_index) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("Registration list empty"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_fatal_mqtt_sn_client_await_context_error(const MqttSnLogger *logger, const char *file_name, const char *function_name,
                                                     int32_t line_number) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("await context error: "));
    log_str(logger, PSTR(" file: "));
    log_str(logger, file_name);
    log_str(logger, PSTR(" function: "));
    log_str(logger, function_name);
    log_str(logger, PSTR(" line: "));
    log_int32(logger, line_number);
    log_flush(logger);
    return log_status(logger);
}

int32_t print_mqtt_sn_client_await_msg_status(const MqttSnLogger *logger, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_status) {
    if (await_status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
        log_str(logger, "SUCCESS");
    } else if (await_status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
        log_str(logger, "AWAIT");
    } else if (await_status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR) {
        log_str(logger, "ERROR");
    } else if (await_status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT) {
        log_str(logger, "TIMEOUT");
    } else {
        log_str(logger, "UNKNOWN");
    }
    return log_status(logger);
}
int32_t print_mqtt_sn_client_await_msg(const MqttSnLogger *logger, MqttSnClientAwaitMessage *await_msg) {
    log_mqtt_sn_message_type(logger, await_msg->msg_type);
    log_str(logger, PSTR(" "));
    log_uint16(logger, await_msg->msg_id);
    log_str(logger, PSTR(" "));
    print_mqtt_sn_client_await_msg_status(logger, await_msg->status);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_await_fd_status(const MqttSnLogger *logger, MqttSnClient *client) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_str(logger, PSTR("AWAIT_FD "));
    for (uint16_t i = 0; i < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
        print_mqtt_sn_client_await_msg(logger, &client->await_msg[i]);
        if (i + 1 < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT) {
            log_str(logger, PSTR(", "));
        }
    }
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_any_loop_message(const MqttSnLogger *logger, MQTT_SN_CLIENT_STATUS status, MQTT_SN_CLIENT_LOOP_RETURN_CODE rc,
                                            const MqttSnMessageData *msg) {
#ifdef WITH_DEBUG_LOGGING
    if (rc == MQTT_SN_CLIENT_LOOP_RETURN_UNHANDLEDMSG) {
        return log_mqtt_sn_client_unhandled_message(logger, msg);
    }
    if (rc == MQTT_SN_CLIENT_LOOP_RETURN_PARSEERROR) {
        return log_malformed_message(logger, msg);
    }
    if (rc == MQTT_SN_CLIENT_LOOP_RETURN_NOTAWAITEDMSG) {
        log_mqtt_sn_client_not_await_message(logger, msg);
    }
#endif
    return log_status(logger);
}
int32_t log_mqtt_sn_client_not_await_message(const MqttSnLogger *logger, const MqttSnMessageData *msg) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("TODO log_mqtt_sn_client_not_await_message"));
    return print_any_message(logger, msg);
}
int32_t log_mqtt_sn_client_initialization_failed(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Initialization - failed"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_initialization_success(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Initialization - success"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_deinitialization_success(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Deinitialization - success"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_deinitialization_failed(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Deinitialization - failed"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_reconnect_success(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Reconnect to "));
    log_str(logger, PSTR("TODO"));
    log_str(logger, PSTR(" - success"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_reconnect_failed(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Reconnect to"));
    log_str(logger, PSTR("TODO"));
    log_str(logger, PSTR(" - failed"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_reconnect_congestion(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Reconnect to"));
    log_str(logger, PSTR("TODO"));
    log_str(logger, PSTR(" - congestion"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_status_gateway_lost(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Connection Gateway "));
    log_str(logger, PSTR("TODO"));
    log_str(logger, PSTR(" - lost"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_connection_maintain_connection(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Maintaining Gateway connection to"));
    log_str(logger, PSTR("TODO"));
    log_str(logger, PSTR(" - lost"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_connection_maintain_wakeup(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Maintaining WakeUp connection to"));
    log_str(logger, PSTR("TODO"));
    log_str(logger, PSTR(" - lost"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_status_asleep(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Client status asleep"));

    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_reconnected_to(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Reconnected to Gateway "));
    log_device_address(logger, mqtt_sn_gateway_address);
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_connected_to(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Connected to Gateway "));
    log_device_address(logger, mqtt_sn_gateway_address);
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_client_connect_failed(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("Connect to Gateway - failed"));
    log_flush(logger);
    return log_status(logger);
}
