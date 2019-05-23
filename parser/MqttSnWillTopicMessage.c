//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnWillTopicMessage.h"

static int32_t parse_will_topic_header(ParsedMqttSnHeader *h,
                                       const uint8_t *data,
                                       uint16_t data_len,
                                       int32_t *parsed_bytes);
static int32_t parse_will_topic_flags(const uint8_t *src_pos,
                                      uint16_t src_len,
                                      int32_t *parsed_bytes,
                                      int8_t *qos,
                                      uint8_t *retain);
static int32_t parse_will_topic_will_topic(const uint8_t *src_pos,
                                           uint16_t src_len,
                                           int32_t *parsed_bytes,
                                           char *will_topic,
                                           uint16_t *will_topic_length,
                                           uint16_t will_topic_max_length);

int32_t parse_will_topic_byte(int8_t *qos,
                              uint8_t *retain,
                              char *will_topic,
                              uint16_t *will_topic_length,
                              uint16_t will_topic_max_length,
                              const uint8_t *data,
                              uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_will_topic_header(&h, data, data_len, &parsed_bytes)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_will_topic_flags(data + parsed_bytes, data_len, &parsed_bytes, qos, retain)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_will_topic_will_topic(data + parsed_bytes,
                                                  data_len,
                                                  &parsed_bytes,
                                                  will_topic,
                                                  will_topic_length,
                                                  will_topic_max_length)) < 0) {
    return -1;
  }
  return parsed_bytes;
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
int32_t parse_will_topic_flags(const uint8_t *src_pos,
                               uint16_t src_len,
                               int32_t *parsed_bytes,
                               int8_t *qos,
                               uint8_t *retain) {
  return parse_mqtt_sn_flags(src_pos, src_len, parsed_bytes, NULL, qos, retain, NULL, NULL, NULL);
}
int32_t parse_will_topic_will_topic(const uint8_t *src_pos,
                                    uint16_t src_len,
                                    int32_t *parsed_bytes,
                                    char *will_topic,
                                    uint16_t *will_topic_length,
                                    uint16_t will_topic_max_length) {
  if (will_topic_max_length > MQTT_SN_MESSAGE_WILLTOPIC_WILLTOPIC_MAX_LENGTH) {
    will_topic_max_length = MQTT_SN_MESSAGE_WILLTOPIC_WILLTOPIC_MAX_LENGTH;
  }
  return parse_mqtt_sn_char_until_end_byte(src_pos,
                                           src_len,
                                           parsed_bytes,
                                           will_topic,
                                           will_topic_length,
                                           will_topic_max_length);
}
