#include "MqttSnUnsubscribeMessage.h"
#include <string.h>
static int32_t parse_unsubscribe_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
static int32_t parse_unsubscribe_flags(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint8_t *dup, int8_t *qos,
                                       uint8_t *topic_id_type, uint16_t length);
static int32_t parse_unsubscribe_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id);
static int32_t parse_unsubscribe_predefined_topic(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *topic_id);
static int32_t parse_unsubscribe_short_topic_name(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *topic_id);
static int32_t parse_unsubscribe_topic_name(const uint8_t *src_pos, uint16_t src_len, int32_t *parse_bytes, char *topic_name,
                                            uint16_t *topic_name_length, uint16_t topic_name_max_length);

int32_t parse_unsubscribe_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, msg_id);
}

int32_t parse_unsubscribe_byte(uint8_t *dup, int8_t *qos, uint8_t *topic_id_type, uint16_t *msg_id, uint16_t *topic_id, char *topic_name,
                               uint16_t *topic_name_length, uint16_t topic_name_max_length, const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_unsubscribe_header(&h, data, data_len, &parsed_bytes) < 0)) {
        return -1;
    }
    if ((parsed_bytes = parse_unsubscribe_flags(data + parsed_bytes, data_len, &parsed_bytes, dup, qos, topic_id_type, h.length)) < 0) {
        return -1;
    }
    if ((parsed_bytes = parse_unsubscribe_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id)) < 0) {
        return -1;
    }

    if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID) {
        if ((parsed_bytes = parse_unsubscribe_predefined_topic(data + parsed_bytes, data_len, &parsed_bytes, topic_id)) < 0) {
            return -1;
        }
    } else if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME) {
        if ((parsed_bytes = parse_unsubscribe_short_topic_name(data + parsed_bytes, data_len, &parsed_bytes, topic_id)) < 0) {
            return -1;
        }
    } else if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME) {
        if ((parsed_bytes = parse_unsubscribe_topic_name(data + parsed_bytes, data_len, &parsed_bytes, topic_name, topic_name_length,
                                                         topic_name_max_length))) {
            return -1;
        }
    }
    return parsed_bytes;
}
int32_t get_unsubscribe_msg_len(uint16_t topic_name_length) {
    if (topic_name_length > UINT16_MAX - MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MAX_LENGTH) {
        return -1;
    }
    uint16_t total_length = MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MIN_LENGTH + topic_name_length;
    if (total_length > UINT8_MAX) {
        total_length = MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MAX_LENGTH + topic_name_length;
    }
    return total_length;
}
int32_t generate_unsubscribe_topic_name_header(uint8_t *dst, uint16_t dst_len, uint16_t topic_name_length, int32_t *used_bytes) {
    int32_t total_length = get_unsubscribe_msg_len(topic_name_length);
    if (total_length < 0 || dst_len < total_length) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, total_length, UNSUBSCRIBE);
}
int32_t generate_unsubscribe_topic_name(uint8_t *dst, uint16_t dst_len, uint8_t dup, uint16_t msg_id, const char *topic_name,
                                        uint16_t topic_name_length) {
    int32_t used_bytes = 0;
    if (generate_unsubscribe_topic_name_header(dst, dst_len, topic_name_length, &used_bytes) < 0) {
        return -1;
    }
    if (generate_flags(dst + used_bytes, dst_len, dup, MQTT_SN_FLAG_QOS_0, MQTT_SN_FLAG_RETAIN_FALSE, MQTT_SN_FLAG_WILL_FALSE,
                       MQTT_SN_FLAG_CLEAN_SESSION_FALSE, MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME, &used_bytes) < 0) {
        return -1;
    }
    if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes) < 0) {
        return -1;
    }
    if (generate_string(dst + used_bytes, dst_len, topic_name, topic_name_length, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t generate_unsubscribe_topic_id_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    int32_t total_length = MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICID_LENGTH;
    if (total_length < 0 || dst_len < total_length) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, total_length, UNSUBSCRIBE);
}
int32_t generate_unsubscribe_topic_id(uint8_t *dst, uint16_t dst_len, uint8_t dup, uint8_t topic_id_type, uint16_t msg_id,
                                      uint16_t topic_id) {
    int32_t used_bytes = 0;
    if (generate_unsubscribe_topic_id_header(dst, dst_len, &used_bytes) < 0) {
        return -1;
    }
    if (generate_flags(dst + used_bytes, dst_len, dup, MQTT_SN_FLAG_QOS_0, MQTT_SN_FLAG_RETAIN_FALSE, MQTT_SN_FLAG_WILL_FALSE,
                       MQTT_SN_FLAG_CLEAN_SESSION_FALSE, topic_id_type, &used_bytes) < 0) {
        return -1;
    }
    if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes) < 0) {
        return -1;
    }
    if (generate_topic_id(dst + used_bytes, dst_len, topic_id, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t generate_unsubscribe(uint8_t *dst, uint16_t dst_len, uint8_t dup, uint8_t topic_id_type, uint16_t msg_id, uint16_t topic_id,
                             const char *topic_name, uint16_t topic_name_length) {
    memset(dst, 0, dst_len);
    if (topic_id_type == MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME && topic_name != NULL && topic_name_length > 0) {
        return generate_unsubscribe_topic_name(dst, dst_len, dup, msg_id, topic_name, topic_name_length);
    } else if (topic_id_type == MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID ||
               topic_id_type == MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME) {
        return generate_unsubscribe_topic_id(dst, dst_len, dup, topic_id_type, msg_id, topic_id);
    }
    return -1;
}
int32_t parse_unsubscribe_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, UNSUBSCRIBE, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length < MQTT_SN_MESSAGE_UNSUBSCRIBE_MIN_LENGTH) {
        return -1;
    }
    /*
  if (h->length < MQTT_SN_MESSAGE_SUBSCRIBE_MIN_LENGTH) {
    return -1;
  }
  */
    return *parsed_bytes;
}
int32_t parse_unsubscribe_flags(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint8_t *dup, int8_t *qos,
                                uint8_t *topic_id_type, uint16_t length) {
    if (parse_mqtt_sn_flags(src_pos, src_length, parsed_bytes, dup, qos, NULL, NULL, NULL, topic_id_type) < 0) {
        return -1;
    }
    if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME) {
        if (length < MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICNAME_MIN_LENGTH) {
            return -1;
        }
    }
    if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID) {
        if (length < MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICID_LENGTH) {
            return -1;
        }
    }
    if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME) {
        if (length < MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICID_LENGTH) {
            return -1;
        }
    }
    return *parsed_bytes;
}
int32_t parse_unsubscribe_predefined_topic(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *topic_id) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, topic_id);
}

int32_t parse_unsubscribe_topic_name(const uint8_t *src_pos, uint16_t src_len, int32_t *parse_bytes, char *topic_name,
                                     uint16_t *topic_name_length, uint16_t topic_name_max_length) {
    return parse_mqtt_sn_char_until_end_byte(src_pos, src_len, parse_bytes, topic_name, topic_name_length, topic_name_max_length);
}
int32_t parse_unsubscribe_short_topic_name(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *topic_id) {
    return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, topic_id);
}
