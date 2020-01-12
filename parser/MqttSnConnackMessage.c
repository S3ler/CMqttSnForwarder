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
  if (parse_connack_header(&h, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (parse_connack_return_code_byte(data + parsed_bytes, data_len, &parsed_bytes, return_code) < 0) {
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

static int32_t generate_connack_header(uint8_t *dst_pos, uint16_t dst_len, int32_t *used_bytes) {
  return generate_mqtt_sn_header(dst_pos, dst_len, used_bytes, MQTT_SN_MESSAGE_CONNACK_LENGTH, CONNACK);
}
static int32_t generate_connack_return_code(uint8_t *dst_pos,
                                            uint16_t dst_len,
                                            MQTT_SN_RETURN_CODE return_code,
                                            int32_t *used_bytes) {
  return generate_mqtt_sn_return_code(dst_pos, dst_len, return_code, used_bytes);
}

int32_t generate_connack(uint8_t *dst, uint16_t dst_len, MQTT_SN_RETURN_CODE return_code) {
  int32_t used_bytes = 0;
  if (generate_connack_header(dst + used_bytes, dst_len, &used_bytes) < 0) {
    return -1;
  }
  if (generate_connack_return_code(dst + used_bytes, dst_len, return_code, &used_bytes) < 0) {
    return -1;
  }
  return used_bytes;
}
int32_t parse_connack(ParsedMqttSnConnack *mqtt_sn_connack, const uint8_t *data, uint16_t data_len) {
  return parse_connack_byte(&mqtt_sn_connack->return_code, data, data_len);
}
