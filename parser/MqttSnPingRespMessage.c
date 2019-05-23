//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnPingRespMessage.h"

int32_t parse_ping_resp_header(const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if (parse_header(&h, PINGREQ, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (h.length != MQTT_SN_MESSAGE_PINGREQ_LENGTH) {
    return -1;
  }
  return parsed_bytes;
}
