//
// Created by SomeDude on 11.05.2019.
//

#include "MqttSnDebugMessageLogging.h"
#include <platform/platform_compatibility.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>
#include <parser/logging/MqttSnMessageParserLogging.h>

int log_db_rec_client_message(const MqttSnLogger *logger, const device_address *from, const device_address *to, const uint8_t *data,
                              uint16_t data_len) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return 0;
    }

    log_msg_start(logger);
    log_str(logger, PSTR("received client message from "));
    log_device_address(logger, from);
    log_str(logger, PSTR(" to "));
    log_device_address(logger, to);
    log_str(logger, PSTR(" ( len"));
    log_uint32(logger, data_len);
    log_str(logger, PSTR(", data( "));
    log_uint8_array(logger, data, data_len);
    log_str(logger, PSTR("))."));

    log_flush(logger);
    return log_status(logger);
}
int log_db_send_client_message(const MqttSnLogger *logger, const device_address *from, const device_address *dst, const uint8_t *data,
                               uint16_t data_len) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return 0;
    }

    log_msg_start(logger);
    log_str(logger, PSTR("send client message from "));
    log_device_address(logger, from);
    log_str(logger, PSTR(" to "));
    log_device_address(logger, dst);
    log_str(logger, PSTR(" ( len"));
    log_uint32(logger, data_len);
    log_str(logger, PSTR(", data( "));
    log_uint8_array(logger, data, data_len);
    log_str(logger, PSTR("))."));

    log_flush(logger);
    return log_status(logger);
}
int log_incomplete_client_message(const MqttSnLogger *logger, const device_address *from, const device_address *to, uint8_t signal_strength,
                                  const uint8_t *data, uint16_t data_len) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return 0;
    }
    log_msg_start(logger);
    log_str(logger, PSTR("could not send client message completely: "));
    if (from) {
        log_str(logger, PSTR("from "));
        log_device_address(logger, from);
    }
    if (to) {
        log_str(logger, PSTR(" to "));
        log_device_address(logger, to);
    }
    log_str(logger, PSTR(" signal strength "));
    log_signal_strength(logger, signal_strength);

    log_flush(logger);
    return log_status(logger);
}
