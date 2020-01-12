//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnPubrecPubrelPubcompMessage.h"

static int32_t parse_pubrec_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
static int32_t parse_pubrec_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id);

int32_t generate_pubrel_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes);
int32_t parse_pubrec_bytes(uint16_t *msg_id, const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if (parse_pubrec_header(&h, data, data_len, &parsed_bytes) < 0) {
        return -1;
    }
    if (parse_pubrec_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int32_t parse_pubrel_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, PUBREL, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length != MQTT_SN_MESSAGE_PUBREL_LENGTH) {
        return -1;
    }
    return *parsed_bytes;
}
int32_t parse_pubrel_bytes(uint16_t *msg_id, const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if (parse_pubrel_header(&h, data, data_len, &parsed_bytes) < 0) {
        return -1;
    }
    if (parse_mqtt_sn_uint16_byte(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int32_t generate_pubrel(uint8_t *dst, uint16_t dst_len, uint16_t msg_id) {
    int32_t used_bytes = 0;
    if (generate_pubrel_header(dst, dst_len, &used_bytes) < 0) {
        return -1;
    }
    if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t parse_pubcomp_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, PUBCOMP, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length != MQTT_SN_MESSAGE_PUBCOMP_LENGTH) {
        return -1;
    }
    return *parsed_bytes;
}
int32_t parse_pubcomp_bytes(uint16_t *msg_id, const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if (parse_pubcomp_header(&h, data, data_len, &parsed_bytes) < 0) {
        return -1;
    }
    if (parse_mqtt_sn_uint16_byte(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int generate_pubrec_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    if (dst_len < MQTT_SN_MESSAGE_PUBREC_LENGTH) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_PUBREC_LENGTH, PUBREC);
}
int32_t generate_pubrec(uint8_t *dst, uint16_t dst_len, uint16_t msg_id) {
    int32_t used_bytes = 0;
    if (generate_pubrec_header(dst, dst_len, &used_bytes) < 0) {
        return -1;
    }
    if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int generate_pubcomp_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    if (dst_len < MQTT_SN_MESSAGE_PUBCOMP_LENGTH) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_PUBCOMP_LENGTH, PUBCOMP);
}
int32_t generate_pubcomp(uint8_t *dst, uint16_t dst_len, uint16_t msg_id) {
    int32_t used_bytes = 0;
    if (generate_pubcomp_header(dst, dst_len, &used_bytes) < 0) {
        return -1;
    }
    if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}

int generate_pubrel_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    if (dst_len < MQTT_SN_MESSAGE_PUBREL_LENGTH) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_PUBREL_LENGTH, PUBREL);
}
int32_t parse_pubrec_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, PUBREC, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length != MQTT_SN_MESSAGE_PUBREC_LENGTH) {
        return -1;
    }
    return *parsed_bytes;
}
int32_t parse_pubrec_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, msg_id);
}
