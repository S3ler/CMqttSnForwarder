//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnWillMsgUpdMessage.h"

static int32_t parse_will_msg_upd_header(ParsedMqttSnHeader *h,
                                         const uint8_t *data,
                                         uint16_t data_len,
                                         int32_t *parsed_bytes);
static int32_t parse_will_msg_upd_will_msg(const uint8_t *src_pos,
                                           uint16_t src_len,
                                           int32_t *parsed_bytes,
                                           uint8_t *will_msg,
                                           uint16_t *will_msg_length,
                                           uint16_t will_msg_max_length);

int32_t parse_will_msg_upd_byte(const uint8_t *data,
                                uint16_t data_len,
                                uint8_t *will_msg,
                                uint16_t *will_msg_length,
                                uint16_t will_msg_max_length) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_will_msg_upd_header(&h, data, data_len, &parsed_bytes) < 0)) {
    return -1;
  }
  if ((parsed_bytes = parse_will_msg_upd_will_msg(data + parsed_bytes,
                                                  data_len,
                                                  &parsed_bytes,
                                                  will_msg,
                                                  will_msg_length,
                                                  will_msg_max_length)) < 0) {
    return -1;
  }
  return 0;
}
int32_t parse_will_msg_upd_header(ParsedMqttSnHeader *h,
                                  const uint8_t *data,
                                  uint16_t data_len,
                                  int32_t *parsed_bytes) {
  if (parse_header(h, WILLMSGUPD, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_WILLMSGUPD_MIN_LENGTH) {
    // invalid message length for will msg request
    return -1;
  }
  /*
  if (h->length < MQTT_SN_MESSAGE_WILLMSGUPD_MAX_LENGTH) {
    // invalid message length for will msg request
    return -1;
  }
  */
  return *parsed_bytes;
}
int32_t parse_will_msg_upd_will_msg(const uint8_t *src_pos,
                                    uint16_t src_len,
                                    int32_t *parsed_bytes,
                                    uint8_t *will_msg,
                                    uint16_t *will_msg_length,
                                    uint16_t will_msg_max_length) {
  if (will_msg_max_length > MQTT_SN_MESSAGE_WILLMSGUPD_WILLMSG_MAX_LENGTH) {
    will_msg_max_length = MQTT_SN_MESSAGE_WILLMSGUPD_WILLMSG_MAX_LENGTH;
  }
  return parse_mqtt_sn_uint8_until_end_byte(src_pos,
                                            src_len,
                                            parsed_bytes,
                                            will_msg,
                                            will_msg_length,
                                            will_msg_max_length);
}
