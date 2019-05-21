//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnConnackMessage.h"

static int32_t parse_connack_header(ParsedMqttSnHeader *h,
                                    const uint8_t *data,
                                    uint16_t data_len,
                                    int32_t *parsed_bytes);
static int32_t parse_connack_return_code_byte(const uint8_t *src_pos,
                                              uint16_t src_len,
                                              int32_t *parsed_bytes,
                                              MQTT_SN_RETURN_CODE *return_code);

int32_t parse_connack_byte(MQTT_SN_RETURN_CODE *return_code, const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_connack_header(&h, data, data_len, &parsed_bytes) < 0)) {
    return -1;
  }
  if ((parsed_bytes = parse_connack_return_code_byte(data + parsed_bytes, data_len, &parsed_bytes, return_code) < 0)) {
    return -1;
  }
  return 0;
}
int32_t parse_connack_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, CONNACK, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length != MQTT_SN_MESSAGE_CONNACK_LENGTH) {
    // invalid message length for connack
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_connack_return_code_byte(const uint8_t *src_pos,
                                       uint16_t src_len,
                                       int32_t *parsed_bytes,
                                       MQTT_SN_RETURN_CODE *return_code) {
  return parse_mqtt_sn_return_code_byte(src_pos, src_len, parsed_bytes, return_code);
}
