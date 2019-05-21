//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnRegister.h"

static int32_t parse_register_header(ParsedMqttSnHeader *h,
                                     const uint8_t *data,
                                     uint16_t data_len,
                                     int32_t *parsed_bytes);
static int32_t parse_register_topic_id(const uint8_t *src_pos,
                                       uint16_t src_length,
                                       int32_t *parsed_bytes,
                                       uint16_t *topic_id);
static int32_t parse_register_msg_id(const uint8_t *src_pos,
                                     uint16_t src_length,
                                     int32_t *parsed_bytes,
                                     uint16_t *msg_id);
static int32_t parse_register_topic_name(const uint8_t *src_pos,
                                         uint16_t src_len,
                                         int32_t *parsed_bytes,
                                         char *topic_name,
                                         uint16_t *topic_name_length,
                                         uint16_t topic_name_max_length);

int32_t parse_register_byte(uint16_t *topic_id,
                            uint16_t *msg_id,
                            char *topic_name,
                            uint16_t *topic_name_length,
                            uint16_t max_topic_name_length,
                            const uint8_t *data,
                            uint16_t data_len) {

  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_register_header(&h, data, data_len, &parsed_bytes)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_register_topic_id(data + parsed_bytes, data_len, &parsed_bytes, topic_id)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_register_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_register_topic_name(data + parsed_bytes,
                                                data_len,
                                                &parsed_bytes,
                                                topic_name,
                                                topic_name_length,
                                                max_topic_name_length)) < 0) {
    return -1;
  }
  return 0;
}
int32_t parse_register_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, REGISTER, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_REGISTER_MIN_LENGTH) {
    return -1;
  }
  /*
  if (h->length > MQTT_SN_MESSAGE_REGISTER_MAX_LENGTH) {
    return -1;
  }
  */
  return *parsed_bytes;
}
int32_t parse_register_topic_id(const uint8_t *src_pos,
                                uint16_t src_length,
                                int32_t *parsed_bytes,
                                uint16_t *topic_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, topic_id);
}
int32_t parse_register_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, msg_id);
}
int32_t parse_register_topic_name(const uint8_t *src_pos,
                                  uint16_t src_len,
                                  int32_t *parsed_bytes,
                                  char *topic_name,
                                  uint16_t *topic_name_length,
                                  uint16_t topic_name_max_length) {
  if (topic_name_max_length > MQTT_SN_MESSAGE_REGISTER_TOPICNAME_MAX_LENGTH) {
    topic_name_max_length = MQTT_SN_MESSAGE_REGISTER_TOPICNAME_MAX_LENGTH;
  }
  return parse_mqtt_sn_char_until_end_byte(src_pos,
                                           src_len,
                                           parsed_bytes,
                                           topic_name,
                                           topic_name_length,
                                           topic_name_max_length);
}


