//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnPublishMessage.h"
static int32_t parse_publish_header(ParsedMqttSnHeader *h, const uint8_t *src, uint16_t src_len, int32_t *parsed_bytes);
static int32_t parse_publish_flags(const uint8_t *src_pos,
                                   uint16_t src_len,
                                   int32_t *parsed_bytes,
                                   uint8_t *dup,
                                   int8_t *qos,
                                   uint8_t *retain,
                                   uint8_t *topic_id_type);
int32_t parse_publish_topic_id(const uint8_t *src_pos,
                               uint16_t src_len,
                               int32_t *parsed_bytes,
                               int8_t qos,
                               uint16_t *topic_id);
int32_t parse_publish_msg_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *msg_id);
int32_t parse_publish_data(const uint8_t *src_pos,
                           uint16_t src_len,
                           int32_t *parsed_bytes,
                           uint8_t *data,
                           uint16_t *data_length,
                           uint16_t data_max_length);
int32_t parse_publish_byte(uint8_t *dup,
                           int8_t *qos,
                           uint8_t *retain,
                           uint8_t *topic_id_type,
                           uint16_t *topic_id,
                           uint16_t *msg_id,
                           uint8_t *data,
                           uint16_t *data_length,
                           uint16_t data_max_length,
                           const uint8_t *src,
                           uint16_t src_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_publish_header(&h, src, src_len, &parsed_bytes)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_publish_flags(src + parsed_bytes, src_len, &parsed_bytes, dup, qos, retain, topic_id_type))
      < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_publish_topic_id(src + parsed_bytes, src_len, &parsed_bytes, (*qos), topic_id)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_publish_msg_id(src + parsed_bytes, src_len, &parsed_bytes, msg_id)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_publish_data(src + parsed_bytes,
                                         src_len,
                                         &parsed_bytes,
                                         data,
                                         data_length,
                                         data_max_length)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_publish_data(const uint8_t *src_pos,
                           uint16_t src_len,
                           int32_t *parsed_bytes,
                           uint8_t *data,
                           uint16_t *data_length,
                           uint16_t data_max_length) {
  return parse_mqtt_sn_uint8_until_end_byte(src_pos, src_len, parsed_bytes, data, data_length, data_max_length);
}
int32_t parse_publish_msg_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *msg_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, msg_id);
}
int32_t parse_publish_topic_id(const uint8_t *src_pos,
                               uint16_t src_len,
                               int32_t *parsed_bytes,
                               int8_t qos,
                               uint16_t *topic_id) {
  if (parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, topic_id) < 0) {
    return -1;
  }
  if ((qos <= 0) || ((*topic_id) != 0)) {
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
int32_t parse_publish_flags(const uint8_t *src_pos,
                            uint16_t src_len,
                            int32_t *parsed_bytes,
                            uint8_t *dup,
                            int8_t *qos,
                            uint8_t *retain,
                            uint8_t *topic_id_type) {
  return parse_mqtt_sn_flags(src_pos, src_len, parsed_bytes, dup, qos, retain, NULL, NULL, topic_id_type);
}
