//
// Created by bele on 20.05.19.
//

#include "MqttSnSearchGwMessage.h"
static int32_t parse_searchgw_radius_byte(const uint8_t *src_pos,
                                          uint16_t src_len,
                                          int32_t *parsed_bytes,
                                          uint8_t *radius);
int32_t parse_searchgw_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
int32_t parse_searchgw_message_byte(uint8_t *radius, const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if (parse_searchgw_header(&h, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (parse_searchgw_radius_byte(data + parsed_bytes, data_len, &parsed_bytes, radius) < 0) {
    return -1;
  }
  return parsed_bytes;
}

int32_t parse_searchgw_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, SEARCHGW, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length != MQTT_SN_MESSAGE_SEARCHGW_LENGTH) {
    // invalid message length for search gw
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_searchgw_radius_byte(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *radius) {
  return parse_mqtt_sn_uint8_byte(src_pos, src_len, parsed_bytes, radius);
}

int32_t generate_searchgw_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes);
int32_t generate_searchgw_radius(uint8_t *dst_pos, uint16_t dst_len, int32_t *used_bytes, uint8_t radius);
int32_t generate_searchgw_message(uint8_t *dst, uint16_t dst_len, uint8_t radius) {
  int32_t used_bytes = 0;
  if (generate_searchgw_header(dst, dst_len, &used_bytes) < 0) {
    return -1;
  }
  if (generate_searchgw_radius(dst + used_bytes, dst_len, &used_bytes, radius) < 0) {
    return -1;
  }
  return used_bytes;
}
int32_t generate_searchgw_radius(uint8_t *dst_pos, uint16_t dst_len, int32_t *used_bytes, uint8_t radius) {
  *used_bytes += 1;
  if (*used_bytes > dst_len) {
    return -1;
  }
  *dst_pos = radius;
  return (*used_bytes);
}
int32_t generate_searchgw_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
  return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_SEARCHGW_LENGTH, SEARCHGW);
}
