//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnConnectMessage.h"
#include "MqttSnMessageParser.h"

static int32_t parse_connect_header(ParsedMqttSnHeader *h,
                                    const uint8_t *data,
                                    uint16_t data_len,
                                    int32_t *parsed_bytes);
static int32_t parse_connect_flags(const uint8_t *src_pos,
                                   uint16_t src_len,
                                   int32_t *parsed_bytes,
                                   uint8_t *will,
                                   uint8_t *clean_session);
static int32_t parse_connect_protocol_id(const uint8_t *src_pos,
                                         uint16_t src_len,
                                         int32_t *parsed_bytes,
                                         uint8_t *protocol_id);
static int32_t parse_connect_duration(const uint8_t *src_pos,
                                      uint16_t src_len,
                                      int32_t *parsed_bytes,
                                      uint16_t *duration);
static int32_t parse_connect_client_id(const uint8_t *src_pos,
                                       uint16_t src_len,
                                       int32_t *parsed_bytes,
                                       char *client_id,
                                       uint16_t *client_id_length,
                                       uint8_t client_id_max_length);

int32_t parse_connect_message_byte(uint8_t *will,
                                   uint8_t *clean_session,
                                   uint8_t *protocol_id,
                                   uint16_t *duration,
                                   char *client_id,
                                   uint16_t *client_id_length,
                                   uint8_t client_id_max_length,
                                   const uint8_t *data,
                                   uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_connect_header(&h, data, data_len, &parsed_bytes)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_flags(data + parsed_bytes,
                                          data_len,
                                          &parsed_bytes,
                                          will,
                                          clean_session)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_protocol_id(data + parsed_bytes, data_len, &parsed_bytes, protocol_id)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_duration(data + parsed_bytes, data_len, &parsed_bytes, duration)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_client_id(data + parsed_bytes,
                                              data_len,
                                              &parsed_bytes,
                                              client_id,
                                              client_id_length,
                                              client_id_max_length)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_connect_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if ((*parsed_bytes = parse_header(h, CONNECT, data, data_len, parsed_bytes)) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_CONNECT_MIN_LENGTH) {
    return -1;
  }
  if (h->length > MQTT_SN_MESSAGE_CONNECT_MAX_LENGTH) {
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_connect_flags(const uint8_t *src_pos,
                            uint16_t src_len,
                            int32_t *parsed_bytes,
                            uint8_t *will,
                            uint8_t *clean_session) {
  return parse_mqtt_sn_flags(src_pos, src_len, parsed_bytes, NULL, NULL, NULL, will, clean_session, NULL);
}
int32_t parse_connect_protocol_id(const uint8_t *src_pos,
                                  uint16_t src_len,
                                  int32_t *parsed_bytes,
                                  uint8_t *protocol_id) {
  return parse_mqtt_sn_uint8_byte(src_pos, src_len, parsed_bytes, protocol_id);
}
int32_t parse_connect_duration(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *duration) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, duration);
}
int32_t parse_connect_client_id(const uint8_t *src_pos,
                                uint16_t src_len,
                                int32_t *parsed_bytes,
                                char *client_id,
                                uint16_t *client_id_length,
                                uint8_t client_id_max_length) {
  if (client_id_max_length > MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH) {
    client_id_max_length = MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH;
  }
  return parse_mqtt_sn_client_id_byte(src_pos,
                                      src_len,
                                      parsed_bytes,
                                      client_id,
                                      client_id_length,
                                      client_id_max_length);
}

