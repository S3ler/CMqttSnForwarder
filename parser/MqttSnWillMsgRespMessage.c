//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnWillMsgRespMessage.h"

static int32_t parse_will_msg_resp_header(ParsedMqttSnHeader *h,
                                          const uint8_t *data,
                                          uint16_t data_len,
                                          int32_t *parsed_bytes);
static int32_t parse_will_msg_resp_return_code(const uint8_t *src_pos,
                                               uint16_t src_len,
                                               int32_t *parsed_byte,
                                               MQTT_SN_RETURN_CODE *return_code);

int32_t parse_will_msg_resp_byte(MQTT_SN_RETURN_CODE *return_code, const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_will_msg_resp_header(&h, data, data_len, &parsed_bytes) < 0)) {
    return -1;
  }
  if ((parsed_bytes = parse_will_msg_resp_return_code(data + data_len, data_len, &parsed_bytes, return_code)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_will_msg_resp_header(ParsedMqttSnHeader *h,
                                   const uint8_t *data,
                                   uint16_t data_len,
                                   int32_t *parsed_bytes) {
  if (parse_header(h, WILLMSGRESP, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length != MQTT_SN_MESSAGE_WILLMSGRESP_LENGTH) {
    // invalid message length for will topic resp
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_will_msg_resp_return_code(const uint8_t *src_pos,
                                        uint16_t src_len,
                                        int32_t *parsed_byte,
                                        MQTT_SN_RETURN_CODE *return_code) {
  return parse_mqtt_sn_return_code_byte(src_pos, src_len, parsed_byte, return_code);
}
