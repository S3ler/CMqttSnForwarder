//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnWillMsgMessage.h"

static int32_t parse_will_msg_header(ParsedMqttSnHeader *h,
                                     const uint8_t *data,
                                     uint16_t data_len,
                                     int32_t *parsed_bytes);
static int32_t parse_will_topic_will_topic(const uint8_t *src_pos,
                                           uint16_t src_len,
                                           int32_t *parsed_bytes,
                                           char *will_topic,
                                           uint16_t *will_topic_length,
                                           uint16_t will_topic_max_length);

int32_t parse_will_msg_byte(char *will_msg,
                            uint16_t *will_msg_length,
                            uint16_t will_msg_max_length,
                            const uint8_t *data,
                            uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_will_msg_header(&h, data, data_len, &parsed_bytes)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_will_topic_will_topic(data + parsed_bytes,
                                                  data_len,
                                                  &parsed_bytes,
                                                  will_msg,
                                                  will_msg_length,
                                                  will_msg_max_length)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_will_topic_will_topic(const uint8_t *src_pos,
                                    uint16_t src_len,
                                    int32_t *parsed_bytes,
                                    char *will_topic,
                                    uint16_t *will_topic_length,
                                    uint16_t will_topic_max_length) {
  if (will_topic_max_length > MQTT_SN_MESSAGE_WILLMSG_WILLMSG_MAX_LENGTH) {
    will_topic_max_length = MQTT_SN_MESSAGE_WILLMSG_WILLMSG_MAX_LENGTH;
  }
  return parse_mqtt_sn_char_until_end_byte(src_pos,
                                           src_len,
                                           parsed_bytes,
                                           will_topic,
                                           will_topic_length,
                                           will_topic_max_length);
}
int32_t parse_will_msg_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if ((*parsed_bytes = parse_header(h, WILLMSG, data, data_len, parsed_bytes)) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_MIN_LENGTH) {
    return -1;
  }
  /*
  if (h->length > MQTT_SN_MESSAGE_MAX_LENGTH) {
    return -1;
  }
  */
  return *parsed_bytes;
}
