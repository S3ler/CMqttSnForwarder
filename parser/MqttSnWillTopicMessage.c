//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnWillTopicMessage.h"

static int32_t parse_will_topic_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
static int32_t parse_will_topic_flags(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, int8_t *qos, uint8_t *retain);
static int32_t parse_will_topic_will_topic(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, char *will_topic,
                                           uint16_t *will_topic_length, uint16_t will_topic_max_length);

int32_t parse_will_topic_byte(int8_t *qos, uint8_t *retain, char *will_topic, uint16_t *will_topic_length, uint16_t will_topic_max_length,
                              const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_will_topic_header(&h, data, data_len, &parsed_bytes)) < 0) {
        return -1;
    }
    if ((parsed_bytes = parse_will_topic_flags(data + parsed_bytes, data_len, &parsed_bytes, qos, retain)) < 0) {
        return -1;
    }
    if ((parsed_bytes = parse_will_topic_will_topic(data + parsed_bytes, data_len, &parsed_bytes, will_topic, will_topic_length,
                                                    will_topic_max_length)) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int32_t get_will_topic_msg_len(uint16_t data_len) {
    if (data_len > UINT16_MAX - MQTT_SN_MESSAGE_WILLTOPIC_WO_WILLTOPIC_MAX_LENGTH) {
        return -1;
    }
    uint16_t total_length = MQTT_SN_MESSAGE_WILLTOPIC_MIN_LENGTH + data_len;
    if (total_length > UINT8_MAX) {
        total_length = MQTT_SN_MESSAGE_WILLTOPIC_WO_WILLTOPIC_MAX_LENGTH + data_len;
    }
    return total_length;
}

int32_t generate_will_topic_header(uint8_t *dst, uint16_t dst_len, uint16_t data_len, int32_t *used_bytes) {
    int32_t total_length = get_will_topic_msg_len(data_len);
    if (total_length < 0 || dst_len < total_length) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, total_length, WILLTOPIC);
}
int32_t generate_will_topic(uint8_t *dst, uint16_t dst_len, int8_t will_qos, uint8_t will_retain, const char *will_topic,
                            uint16_t will_topic_length) {
    int32_t used_bytes = 0;
    if (generate_will_topic_header(dst, dst_len, will_topic_length, &used_bytes) < 0) {
        return -1;
    }
    if (generate_flags(dst + used_bytes, dst_len, 0, will_qos, will_retain, MQTT_SN_FLAG_WILL_FALSE, MQTT_SN_FLAG_CLEAN_SESSION_FALSE,
                       MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME, &used_bytes) < 0) {
        return -1;
    }
    if (generate_string(dst + used_bytes, dst_len, will_topic, will_topic_length, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}

int32_t parse_will_topic_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, WILLTOPIC, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length < MQTT_SN_MESSAGE_WILLTOPIC_MIN_LENGTH) {
        return -1;
    }
    /*
  if (h->length > MQTT_SN_MESSAGE_WILLTOPIC_MAX_LENGTH) {
    return -1;
  }
  */
    return *parsed_bytes;
}
int32_t parse_will_topic_flags(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, int8_t *qos, uint8_t *retain) {
    return parse_mqtt_sn_flags(src_pos, src_len, parsed_bytes, NULL, qos, retain, NULL, NULL, NULL);
}
int32_t parse_will_topic_will_topic(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, char *will_topic,
                                    uint16_t *will_topic_length, uint16_t will_topic_max_length) {
    if (will_topic_max_length > MQTT_SN_MESSAGE_WILLTOPIC_WILLTOPIC_MAX_LENGTH) {
        will_topic_max_length = MQTT_SN_MESSAGE_WILLTOPIC_WILLTOPIC_MAX_LENGTH;
    }
    return parse_mqtt_sn_char_until_end_byte(src_pos, src_len, parsed_bytes, will_topic, will_topic_length, will_topic_max_length);
}
