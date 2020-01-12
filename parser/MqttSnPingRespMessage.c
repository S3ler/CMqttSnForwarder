//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnPingRespMessage.h"

int32_t parse_ping_resp_byte(const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if (parse_header(&h, PINGRESP, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (h.length != MQTT_SN_MESSAGE_PINGRESP_LENGTH) {
    return -1;
  }
  return parsed_bytes;
}
int32_t generate_ping_resp(uint8_t *dst, uint16_t dst_len) {
  int32_t used_bytes = 0;
  if (generate_mqtt_sn_header(dst, dst_len, &used_bytes, MQTT_SN_MESSAGE_PINGRESP_LENGTH, PINGRESP) < 0) {
    return -1;
  }
  return used_bytes;
}
