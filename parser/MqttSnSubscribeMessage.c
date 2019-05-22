//
// Created by SomeDude on 22.05.2019.
//

#include "MqttSnSubscribeMessage.h"

static int32_t parse_subscribe_header(ParsedMqttSnHeader *h,
                                      const uint8_t *data,
                                      uint16_t data_len,
                                      int32_t *parsed_bytes);
static int32_t parse_subscribe_flags(const uint8_t *src_pos,
                                     uint16_t src_length,
                                     int32_t *parsed_bytes,
                                     uint8_t *dup,
                                     int8_t *qos,
                                     uint8_t *topic_id_type,
                                     uint16_t length);
static int32_t parse_subscribe_msg_id(const uint8_t *src_pos,
                                      uint16_t src_length,
                                      int32_t *parsed_bytes,
                                      uint16_t *msg_id);
static int32_t parse_subscribe_predefined_topic(const uint8_t *src_pos,
                                                uint16_t src_len,
                                                int32_t *parsed_bytes,
                                                uint16_t *topic_id);
static int32_t parse_subscribe_short_topic_name(const uint8_t *src_pos,
                                                uint16_t src_len,
                                                int32_t *parsed_bytes,
                                                uint16_t *topic_id);
static int32_t parse_subscribe_topic_name(const uint8_t *src_pos,
                                          uint16_t src_len,
                                          int32_t *parse_bytes,
                                          char *topic_name,
                                          uint16_t *topic_name_length,
                                          uint16_t topic_name_max_length);

int32_t parse_subscribe_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, msg_id);
}

int32_t parse_subscribe_byte(uint8_t *dup,
                             int8_t *qos,
                             uint8_t *topic_id_type,
                             uint16_t *msg_id,
                             uint16_t *topic_id,
                             char *topic_name,
                             uint16_t *topic_name_length,
                             uint16_t topic_name_max_length,
                             const uint8_t *data,
                             uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_subscribe_header(&h, data, data_len, &parsed_bytes) < 0)) {
    return -1;
  }
  if ((parsed_bytes = parse_subscribe_flags(data + parsed_bytes,
                                            data_len, &parsed_bytes,
                                            dup,
                                            qos,
                                            topic_id_type,
                                            h.length)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_subscribe_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id)) < 0) {
    return -1;
  }

  if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID) {
    if ((parsed_bytes = parse_subscribe_predefined_topic(data + parsed_bytes, data_len, &parsed_bytes, topic_id)) < 0) {
      return -1;
    }
  } else if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME) {
    if ((parsed_bytes = parse_subscribe_short_topic_name(data + parsed_bytes, data_len, &parsed_bytes, topic_id)) < 0) {
      return -1;
    }
  } else if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME) {
    if ((parsed_bytes = parse_subscribe_topic_name(data + parsed_bytes,
                                                   data_len,
                                                   &parsed_bytes,
                                                   topic_name,
                                                   topic_name_length,
                                                   topic_name_max_length))) {
      return -1;
    }
  }
  return parsed_bytes;
}
int32_t parse_subscribe_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, SUBSCRIBE, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_SUBSCRIBE_MIN_LENGTH) {
    return -1;
  }
  /*
  if (h->length < MQTT_SN_MESSAGE_SUBSCRIBE_MIN_LENGTH) {
    return -1;
  }
  */
  return *parsed_bytes;
}
int32_t parse_subscribe_flags(const uint8_t *src_pos,
                              uint16_t src_length,
                              int32_t *parsed_bytes,
                              uint8_t *dup,
                              int8_t *qos,
                              uint8_t *topic_id_type,
                              uint16_t length) {
  if (parse_mqtt_sn_flags(src_pos, src_length, parsed_bytes, dup, qos, NULL, NULL, NULL, topic_id_type) < 0) {
    return -1;
  }
  if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME) {
    if (length < MQTT_SN_MESSAGE_SUBSCRIBE_W_TOPICNAME_MIN_LENGTH) {
      return -1;
    }
  }
  if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID) {
    if (length < MQTT_SN_MESSAGE_SUBSCRIBE_W_TOPICID_LENGTH) {
      return -1;
    }
  }
  if ((*topic_id_type) == MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME) {
    if (length < MQTT_SN_MESSAGE_SUBSCRIBE_W_TOPICID_LENGTH) {
      return -1;
    }
  }
  return *parsed_bytes;
}
int32_t parse_subscribe_predefined_topic(const uint8_t *src_pos,
                                         uint16_t src_len,
                                         int32_t *parsed_bytes,
                                         uint16_t *topic_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, topic_id);
}

int32_t parse_subscribe_topic_name(const uint8_t *src_pos,
                                   uint16_t src_len,
                                   int32_t *parse_bytes,
                                   char *topic_name,
                                   uint16_t *topic_name_length,
                                   uint16_t topic_name_max_length) {
  return parse_mqtt_sn_char_until_end_byte(src_pos,
                                           src_len,
                                           parse_bytes,
                                           topic_name,
                                           topic_name_length,
                                           topic_name_max_length);
}
int32_t parse_subscribe_short_topic_name(const uint8_t *src_pos,
                                         uint16_t src_len,
                                         int32_t *parsed_bytes,
                                         uint16_t *topic_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, topic_id);
}
