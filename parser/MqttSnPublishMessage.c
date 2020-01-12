//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnPublishMessage.h"
#include <string.h>

int32_t parse_publish_header(ParsedMqttSnHeader *h, const uint8_t *src, uint16_t src_len, int32_t *parsed_bytes);
int32_t parse_publish_flags(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *dup, int8_t *qos, uint8_t *retain,
                            uint8_t *topic_id_type);

int32_t parse_publish_topic_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, int8_t qos, uint16_t *topic_id);
int32_t parse_publish_msg_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *msg_id);
int32_t parse_publish_data(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *data, uint16_t *data_length,
                           uint16_t data_max_length);

int32_t parse_publish_byte(uint8_t *dup, int8_t *qos, uint8_t *retain, uint8_t *topic_id_type, uint16_t *topic_id, uint16_t *msg_id,
                           uint8_t *payload, uint16_t *payload_length, uint16_t payload_max_length, const uint8_t *data,
                           uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if (parse_publish_header(&h, data, data_len, &parsed_bytes) < 0) {
        return -1;
    }
    if (parse_publish_flags(data + parsed_bytes, data_len, &parsed_bytes, dup, qos, retain, topic_id_type) < 0) {
        return -1;
    }
    if (parse_publish_topic_id(data + parsed_bytes, data_len, &parsed_bytes, (*qos), topic_id) < 0) {
        return -1;
    }
    if (parse_publish_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
        return -1;
    }
    if (parse_publish_data(data + parsed_bytes, data_len, &parsed_bytes, payload, payload_length, payload_max_length) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int32_t parse_publish_data(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *data, uint16_t *data_length,
                           uint16_t data_max_length) {
    return parse_mqtt_sn_uint8_until_end_byte(src_pos, src_len, parsed_bytes, data, data_length, data_max_length);
}
int32_t parse_publish_msg_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *msg_id) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, msg_id);
}
int32_t parse_publish_topic_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, int8_t qos, uint16_t *topic_id) {
    if (parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, topic_id) < 0) {
        return -1;
    }
    if ((qos < 0) && ((*topic_id) != 0)) {
        return -1;
    }
    return (*parsed_bytes);
}
int32_t parse_publish_header(ParsedMqttSnHeader *h, const uint8_t *src, uint16_t src_len, int32_t *parsed_bytes) {
    if (parse_header(h, PUBLISH, src, src_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length < MQTT_SN_MESSAGE_PUBLISH_MIN_LENGTH) {
        return -1;
    }
    /*
  if (h->length > MQTT_SN_MESSAGE_PUBLISH_MAX_LENGTH) {
    return -1;
  }
  */
    return *parsed_bytes;
}
int32_t parse_publish_flags(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *dup, int8_t *qos, uint8_t *retain,
                            uint8_t *topic_id_type) {
    return parse_mqtt_sn_flags(src_pos, src_len, parsed_bytes, dup, qos, retain, NULL, NULL, topic_id_type);
}

static int32_t generate_publish_header(uint8_t *dst, uint16_t dst_len, uint16_t data_len, int32_t *used_bytes) {
    int32_t total_length = get_publish_msg_len(data_len);
    if (total_length < 0 || dst_len < total_length) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, total_length, PUBLISH);
}
int32_t generate_publish(uint8_t *dst, uint16_t dst_len, uint8_t dup, int8_t qos, uint8_t retain, uint8_t topic_id_type, uint32_t topic_id,
                         uint16_t msg_id, uint8_t *data, uint16_t data_len) {
    memset(dst, 0, dst_len);
    int32_t used_bytes = 0;
    if (generate_publish_header(dst, dst_len, data_len, &used_bytes) < 0) {
        return -1;
    }
    if (generate_flags(dst + used_bytes, dst_len, dup, qos, retain, MQTT_SN_FLAG_WILL_FALSE, MQTT_SN_FLAG_CLEAN_SESSION_FALSE,
                       topic_id_type, &used_bytes) < 0) {
        return -1;
    }
    if (generate_topic_id(dst + used_bytes, dst_len, topic_id, &used_bytes) < 0) {
        return -1;
    }
    if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes) < 0) {
        return -1;
    }
    if (generate_data(dst + used_bytes, dst_len, data, data_len, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t get_publish_msg_len(uint16_t data_len) {
    if (data_len > UINT16_MAX - MQTT_SN_MESSAGE_PUBLISH_WO_DATA_MAX_LENGTH) {
        return -1;
    }
    uint16_t total_length = MQTT_SN_MESSAGE_PUBLISH_MIN_LENGTH + data_len;
    if (total_length > UINT8_MAX) {
        total_length = MQTT_SN_MESSAGE_PUBLISH_WO_DATA_MAX_LENGTH + data_len;
    }
    return total_length;
}
int32_t parse_publish(MqttSnPublish *publish, const uint8_t *data, uint16_t data_len) {
    return parse_publish_byte_payload_ptr(&publish->dup, &publish->qos, &publish->retain, &publish->topic_id_type, &publish->topic_id,
                                          &publish->msg_id, &publish->payload, &publish->payload_length, data, data_len);
}
int32_t parse_publish_payload_ptr(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t **payload_ptr,
                                  uint16_t *payload_length) {
    return parse_mqtt_sn_uint8_until_end_byte_ptr(src_pos, src_len, parsed_bytes, payload_ptr, payload_length);
}
int32_t parse_publish_byte_payload_ptr(uint8_t *dup, int8_t *qos, uint8_t *retain, uint8_t *topic_id_type, uint16_t *topic_id,
                                       uint16_t *msg_id, uint8_t **payload_ptr, uint16_t *payload_length, const uint8_t *data,
                                       uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if (parse_publish_header(&h, data, data_len, &parsed_bytes) < 0) {
        return -1;
    }
    if (parse_publish_flags(data + parsed_bytes, data_len, &parsed_bytes, dup, qos, retain, topic_id_type) < 0) {
        return -1;
    }
    if (parse_publish_topic_id(data + parsed_bytes, data_len, &parsed_bytes, (*qos), topic_id) < 0) {
        return -1;
    }
    if (parse_publish_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
        return -1;
    }
    if (parse_publish_payload_ptr(data + parsed_bytes, data_len, &parsed_bytes, payload_ptr, payload_length) < 0) {
        return -1;
    }
    return parsed_bytes;
}
