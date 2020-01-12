//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnRegAckMessage.h"
static int32_t parse_regack_header(ParsedMqttSnHeader *h,
                                   const uint8_t *data,
                                   uint16_t data_len,
                                   int32_t *parsed_bytes);
static int32_t parse_regack_topic_id(const uint8_t *src_pos,
                                     uint16_t src_length,
                                     int32_t *parsed_bytes,
                                     uint16_t *topic_id);
static int32_t parse_regack_msg_id(const uint8_t *src_pos,
                                   uint16_t src_length,
                                   int32_t *parsed_bytes,
                                   uint16_t *msg_id);
static int32_t parse_regack_return_code_byte(const uint8_t *src_pos,
                                             uint16_t src_len,
                                             int32_t *parsed_bytes,
                                             MQTT_SN_RETURN_CODE *return_code);
int32_t parse_regack_bytes(uint16_t *topic_id,
                           uint16_t *msg_id,
                           MQTT_SN_RETURN_CODE *return_code,
                           const uint8_t *data,
                           uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if (parse_regack_header(&h, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (parse_regack_topic_id(data + parsed_bytes, data_len, &parsed_bytes, topic_id) < 0) {
    return -1;
  }
  if (parse_regack_msg_id(data + parsed_bytes, data_len, &parsed_bytes, msg_id) < 0) {
    return -1;
  }
  if (parse_regack_return_code_byte(data + parsed_bytes, data_len, &parsed_bytes, return_code) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_regack(MqttSnRegack *mqttSnRegAck, const uint8_t *data, uint16_t data_len) {
    return parse_regack_bytes(&mqttSnRegAck->topic_id, &mqttSnRegAck->msg_id, &mqttSnRegAck->return_code, data, data_len);
}
int32_t generate_regack_header(uint8_t *dst, uint16_t dst_len, int32_t *used_bytes) {
    if (dst_len < MQTT_SN_MESSAGE_REGACK_LENGTH) {
        return -1;
    }
    return generate_mqtt_sn_header(dst, dst_len, used_bytes, MQTT_SN_MESSAGE_REGACK_LENGTH, REGACK);
}
int32_t generate_regack(uint8_t *dst, uint16_t dst_len, uint16_t topic_id, uint16_t msg_id, MQTT_SN_RETURN_CODE return_code) {
    int32_t used_bytes = 0;
    if (generate_regack_header(dst + used_bytes, dst_len, &used_bytes) < 0) {
        return -1;
    }
    if (generate_topic_id(dst + used_bytes, dst_len, topic_id, &used_bytes) < 0) {
        return -1;
    }
    if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes) < 0) {
        return -1;
    }
    if (generate_mqtt_sn_return_code(dst + used_bytes, dst_len, return_code, &used_bytes) < 0) {
        return -1;
    }
    return used_bytes;
}
int32_t parse_regack_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, REGACK, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length != MQTT_SN_MESSAGE_REGACK_LENGTH) {
    // invalid message length for regack
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_regack_topic_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *topic_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, topic_id);
}
int32_t parse_regack_msg_id(const uint8_t *src_pos, uint16_t src_length, int32_t *parsed_bytes, uint16_t *msg_id) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_length, parsed_bytes, msg_id);
}
int32_t parse_regack_return_code_byte(const uint8_t *src_pos,
                                      uint16_t src_len,
                                      int32_t *parsed_bytes,
                                      MQTT_SN_RETURN_CODE *return_code) {
  return parse_mqtt_sn_return_code_byte(src_pos, src_len, parsed_bytes, return_code);
}

