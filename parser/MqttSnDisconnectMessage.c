//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnDisconnectMessage.h"

static int32_t parse_disconnect_header(ParsedMqttSnHeader *h,
                                       const uint8_t *data,
                                       uint16_t data_len,
                                       int32_t *parsed_bytes);
static int32_t parse_disconnect_duration(const uint8_t *src_pos,
                                         uint16_t src_len,
                                         int32_t *parsed_bytes,
                                         uint16_t *duration);

int32_t parse_disconnect_byte(uint16_t *duration, const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_disconnect_header(&h, data, data_len, &parsed_bytes) < 0)) {
    return -1;
  }
  if ((parsed_bytes = parse_disconnect_duration(data + parsed_bytes, data_len, &parsed_bytes, duration)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_disconnect_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, PINGREQ, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_DISCONNECT_MIN_LENGTH) {
    return -1;
  }
  if (h->length > MQTT_SN_MESSAGE_DISCONNECT_MAX_LENGTH) {
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_disconnect_duration(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *duration) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, duration);
}
