//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnUnsubackMessage.h"

static int32_t parse_unsuback_header(ParsedMqttSnHeader *h,
                                     const uint8_t *data,
                                     uint16_t data_len,
                                     int32_t *parsed_bytes);
static int32_t parse_unsuback_msg_id(const uint8_t *src_pos,
                                     uint16_t src_length,
                                     int32_t *parsed_bytes,
                                     uint16_t *msg_id);
int32_t parse_unsuback_byte(uint16_t *msg_id, const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if (parse_unsuback_header(&h, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (parse_unsuback_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_unsuback_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, UNSUBACK, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length != MQTT_SN_MESSAGE_UNSUBACK_LENGTH) {
    // invalid message length for unsuback
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_unsuback_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, msg_id);
}
