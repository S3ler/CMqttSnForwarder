//
// Created by bele on 20.05.19.
//

#include <platform/platform_compatibility.h>
#include <parser/MqttSnMessageParser.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#include "MqttSnMessageParserLogging.h"
#include "MqttSnAdvertiseMessageLogging.h"
#include "MqttSnSearchGwMessageLogging.h"
#include "MqttSnGwInfoMessageLogging.h"

#define MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET 2
static const char *MQTT_SN_MESSAGE_TYPE_STRING[] = { FOREACH_MQTT_SN_MESSAGE_TYPE(GENERATE_MQTT_SN_MESSAGE_TYPE_STRING) };

int log_duration(const MqttSnLogger *logger, uint16_t duration) {
    return log_str_key_uint16_value(logger, PSTR("dur"), duration);
}

int log_msg_from(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from) {
    if (MQTT_SN_MESSAGE_TYPE_RESERVED(msg_type)) {
        return log_status(logger);
    }
    const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
    log_str(logger, msg_type_str);
    log_str(logger, PSTR(" from "));
    log_device_address(logger, from);
    log_str(logger, PSTR(" "));
    return log_status(logger);
}
int log_msg_to(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from) {
    if (MQTT_SN_MESSAGE_TYPE_RESERVED(msg_type)) {
        return log_status(logger);
    }
    const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
    log_str(logger, msg_type_str);
    log_str(logger, PSTR(" to "));
    log_device_address(logger, from);
    log_char(logger, ' ');
    return log_status(logger);
}
int32_t log_mqtt_sn_payload(const MqttSnLogger *logger, const uint8_t *payload, uint16_t payload_length) {
#ifdef WITH_DEBUG_LOGGING
    if (!shall_not_be_logged(logger, LOG_LEVEL_QUIET)) {
        log_str(logger, PSTR("data( "));
        log_uint8_array(logger, payload, payload_length);
        log_str(logger, PSTR(")"));
    } else {
#endif
        log_str(logger, PSTR("... ("));
        log_uint16(logger, payload_length);
        log_str(logger, PSTR(" bytes)"));
#ifdef WITH_DEBUG_LOGGING
    }
#else

#endif
    return log_status(logger);
}
int log_clean_session_flag(const MqttSnLogger *logger, uint8_t clean_session) {
    return log_char_key_uint8_value(logger, 'c', clean_session);
}
int32_t log_mqtt_sn_message_type(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type) {
    const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
    return log_str(logger, msg_type_str);
}

int32_t log_any_message(const MqttSnLogger *logger, const MqttSnMessageData *msg) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    return print_any_message(logger, msg);
    //
}
int32_t print_any_message(const MqttSnLogger *logger, const MqttSnMessageData *msg) {
    return print_any_message_bytes(logger, &msg->from, &msg->to, msg->signal_strength, msg->data, msg->data_length);
}


int32_t print_any_message_bytes(const MqttSnLogger *logger, const device_address *from, const device_address *to, uint8_t signal_strength,
                                const uint8_t *data, uint16_t data_length) {
    ParsedMqttSnHeader header              = { 0 };
    int32_t            parsed_header_bytes = 0;
    if (parse_header(&header, ANY_MESSAGE_TYPE, data, data_length, &parsed_header_bytes) < 0) {
        log_malformed_message_header(logger, from, signal_strength, data, data_length);
        return 0;
    }
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

    switch (header.msg_type) {
    case ADVERTISE: {
        MqttSnAdvertise advertise = { 0 };
        if (parse_advertise(&advertise, data, data_length) > 0) {
            return log_advertise_message_struct(logger, &advertise);
        }
        break;
    }
    case SEARCHGW: {
        MqttSnSearchGw search_gw = { 0 };
        if (parse_searchgw(&search_gw, data, data_length) > 0) {
            return log_searchgw_message_struct(logger, &search_gw);
        }
        break;
    }
    case GWINFO: {
        MqttSnGwInfo gw_info = { 0 };
        if (parse_gwinfo(&gw_info, data, data_length) > 0) {
            return log_gwinfo_message_struct(logger, &gw_info);
        }
        break;
    }
    default:
        break;
    }
    return print_malformed_message(logger, header.msg_type, from, to, signal_strength, data, data_length);
}
int32_t log_malformed_message_header(const MqttSnLogger *logger, const device_address *from, uint8_t signal_strength, const uint8_t *data,
                                     uint16_t data_len) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR("received malformed header "));

    log_str(logger, PSTR(" message"));

    log_str(logger, PSTR(" from "));
    log_device_address(logger, from);

    log_str(logger, PSTR(" signal strength: "));
    log_uint8(logger, signal_strength);

    log_str(logger, PSTR(" ( len"));
    log_uint16(logger, data_len);

#ifdef WITH_DEBUG_LOGGING
    if (!shall_not_be_logged(logger, LOG_LEVEL_DEBUG) && data != NULL) {
        log_str(logger, PSTR(", data( "));
        log_uint8_array(logger, data, data_len);
        log_str(logger, PSTR(")"));
    }
#endif
    log_str(logger, PSTR(")"));
    log_flush(logger);
    return log_status(logger);
}
int32_t log_msg_gen_error(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const char *file_name, const char *function_name,
                          int line_number) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    log_str(logger, PSTR("msg gen error: "));
    log_mqtt_sn_message_type(logger, msg_type);
    log_str(logger, PSTR(" file: "));
    log_str(logger, file_name);
    log_str(logger, PSTR(" function: "));
    log_str(logger, function_name);
    log_str(logger, PSTR(" line: "));
    log_int32(logger, line_number);

    log_flush(logger);
    return log_status(logger);
}
int32_t log_malformed_message(const MqttSnLogger *logger, const MqttSnMessageData *msg) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    return print_malformed_message(logger, ANY_MESSAGE_TYPE, &msg->from, &msg->to, msg->signal_strength, msg->data, msg->data_length);
}
int32_t print_malformed_message(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE optional_msg_type, const device_address *from,
                                const device_address *to, uint8_t signal_strength, const uint8_t *data, uint16_t data_len) {
    if (optional_msg_type != ANY_MESSAGE_TYPE) {
        // TODO log
    }
    // TODO log data only in debug#ifdef WITH_LOGGING
    log_str(logger, PSTR("print_malformed_message"));  // TODO implement me

    log_flush(logger);
    return log_status(logger);
}
int32_t log_fatal_cannot_add_dup_flag(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number,
                                      const uint8_t *msg_data, uint16_t msg_len) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);
    log_str(logger, PSTR(" file: "));
    log_str(logger, file_name);
    log_str(logger, PSTR(" function: "));
    log_str(logger, function_name);
    log_str(logger, PSTR(" line: "));
    log_int32(logger, line_number);
    log_str(logger, PSTR(" data: "));
    log_mqtt_sn_payload(logger, msg_data, msg_len);
    log_flush(logger);
    return log_status(logger);
}
