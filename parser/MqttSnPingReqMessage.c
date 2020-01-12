//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnPingReqMessage.h"
#include <string.h>

int32_t parse_ping_req_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
int32_t parse_ping_req_header_w_client_id(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
int32_t parse_ping_req_client_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, char *client_id,
                                 uint16_t *client_id_length, uint8_t client_id_max_length);

int32_t generate_ping_req_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes);
int32_t parse_ping_req_w_client_id_byte(char *client_id, uint16_t *client_id_length, uint8_t client_id_max_length, const uint8_t *data,
                                        uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_ping_req_header_w_client_id(&h, data, data_len, &parsed_bytes) < 0)) {
        return -1;
    }
    if ((parsed_bytes =
           parse_ping_req_client_id(data + parsed_bytes, data_len, &parsed_bytes, client_id, client_id_length, client_id_max_length)) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int32_t generate_ping_req(uint8_t *dst, uint16_t dst_len) {
    int32_t used_bytes = 0;
    if (generate_ping_req_header(dst + used_bytes, dst_len, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t parse_ping_req_byte(const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_ping_req_header(&h, data, data_len, &parsed_bytes) < 0)) {
        return -1;
    }
    return parsed_bytes;
}
int32_t generate_ping_req_header_w_client_id(uint8_t *dst, uint16_t dst_len, const char *client_id, uint16_t client_id_length,
                                             int32_t *used_bytes) {
    if (client_id_length > MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH) {
        return -1;
    }
    if (client_id_length == MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH && client_id[client_id_length] != '\0') {
        return -1;
    }
    uint16_t total_length = MQTT_SN_MESSAGE_PINGREQ_WO_CLIENTID_LENGTH + client_id_length;
    if (total_length > MQTT_SN_MESSAGE_PINGREQ_W_CLIENTID_MAX_LENGTH) {
        return -1;
    }

    if (dst_len < total_length) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, total_length, PINGREQ);
}
int32_t generate_ping_req_w_client_id_client_id(uint8_t *dst, uint16_t dst_len, const char *client_id, uint16_t client_id_length,
                                                int32_t *used_bytes) {
    (*used_bytes) += client_id_length;
    if (dst_len < (*used_bytes)) {
        return -1;
    }
    memcpy(dst, client_id, client_id_length);
    return (*used_bytes);
}
int32_t generate_ping_req_w_client_id(uint8_t *dst, uint64_t dst_len, const char *client_id, uint16_t client_id_length) {
    int32_t used_bytes = 0;
    if (generate_ping_req_header_w_client_id(dst, dst_len, client_id, client_id_length, &used_bytes) < 0) {
        return -1;
    }
    if (generate_ping_req_w_client_id_client_id(dst + used_bytes, dst_len, client_id, client_id_length, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t generate_ping_req_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_PINGREQ_WO_CLIENTID_LENGTH, PINGREQ);
}

int32_t parse_ping_req_header_w_client_id(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, PINGREQ, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length < MQTT_SN_MESSAGE_PINGREQ_MIN_LENGTH) {
        return -1;
    }
    if (h->length > MQTT_SN_MESSAGE_PINGREQ_W_CLIENTID_MAX_LENGTH) {
        return -1;
    }
    return *parsed_bytes;
}
int32_t parse_ping_req_client_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, char *client_id,
                                 uint16_t *client_id_length, uint8_t client_id_max_length) {
    if (client_id_max_length > MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH) {
        client_id_max_length = MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH;
    }
    return parse_mqtt_sn_client_id_byte(src_pos, src_len, parsed_bytes, client_id, client_id_length, client_id_max_length);
}
int32_t parse_ping_req_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, PINGREQ, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length != MQTT_SN_MESSAGE_PINGREQ_MIN_LENGTH) {
        return -1;
    }
    return *parsed_bytes;
}
