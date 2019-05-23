//
// Created by SomeDude on 23.05.2019.
//

#include "MqttSnPingReqMessage.h"
static int32_t parse_ping_req_header(ParsedMqttSnHeader *h,
                                     const uint8_t *data,
                                     uint16_t data_len,
                                     int32_t *parsed_bytes);
static int32_t parse_ping_req_client_id(const uint8_t *src_pos,
                                        uint16_t src_len,
                                        int32_t *parsed_bytes,
                                        char *client_id,
                                        uint16_t *client_id_length,
                                        uint8_t client_id_max_length);

int32_t parse_ping_req_byte(char *client_id,
                            uint16_t *client_id_length,
                            uint8_t client_id_max_length,
                            const uint8_t *data,
                            uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_ping_req_header(&h, data, data_len, &parsed_bytes) < 0)) {
    return -1;
  }
  if ((parsed_bytes = parse_ping_req_client_id(data + parsed_bytes,
                                               data_len,
                                               &parsed_bytes,
                                               client_id,
                                               client_id_length,
                                               client_id_max_length)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_ping_req_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, PINGREQ, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_PINGREQ_MIN_LENGTH) {
    return -1;
  }
  if (h->length > MQTT_SN_MESSAGE_PINGREQ_W_CLIENTID_MAX_LENGTH) {
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_ping_req_client_id(const uint8_t *src_pos,
                                 uint16_t src_len,
                                 int32_t *parsed_bytes,
                                 char *client_id,
                                 uint16_t *client_id_length,
                                 uint8_t client_id_max_length) {
  if (client_id_max_length > MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH) {
    client_id_max_length = MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH;
  }
  return parse_mqtt_sn_client_id_byte(src_pos,
                                      src_len,
                                      parsed_bytes,
                                      client_id,
                                      client_id_length,
                                      client_id_max_length);
}
