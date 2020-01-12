
#include "MqttSnForwarderLogging.h"
#include <platform/platform_compatibility.h>

int log_forwarder_started(const MqttSnLogger *logger, const char *version, int major, int minor, int tweak, const char *build_date) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
        return log_status(logger);
    }

    log_msg_start(logger);
    log_str(logger, PSTR("cmqttsnforwarder version "));
    log_str(logger, version);
    log_str(logger, PSTR(" is "));
    log_uint32(logger, major);
    log_str(logger, PSTR("."));
    log_uint32(logger, minor);
    log_str(logger, PSTR("."));
    log_uintmax(logger, tweak);
    log_str(logger, PSTR(" (build date "));
    log_str(logger, build_date);
    log_str(logger, PSTR(") started."));
    log_flush(logger);
    return log_status(logger);
}

int log_forwarder_terminated(const MqttSnLogger *logger, const char *version, uint32_t major, uint32_t minor, uintmax_t tweak) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
        return log_status(logger);
    }

    log_msg_start(logger);
    log_str(logger, PSTR("cmqttsnforwarder version "));
    log_str(logger, version);
    log_str(logger, PSTR(" is "));
    log_uint32(logger, major);
    log_str(logger, PSTR("."));
    log_uint32(logger, minor);
    log_str(logger, PSTR("."));
    log_uintmax(logger, tweak);
    log_str(logger, PSTR(" terminated."));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_connected(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("connected to gateway "));
    log_device_address(logger, mqtt_sn_gateway_address);
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_disconnect(const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("disconnected from gateway "));
    log_device_address(logger, mqtt_sn_gateway_address);
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_client_and_gateway_network_connection_lost(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("network connection to client and gateway network lost"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_client_network_connection_lost(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("network connection to client gateway network lost"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_gateway_network_connection_lost(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("network connection to gateway gateway network lost"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_sending_buffer_gateway_messages(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("sending buffered gateway messages to client network"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_sending_buffer_client_messages(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("sending buffered client messages to gateway network"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_buffer_gateway_messages_send(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("send buffered gateway messages to client network"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_mqtt_sn_forwarder_buffer_client_messages_send(const MqttSnLogger *logger) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("send buffered client messages to client network"));
    log_flush(logger);
    return log_status(logger);
}
