//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnSubackMessage.h"

static int32_t parse_suback_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
static int32_t parse_suback_topic_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *topic_id);
static int32_t parse_suback_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id);
static int32_t parse_suback_return_code_byte(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes,
                                             MQTT_SN_RETURN_CODE *return_code);
static int32_t parse_suback_qos(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, int8_t *granted_qos);

int32_t parse_suback_bytes(int8_t *granted_qos, uint16_t *topic_id, uint16_t *msg_id, MQTT_SN_RETURN_CODE *return_code, const uint8_t *data,
                           uint16_t data_len) {
    int32_t parsed_bytes = 0;

    ParsedMqttSnHeader h = { 0 };
    if (parse_suback_header(&h, data, data_len, &parsed_bytes) < 0) {
        return -1;
    }
    if (parse_suback_qos(data + parsed_bytes, data_len, &parsed_bytes, granted_qos) < 0) {
        return -1;
    }
    if (parse_suback_topic_id(data + parsed_bytes, data_len, &parsed_bytes, topic_id) < 0) {
        return -1;
    }
    if (parse_suback_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
        return -1;
    }
    if (parse_suback_return_code_byte(data + parsed_bytes, data_len, &parsed_bytes, return_code) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int32_t parse_suback(MqttSnSuback *suback, const uint8_t *data, uint16_t data_len) {
    return parse_suback_bytes(&suback->granted_qos, &suback->topic_id, &suback->msg_id, &suback->return_code, data, data_len);
}
int32_t parse_suback_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, SUBACK, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length != MQTT_SN_MESSAGE_SUBACK_LENGTH) {
        return -1;
    }
    return *parsed_bytes;
}
int32_t parse_suback_topic_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *topic_id) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, topic_id);
}
int32_t parse_suback_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, msg_id);
}
int32_t parse_suback_return_code_byte(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, MQTT_SN_RETURN_CODE *return_code) {
    return parse_mqtt_sn_return_code_byte(src_pos, src_len, parsed_bytes, return_code);
}
int32_t parse_suback_qos(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, int8_t *granted_qos) {
    return parse_mqtt_sn_flags(src_pos, src_length, parsed_bytes, NULL, granted_qos, NULL, NULL, NULL, NULL);
}
