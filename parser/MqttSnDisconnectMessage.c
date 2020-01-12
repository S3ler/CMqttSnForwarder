//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnDisconnectMessage.h"

static int32_t parse_disconnect_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
static int32_t parse_disconnect_internal_duration(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *duration);
static int32_t generate_disconnect_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes);
int32_t        parse_disconnect_duration(uint16_t *duration, const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_disconnect_header(&h, data, data_len, &parsed_bytes) < 0)) {
        return -1;
    }
    if ((parsed_bytes = parse_disconnect_internal_duration(data + parsed_bytes, data_len, &parsed_bytes, duration)) < 0) {
        return -1;
    }
    return parsed_bytes;
}

int32_t parse_disconnect_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, DISCONNECT, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length < MQTT_SN_MESSAGE_DISCONNECT_WO_DURATION_LENGTH) {
        return -1;
    }
    if (h->length > MQTT_SN_MESSAGE_DISCONNECT_W_DURATION_LENGTH) {
        return -1;
    }
    return *parsed_bytes;
}
int32_t parse_disconnect_internal_duration(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *duration) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, duration);
}
int32_t generate_disconnect_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_DISCONNECT_WO_DURATION_LENGTH, DISCONNECT);
}
int32_t generate_disconnect_message(uint8_t *dst, uint16_t dst_len) {
    int32_t used_bytes = 0;
    if (generate_disconnect_header(dst + used_bytes, dst_len, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t parse_disconnect_bytes(const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_disconnect_header(&h, data, data_len, &parsed_bytes) < 0)) {
        return -1;
    }
    return 0;
}
int32_t generate_disconnect_add_duration(uint8_t *dst_pos, uint16_t dst_len, uint16_t duration, int32_t *used_bytes) {
    return generate_mqtt_sn_uint16(dst_pos, dst_len, duration, used_bytes);
}
int32_t generate_disconnect_header_with_duration(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_DISCONNECT_W_DURATION_LENGTH, DISCONNECT);
}
int32_t generate_disconnect_duration(uint8_t *dst, uint16_t dst_len, uint16_t duration) {
    int32_t used_bytes = 0;
    if (generate_disconnect_header_with_duration(dst + used_bytes, dst_len, &used_bytes) < 0) {
        return -1;
    }
    if (generate_disconnect_add_duration(dst + used_bytes, dst_len, duration, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
